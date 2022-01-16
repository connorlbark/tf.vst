#include "tf_vst.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "include/cppflow/ops.h"
#include "include/cppflow/model.h"
#include "include/cppflow/tensor.h"

#define MODELPATH "/Users/USERNAME/Documents/tf_vst/Model"

tf_vst::tf_vst(const InstanceInfo& info) : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kEnabled)->InitBool("Enabled", false);
  
  this->model = std::make_unique<cppflow::model>(MODELPATH);
  this->memory = std::make_unique<std::deque<sample>>(memory_capacity);
  for (int i = 0; i < memory_capacity; i++)
    this->memory->push_back(0.0);
  
#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new IVRadioButtonControl(b.GetCentredInside(100), kEnabled));

  };
#endif
}

#if IPLUG_DSP
void tf_vst::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  
  const bool enabled = GetParam(kEnabled)->Value();

  const int nChans = NOutChansConnected();
  
  // add new sample inputs to memory
  for (int s = 0; s < nFrames; s++) {
      if (memory_capacity >= this->memory->size()) {
        this->memory->pop_front();
        this->memory->pop_front();

      }
    this->memory->push_back(inputs[0][s]);
    this->memory->push_back(inputs[1][s]);
  }

  if (enabled and (nFrames < memory_capacity)) {
    
    auto input = cppflow::tensor(std::vector<float>{this->memory->begin(), this->memory->end()}, std::vector<int64_t>{1,memory_capacity/2 ,2});
    auto output = this->model->operator()(input);
    
    auto data = output.get_data<float>();
    unsigned long startIdx = data.size() - 2 * nFrames;
    for (int s = 0; s < nFrames; s++) {
      auto l = data[startIdx+2*s];
      auto r = data[startIdx+2*s+1];
      outputs[0][s] = l;
      outputs[1][s] = r;
    }
    
//    
//    for (int s = 0; s < nFrames; s++) {
//      for (int c = 0; c < nChans; c++) {
//        outputs[c][s] = values[c][s] * outGain;
//      }
//    }
  } else {
    for (int s = 0; s < nFrames; s++) {
      for (int c = 0; c < nChans; c++) {
        outputs[c][s] = inputs[c][s];
      }
    }
  }

}
#endif
