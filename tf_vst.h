#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "include/cppflow/model.h"
#include <memory>

const int kNumPresets = 1;
const int memory_capacity = 44100;

enum EParams
{
  kEnabled = 0,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class tf_vst final : public Plugin
{
public:
  std::unique_ptr<cppflow::model> model;
  std::unique_ptr<std::deque<sample>> memory;
  
  tf_vst(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
