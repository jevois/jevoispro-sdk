#!/bin/bash

# Download, build, and pack mediapipe for jevois-pro
# USAGE: jevoispro-build-mediapipe.sh [-y]

mpver="0.10.14"  # mediapipe
cvver="4.10.0"   # opencv
bzver="6.1.1"    # bazel
pbver="21.2"     # protoc
pyver="cp312"    # python version

set -e

# Architecture and paths:
libarch=`uname -p` # x86_64 or aarch64
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture` # amd64 or arm64

# Helper to ask questions
if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "JEVOIS: ${1}? [Y/n] "; fi }

####################################################################################################
question "Nuke any old bazel and install Bazel $bzver"
if [ "X$REPLY" != "Xn" ]; then

    sudo /bin/rm -f /usr/local/bin/bazel*
    
    if [ "X$libarch" = "Xaarch64" ]; then
        bzpkg="bazel-${bzver}-linux-arm64"
    else
        bzpkg="bazel-${bzver}-linux-${libarch}"
    fi
    
    wget https://github.com/bazelbuild/bazel/releases/download/${bzver}/${bzpkg}
    chmod a+x ${bzpkg}
    sudo mv ${bzpkg} /usr/local/bin/
    sudo ln -s /usr/local/bin/${bzpkg} /usr/local/bin/bazel
fi

####################################################################################################
question "Install Ubuntu packages"
if [ "X$REPLY" != "Xn" ]; then
    sudo apt install -y python3-dev cmake protobuf-compiler python3-pip git make openjdk-11-jdk-headless \
         python3-testresources python3-setuptools python3-numpy python3-pil python3-absl python3-full \
         python3-venv libusb-dev python-is-python3 ninja-build \
         mesa-common-dev libegl1-mesa-dev libgles2-mesa-dev mesa-utils
fi

####################################################################################################
question "Create virtual env ~/mp_env"
if [ "X$REPLY" != "Xn" ]; then
    deactivate &> /dev/null || true
    if [ -d ~/mp_env ]; then rm -rf ~/mp_env; fi
    python -m venv ~/mp_env
fi

if [ ! -d ~/mp_env ]; then echo "Need virtual mp_env -- ABORT"; exit 1; fi
source ~/mp_env/bin/activate

####################################################################################################
question "Install protobuf compiler $pbver"
if [ "X$REPLY" != "Xn" ]; then
    
    if [ "X$libarch" = "Xaarch64" ]; then
        pbpkg="protoc-${pbver}-linux-aarch_64.zip"
    else
        pbpkg="protoc-${pbver}-linux-${libarch}.zip"
    fi
    
    wget https://github.com/protocolbuffers/protobuf/releases/download/v${pbver}/${pbpkg}
    sudo unzip -o ${pbpkg} -d /usr/local
    /bin/rm ${pbpkg}
    sudo /bin/rm /usr/local/readme.txt
fi

####################################################################################################
# get mediapipe
if [ -d mediapipe ]; then
    question "Directory 'mediapipe' found, nuke it"
    if [ "X$REPLY" != "Xn" ]; then
        /bin/rm -rf mediapipe
        git clone --depth 1 -b v${mpver} https://github.com/google/mediapipe
    fi
else
    git clone --depth 1 -b v${mpver} https://github.com/google/mediapipe
fi

cd mediapipe

question "Install mediapipe pip3 requirements"
if [ "X$REPLY" != "Xn" ]; then
    pip3 install setuptools wheel ninja
    pip3 install -r requirements.txt
fi

# Remove unnecessary linked libraries that actually are not used by mediapipe:
sed -i -e "/\"imgcodecs\"/d;/\"calib3d\"/d;/\"features2d\"/d;/\"highgui\"/d" third_party/BUILD
sed -i -e "/\"video\"/d;/\"videoio\"/d" third_party/BUILD
sed -i -e "/-ljpeg/d;/-lpng/d;/-ltiff/d;/-lImath/d;/-lIlmImf/d;/-lHalf/d;/-lIex/d;/-lIlmThread/d" third_party/BUILD
sed -i -e "/-lrt/d;/-ldc1394/d;/-lavcodec/d;/-lavformat/d;/-lavutil/d;/-lswscale/d;/-lavresample/d" third_party/BUILD

# Custom build flags on platform:
if [ "X$arch" = "Xarm64" ]; then
    question "Limit compilation CPUs to avoid running out of RAM"
    if [ "X$REPLY" != "Xn" ]; then
        echo "build --local_cpu_resources=1" >> ~/.bazelrc
    fi

    # See here for optimizations:
    # https://github.com/superuser789/MediaPipe-on-RaspberryPi#readme

    cflags="-march=armv8-a+crc+simd -mcpu=cortex-a73 -mfpu=neon-fp-armv8 -mtune=cortex-a73 -mfloat-abi=hard -O3",
    sed -i -e "/# Optimization flags/i \        \"ENABLE_NEON\": \"ON\"," third_party/BUILD
    #sed -i -e "/# Optimization flags/i \        \"ENABLE_VFPV4\": \"ON\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"WITH_TENGINE\": \"OFF\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"WITH_CAROTENE\": \"ON\"," third_party/BUILD
    #sed -i -e "/# Optimization flags/i \        \"CMAKE_CXX_FLAGS"": \"${cflags}\"," third_party/BUILD

    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-O3'," setup.py
    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mfloat-abi=hard'," setup.py
    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mtune=cortex-a73'," setup.py
    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mcpu=cortex-a73'," setup.py
    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mfpu=neon-vfpv3'," setup.py
    #sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-march=armv8-a+crc+simd'," setup.py

    # We cannot enable the GPU as we get errors that stem from the fact that GL_VERSION_4_4 is not defined on
    # our platform:
    export MEDIAPIPE_DISABLE_GPU=1
fi

# Force 4 threads for tflite:
nth=4
sed -i -e "s/cc->Options<mediapipe::InferenceCalculatorOptions>().cpu_num_thread()/${nth}/" \
    mediapipe/calculators/tensor/inference_calculator_cpu.cc
sed -i -e "s/GetXnnpackNumThreads(opts_has_delegate, opts_delegate)/${nth}/" \
    mediapipe/calculators/tensor/inference_calculator_cpu.cc

sed -i -e "s/options.cpu_num_thread()/${nth}/" \
    mediapipe/calculators/tensor/inference_calculator_xnnpack.cc
sed -i -e "s/GetXnnpackNumThreads(opts_has_delegate, opts_delegate)/${nth}/" \
    mediapipe/calculators/tensor/inference_calculator_xnnpack.cc

sed -i -e "s/builder.SetNumThreads(interpreter_num_threads)/builder.SetNumThreads(${nth})/" \
    mediapipe/calculators/tensor/inference_interpreter_delegate_runner.cc

sed -i -e "s/return opts.delegate().xnnpack().num_threads()/return ${nth}/" \
    mediapipe/calculators/tflite/tflite_inference_calculator.cc
sed -i -e "s/return GetXnnpackDefaultNumThreads()/return ${nth}/" \
    mediapipe/calculators/tflite/tflite_inference_calculator.cc
sed -i -e "s/\&interpreter_/\&interpreter_, ${nth}/" \
    mediapipe/calculators/tflite/tflite_inference_calculator.cc
sed -i -e "/#endif  \/\/ __EMSCRIPTEN__/a interpreter_->SetNumThreads(${nth});/" \
    mediapipe/calculators/tflite/tflite_inference_calculator.cc
sed -i -e "s/GetXnnpackNumThreads(calculator_opts)/${nth}/" \
    mediapipe/calculators/tflite/tflite_inference_calculator.cc
    
# Enable coral TPU:
#sed -i -e "s/+ GPU_OPTIONS/+ GPU_OPTIONS + \[ '--define MEDIAPIPE_EDGE_TPU=all' \]/g" setup.py

# Update workspace and opencv_linux.BUILD to use our installed opencv:
# see https://unix.stackexchange.com/questions/281003/replace-2-successive-lines-using-sed
sed -i -e "/opencv_linux.BUILD/{N;s/path = \"\/usr\"/path = \"\/usr\/share\/jevoispro-opencv-${cvver}\"/}" WORKSPACE
sed -i -e 's/#"include\/opencv4/"include\/opencv4/g' third_party/opencv_linux.BUILD
sed -i -e "/linkopts = \[/a \        \"-L\/usr\/share\/jevoispro-opencv-${cvver}\/lib\"," third_party/opencv_linux.BUILD
sed -i -e "s/.so/.so.${cvver}/g" third_party/opencv_linux.BUILD

# select correct opencv version
sed -i -e "s/OPENCV_SO_VERSION = \"3.4\"/OPENCV_SO_VERSION = \"${cvver}\"/" third_party/BUILD
sed -i -e "s/opencv-3.4.10/opencv-${cvver}/" WORKSPACE
sed -i -e "s/3.4.10.tar.gz/${cvver}.tar.gz/" WORKSPACE

# set mediapipe version
sed -i -e "s/__version__ = 'dev'/__version__ = '${mpver}'/" setup.py

####################################################################################################
# Compile it
bazel clean --expunge
/bin/rm -rf ~/.cache/bazel
python3 setup.py gen_protos

# Build the wheel:
/bin/rm -rf /usr/include/opencv2
sudo ln -s /usr/share/jevoispro-opencv-${cvver}/include/opencv4/opencv2 /usr/include/opencv2

python3 setup.py install --link-opencv
python3 setup.py bdist_wheel --skip-build

sudo /bin/rm /usr/include/opencv2

# Copy result
wheel="mediapipe-${mpver}-${pyver}-${pyver}-linux_${libarch}.whl"
if [ -d /lab/jevois/pkg ]; then /bin/cp dist/${wheel} /lab/jevois/pkg; fi
echo "JEVOIS: Compilation success. Wheel is at: mediapipe/dist/${wheel}"

####################################################################################################
# Cleanup:
#bazel clean --expunge
#/bin/rm -rf ~/.cache/bazel
#deactivate
#/bin/rm -rf ~/mp_env

echo "JEVOIS: All done. Remember to load all objectron models... and possibly fix mediapipe/__init__.py"
