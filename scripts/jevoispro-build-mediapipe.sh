#!/bin/bash

# Download, build, and pack mediapipe for jevois-pro
# USAGE: jevoispro-build-opencv.sh [-y]

#mpver="0.8.10.2" # mediapipe
mpver="0.8.9" # mediapipe
cvver="4.6.0"    # opencv
bzver="5.2.0"    # bazel
pbver="21.2"     # protoc
pyver="cp38"     # python version

set -e

# Architecture and paths:
libarch=`uname -p` # x86_64 or aarch64
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture` # amd64 or aarch64

# Helper to ask questions
if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "JEVOIS: ${1}? [Y/n] "; fi }

####################################################################################################
if [ ! -x /usr/local/bin/bazel ]; then
    question "Install Bazel $bzver"
    if [ "X$REPLY" != "Xn" ]; then

        if [ "X$libarch" = "Xaarch64" ]; then
            bzpkg="bazel-${bzver}-linux-arm64"
        else
            bzpkg="bazel-${bzver}-linux-${libarch}"
        fi
        
        wget https://github.com/bazelbuild/bazel/releases/download/${bzver}/${bzpkg}-linux-${arch2}
        chmod a+x ${bzpkg}
        sudo mv ${bzpkg} /usr/local/bin/
        sudo ln -s /usr/local/bin/${bzpkg} /usr/local/bin/bazel
    fi
fi

####################################################################################################
question "Install Ubuntu and pip packages"
if [ "X$REPLY" != "Xn" ]; then
    sudo apt install -y  python3-dev cmake protobuf-compiler python3-pip git make openjdk-11-jdk-headless
    sudo pip3 install pip --upgrade
    sudo pip3 install testresources
    sudo pip3 install setuptools
    sudo pip3 install numpy --upgrade
    sudo pip3 install cpython --upgrade
    sudo pip3 install pillow --upgrade
fi

####################################################################################################
if [ ! -x /usr/local/bin/protoc ]; then
    question "Install protobuf compiler $pbver"
    if [ "X$REPLY" != "Xn" ]; then

        if [ "X$libarch" = "Xaarch64" ]; then
            pbpkg="protoc-${pbver}-linux-aarch_64.zip"
        else
            pbpkg="protoc-${pbver}-linux-${libarch}.zip"
        fi

        wget https://github.com/protocolbuffers/protobuf/releases/download/v${pbver}/${pbpkg}
        sudo unzip ${pbpkg} -d /usr/local
        /bin/rm ${pbpkg}
        sudo /bin/rm /usr/local/readme.txt
    fi
fi

####################################################################################################
# get mediapipe
if [ -d mediapipe ]; then
    question "Directory 'mediapipe' found, nuke it"
    if [ "X$REPLY" != "Xn" ]; then
        /bin/rm -rf mediapipe
        git clone -b v${mpver} https://github.com/google/mediapipe
    fi
else
    git clone -b v${mpver} https://github.com/google/mediapipe
fi

cd mediapipe

question "Install mediapipe pip3 requirements"
if [ "X$REPLY" != "Xn" ]; then
    sudo pip3 install -r requirements.txt
fi

# Remove unnecessary linked libraries that actually are not used by mediapipe:
sed -i -e "/\"imgcodecs\"/d;/\"calib3d\"/d;/\"features2d\"/d;/\"highgui\"/d" third_party/BUILD
sed -i -e "/\"video\"/d;/\"videoio\"/d" third_party/BUILD
sed -i -e "/-ljpeg/d;/-lpng/d;/-ltiff/d;/-lImath/d;/-lIlmImf/d;/-lHalf/d;/-lIex/d;/-lIlmThread/d" third_party/BUILD
sed -i -e "/-lrt/d;/-ldc1394/d;/-lavcodec/d;/-lavformat/d;/-lavutil/d;/-lswscale/d;/-lavresample/d" third_party/BUILD

# Disable carotene on aarch64 as it leads to undefined symbols errors, enable some optimizations:
if [ "X$arch" = "Xaarch64" ]; then
    # See here for optimizations:
    # https://github.com/superuser789/MediaPipe-on-RaspberryPi#readme

    cflags="-march=armv8-a+crc+simd -mcpu=cortex-a73 -mfpu=neon-fp-armv8 -mtune=cortex-a73 -mfloat-abi=hard -O3",
    sed -i -e "/# Optimization flags/i \        \"ENABLE_NEON\": \"OFF\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"ENABLE_VFPV4\": \"ON\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"WITH_TENGINE\": \"OFF\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"WITH_CAROTENE\": \"OFF\"," third_party/BUILD
    sed -i -e "/# Optimization flags/i \        \"CMAKE_CXX_FLAGS"": \"${cflags}\"," third_party/BUILD

    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-O3'," setup.py
    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mfloat-abi=hard'," setup.py
    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mtune=cortex-a73'," setup.py
    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mcpu=cortex-a73'," setup.py
    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-mfpu=neon-vfpv3'," setup.py
    sed -i -e "/'--compilation_mode=opt',/i \         '--copt=-march=armv8-a+crc+simd'," setup.py
fi

# Update workspace and opencv_linux.BUILD to use our installed opencv:
# see https://unix.stackexchange.com/questions/281003/replace-2-successive-lines-using-sed
sed -i -e "/opencv_linux.BUILD/{N;s/path = \"\/usr\"/path = \"\/usr\/share\/jevoispro-opencv-${cvver}\"/}" WORKSPACE
sed -i -e 's/#"include\/opencv4/"include\/opencv4/g' third_party/opencv_linux.BUILD
sed -i -e "/linkopts = \[/a \        \"-L\/usr\/share\/jevoispro-opencv-${cvver}\/lib\"," third_party/opencv_linux.BUILD

# select correct opencv version
sed -i -e "s/OPENCV_SO_VERSION = \"3.4\"/OPENCV_SO_VERSION = \"${cvver}\"/" third_party/BUILD
sed -i -e "s/opencv-3.4.10/opencv-${cvver}/" WORKSPACE
sed -i -e "s/3.4.10.tar.gz/${cvver}.tar.gz/" WORKSPACE

# set mediapipe version
sed -i -e "s/__version__ = 'dev'/__version__ = '${mpver}'/" setup.py

# Compile it
bazel clean --expunge
/bin/rm -rf ~/.cache/bazel
python3 setup.py gen_protos

# On host, we statically compile opencv and build a wheel; on platform, we dynamically link
# to opencv and install the package:
if [ "X${arch}" != "Xaarch64" ]; then
    python3 setup.py bdist_wheel

    # Copy result
    wheel="mediapipe-${mpver}-${pyver}-${pyver}-linux_${libarch}.whl"
    if [ -d /lab/jevois/pkg ]; then /bin/cp dist/${wheel} /lab/jevois/pkg; fi
    echo "JEVOIS: All done. Wheel is at: mediapipe/dist/${wheel}"
else
    sudo ln -s /usr/share/jevoispro-opencv-${cvver}/include/opencv4/opencv2 /usr/include/opencv2
    
    python3 setup.py bdist_wheel
    #sudo python3 setup.py install --link-opencv || true
    #sudo python3 setup.py install --link-opencv || true

    sudo rm /usr/include/opencv2
    echo "JEVOIS: All done and installed. Remember to load all objectron models..."
fi


bazel clean --expunge
/bin/rm -rf ~/.cache/bazel
