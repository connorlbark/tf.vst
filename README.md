# ConvModeler
A basic volume control effect plug-in with IGraphics GUI


# tensorflow setup

```bash

git clone ... # get tensorflow
cd tensorflow
./configure
bazel build --config=opt //tensorflow:libtensorflow_cc.so

mkdir /usr/local/include/tf
cp -r bazel-genfiles/ /usr/local/include/tf/
cp -r tensorflow /usr/local/include/tf/
cp -r third_party /usr/local/include/tf/
cp -r bazel-bin/libtensorflow_cc.so /usr/local/lib/
```