#!/bin/bash

# Download, build, and pack opencv for jevois-pro
# USAGE: jevoispro-build-opencv.sh [-y]

set -e

# OpenCV version to build:
ma=4
mi=10
pa=0

# Support for MyriadX ended with 2022.3.2 so we are stuck there...
vino="2022.3.2" # release
vinobranch="releases/2022/3" # github branch

# note: for vino 2021.4 use -DINF_ENGINE_RELEASE=${vino//./0}0000 \
# note: for vino 2021.4.2 use -DINF_ENGINE_RELEASE=${vino//./0}00 \
vinodots="${vino//[^\.]}"
if [ ${#vinodots} -eq 1 ]; then infrel=${vino//./0}0000 ; else infrel=${vino//./0}00 ; fi

# Our deb package release number:
pkgrel=1

# Architecture and paths:
libarch=`uname -p` # x86_64 or aarch64
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture` # amd64 or arm64
ncpu=`grep -c processor /proc/cpuinfo`
root=""

job=""
if [ $arch != "amd64" -a ! -f /.dockerenv ]; then job="4"; fi

# NOTE: 4.6.0 disable OpenCL on host build as it leads to an undefined symbol in python??
if [ $arch = "amd64" -a $mi -eq 6 ]; then JVOPENCL=OFF; else JVOPENCL=ON; fi

if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "JEVOIS: ${1}? [Y/n] "; fi }

####################################################################################################
# Install boost first and then decide on python version:
sudo apt -y install libboost-all-dev
if [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python312.so" ]; then pyver="3.12";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python38.so" ]; then pyver="3.8";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python3-py37.so" ]; then pyver="3.7";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python-py36.so" ]; then pyver="3.6";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python-py35.so" ]; then pyver="3.5";
else echo "Cannot find libboost_python py35, py36, py37, 38, or 312 -- ABORT"; exit 5; fi

####################################################################################################
# Compiler packages and dependencies
if [ ${uburel} = "18.04" -o ${uburel} = "19.04" ]; then
    gccver=8
    qt4lib="libqt4-dev"
elif [ ${uburel} = "20.04" ]; then
    gccver=10
    qt4lib="" # no more qt4 on 20.04; we already have qt5-default in the list below
else
    gccver=14
    qt4lib="" # no more qt4
fi

packages=( build-essential gcc-${gccver} g++-${gccver} gfortran-${gccver} cmake libboost-all-dev autoconf
           libeigen3-dev libgtk2.0-dev libdc1394-dev libjpeg-dev libpng-dev libtiff5-dev libavcodec-dev
           libavformat-dev libswscale-dev libxine2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libv4l-dev
           libtbb-dev ${qt4lib} libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev
           libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip qtbase5-dev
           python${pyver}-dev python3-numpy python3-pip libgtk-3-dev liblapacke-dev libturbojpeg
           checkinstall protobuf-compiler libprotobuf-dev
           libtesseract-dev tesseract-ocr-all libleptonica-dev libopenblas-openmp-dev
           pylint libopenjp2-7-dev libopenjpip7 libopenjp2-tools curl wget libssl-dev git pkg-config
           automake libtool shellcheck libcairo2-dev libpango1.0-dev libglib2.0-dev libgstreamer1.0-0
           gstreamer1.0-plugins-base libusb-1.0-0-dev libopenmpi-dev
           # for openvino:
           clang libclang-dev clang-format ccache libssl-dev ca-certificates git-lfs libgtk2.0-dev pkg-config
           libtool autoconf shellcheck libcairo2-dev libpango1.0-dev libglib2.0-dev
           libgtk2.0-dev libgstreamer1.0-0 gstreamer1.0-plugins-base libusb-1.0-0-dev scons patchelf
           lintian file gzip libpugixml-dev libva-dev python3-pip python3-venv python3-enchant python3-setuptools
           libpython3-dev libgflags-dev zlib1g-dev libudev1 libusb-1.0-0 libusb-1.0-0-dev git-lfs
           nlohmann-json3-dev python3-yaml python3-setuptools
	       python3-wheel cython3 )

# on platform, opencl headers conflict with aml-npu package, which also provides them:
if [ $arch = "amd64" ]; then
    packages+=( ocl-icd-opencl-dev opencl-headers )
fi

####################################################################################################
question "Install/update packages"
if [ "X$REPLY" != "Xn" ]; then
    set +e
    
    # Install packages unless they are already installed:
    failed=""
    for pack in "${packages[@]}"; do
        if `dpkg --list | grep -q $pack`; then
            echo "$pack already installed"
        else
            sudo apt-get --assume-yes install $pack
            if [ $? -ne 0 ]; then failed="$failed $pack"; fi
        fi
    done
    
    if [ "X$failed" != "X" ]; then
        echo
        echo
        echo "### WARNING: Some packages failed to install #####" 
        echo
        echo "Install failed for: $failed"
        echo
        echo
        echo "This will likely be a problem for the rest of this script."
        echo "You should try to install them using synaptic or other."
        read -p "Abort this script now [Y/n]? "
        if [ "X$REPLY" != "Xn" ]; then exit 2; fi
    fi

    set -e
fi

####################################################################################################
# Set some defs needed by openvino and opencv:
if [ "X$arch" != "Xamd64" ]; then
    NEON="-DENABLE_NEON=ON -DWITH_CAROTENE=ON"
    VINOARCH="aarch64" # only available when compiling openvino from source
else
    VINOARCH="intel64"
fi
    
####################################################################################################
# Create the deb dependencies list from the package list:
printf -v pkgdeps '%s,' "${packages[@]}"
pkgdeps="${pkgdeps%,}"

####################################################################################################
# Fix compilation error with libjpeg-turbo8
if [ ! -f ${root}/usr/lib/${libarch}-linux-gnu/libturbojpeg.so ]; then
    sudo ln -s ${root}/usr/lib/${libarch}-linux-gnu/libturbojpeg.so.0.?.0 \
         ${root}/usr/lib/${libarch}-linux-gnu/libturbojpeg.so
fi

####################################################################################################
# Install the desired compiler as default:
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${gccver} 100 \
     --slave /usr/bin/g++ g++ /usr/bin/g++-${gccver} \
     --slave /usr/bin/gfortran gfortran /usr/bin/gfortran-${gccver}
sudo update-alternatives --set gcc /usr/bin/gcc-${gccver}

####################################################################################################
# install openvino
question "Compile and install OpenVino"
if [ "X$REPLY" != "Xn" ]; then

    sudo rm -rf ${root}/usr/share/jevoispro-openvino-${vino}

    # We may re-use already downloaded tree, if present, only when not forcing -y option:
    nuke=1
    if [ -d openvino-${vino} ]; then
        question "Nuke existing OpenVino source tree"
        if [ "X$REPLY" != "Xn" ]; then
            nuke=1
        else
            nuke=0
            sudo rm -rf openvino-${vino}/build
        fi
    fi
    
    if [ "X$nuke" = "X1" ]; then
        sudo rm -rf openvino-${vino} openvino
        sudo rm -rf openvino_contrib-${vino} openvino_contrib
        
        # Get openvino_contrib:
        git clone --recursive --recurse-submodules --branch ${vinobranch} \
            https://github.com/openvinotoolkit/openvino_contrib.git
        mv openvino_contrib openvino_contrib-${vino}
        
        # Get openvino:
        git clone --recursive --recurse-submodules --branch ${vinobranch} \
            https://github.com/openvinotoolkit/openvino.git
        mv openvino openvino-${vino}
    
        cd openvino-${vino}
        
        # Patch it up for compatibility with ubuntu noble:
        if [ ${uburel} = "24.04" ]; then
            sed -i '/#pragma once/a #include <cstdint>' src/common/util/include/openvino/util/file_util.hpp
            sed -i '/#pragma once/a #include <cstdint>' src/common/preprocessing/ie_preprocess_gapi_kernels.hpp
            sed -i '/#pragma once/a #include <cstdint>' src/common/itt/include/openvino/itt.hpp
            
            sed -i '/#include <array>/a #include <cstdint>' \
                ../openvino_contrib-${vino}/modules/arm_plugin/thirdparty/ComputeLibrary/arm_compute/core/utils/misc/Utility.h
            sed -i '/#include <array>/a #include <cstdint>' \
                thirdparty/ade/sources/ade/include/ade/typed_graph.hpp
            sed -i '/#include <array>/a #include <cstdint>' \
                ../openvino_contrib-2022.3.2/modules/arm_plugin/thirdparty/ComputeLibrary/arm_compute/core/Strides.h
            sed -i '/#include <array>/a #include <cstdint>' \
                src/frontends/onnx/onnx_common/src/onnx_model_validator.cpp
            sed -i '/#include <string>/a #include <cstdint>' \
                src/plugins/intel_gpu/thirdparty/onednn_gpu/src/gpu/jit/gemm/gen_gemm_kernel_common.hpp
            sed -i '/#include <string>/a #include <cstdint>' \
                src/plugins/intel_gpu/src/kernel_selector/jitter.h
            sed -i '/#include <string>/a #include <cstdint>' \
                src/plugins/intel_gpu/include/intel_gpu/runtime/device_info.hpp
            
            sed -i '238i using JitConstant::GetDefinitions;' \
                src/plugins/intel_gpu/src/kernel_selector/jitter.cpp
            
            sed -i '264i using primitive_impl::execute;' \
                src/plugins/intel_gpu/src/graph/impls/cpu/proposal.cpp
            
            sed -i '71i using WeightBiasKernelBase::GetJitConstants;' \
                src/plugins/intel_gpu/src/kernel_selector/kernels/deconvolution/deconvolution_kernel_base.h
            
            sed -i 's/return std::move/return /g' \
                src/inference/include/ie/ie_blob.h
            
            sed -i 's/ -1/ 255/g' thirdparty/ocv/opencv_hal_neon.hpp
            
            sed -i 's/device.getInfo<CL_DEVICE_PLATFORM>()/device.getInfo<CL_DEVICE_PLATFORM>()()/g' \
                src/plugins/intel_gpu/src/runtime/ocl/ocl_device_detector.cpp
        fi
    else
        cd openvino-${vino}
    fi        

    # Build process is quite different on host vs platform:
    if [ $arch = "amd64" ]; then
    
        # Install dependencies
        sudo ./install_build_dependencies.sh -y
        ##sudo pip3 install -r src/bindings/python/wheel/requirements-dev.txt
        ##sudo pip3 install -r cmake/developer_package/ncc_naming_style/requirements_dev.txt

        # Ok, build it. Note, on x86_64, we do not compile openvino_contrib anymore because it requires obsoleted nvidia
        # packages that cannot install on ubuntu noble:
        mkdir build && cd build

        cmake -DCMAKE_BUILD_TYPE=Release \
              -DBUILD_java_api=OFF \
              -DENABLE_OPENCV=ON -DENABLE_SAMPLES=OFF -DENABLE_TESTS=OFF -DENABLE_PYTHON=OFF \
              -DCMAKE_INSTALL_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
              -DCPACK_PACKAGING_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
              -DCPACK_SET_DESTDIR=ON \
	          -Wno-dev \
              ..
    
        make -j ${job}
    else
        # Build on arm64:
        sudo apt install -y git cmake scons build-essential
        
        mkdir build && cd build

        cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_SAMPLES=OFF -DENABLE_TESTS=OFF \
              -DOPENVINO_EXTRA_MODULES=../openvino_contrib-${vino}/modules/arm_plugin \
              -DARM_COMPUTE_SCONS_JOBS=${job} \
              -DCMAKE_INSTALL_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
              -DCPACK_PACKAGING_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
              -DCPACK_SET_DESTDIR=ON \
              ..
        
        make -j ${job}
    fi
    
    sudo make install
    
    # Description
    cat > description-pak <<EOF
OpenVino ${vino} configured for use with JeVois-Pro smart machine vision
EOF

    # create post install and remove scripts:
    cat > postinstall-pak <<EOF
#!/bin/sh
f=/etc/ld.so.conf.d/jevoispro-openvino.conf
echo "/usr/share/jevoispro-openvino-${vino}/runtime/lib/${VINOARCH}" > \${f}
ldconfig
EOF
    chmod a+x postinstall-pak

    cat > postremove-pak<<EOF
#!/bin/sh
/bin/rm -f /etc/ld.so.conf.d/jevoispro-openvino.conf
ldconfig
EOF
    chmod a+x postremove-pak

    # Pack it up:
    sudo checkinstall \
         --type debian \
         --pkgname jevoispro-openvino \
         --provides jevoispro-openvino \
         --pkglicense "GPL" \
         --maintainer "Laurent Itti \\<jevois.org@gmail.com\\>" \
         --nodoc \
         --arch "${arch}" \
         --pkgsource="https://github.com/openvinotoolkit/openvino/archive/${vino}.tar.gz" \
         --pkgaltsource="https://github.com/openvinotoolkit/openvino/tree/${vino}" \
         --pkgversion "${vino}" \
         --reset-uids \
         --exclude "/home*" \
         --pkggroup "universe" \
         -y \
         --install=no \
         --pkgrelease "${pkgrel}ubuntu${uburel}" \
         --requires "${pkgdeps}"

    # run the postinstall so that opencv can find the openvino libs when we compile it:
    sudo ./postinstall-pak

    cd ../..
fi

####################################################################################################
# OpenCv installation and packing
ver=${ma}.${mi}.${pa}

if [ -f /usr/lib/libavformat.so.53.4.0 ]; then
    question "Nuke old ffmpeg 53, which is not compatible with opencv"
    if [ "X$REPLY" != "Xn" ]; then
        sudo rm -f /usr/lib/libav* /usr/lib/libswscale*
    fi
fi

question "Compile OpenCv"
if [ "X$REPLY" != "Xn" ]; then

    sudo rm -rf opencv-${ver}
    sudo rm -rf ${root}/usr/share/jevoispro-opencv-${ver}
    mkdir opencv-${ver}
    cd opencv-${ver}
    ocvroot=`pwd`
    
    wget https://github.com/opencv/opencv/archive/${ver}.tar.gz
    tar zxf ${ver}.tar.gz
    rm -f ${ver}.tar.gz
    
    wget https://github.com/opencv/opencv_contrib/archive/${ver}.tar.gz
    tar zxf ${ver}.tar.gz
    rm -f ${ver}.tar.gz
    
    # Run cmake to generate the makefile, install to /usr:
    cd opencv-${ver}
    mkdir build

    # Patch location of OpenVino:
    sed -i -e "s/find_package(OpenVINO QUIET)/find_package(OpenVINO QUIET PATHS \/usr\/share\/jevoispro-openvino-${vino}\/runtime\/cmake)/" cmake/OpenCVDetectInferenceEngine.cmake
    
    cd build

    opts="-DCMAKE_BUILD_TYPE=RELEASE \
    -DCMAKE_INSTALL_PREFIX=${root}/usr/share/jevoispro-opencv-${ver} \
    -DCPACK_PACKAGING_PREFIX=${root}/usr/share/jevoispro-opencv-${ver} \
    -DCPACK_SET_DESTDIR=ON \
    -DOPENCV_VCSVERSION=\"${ver}-${pkgrel}ubuntu${uburel}\" \
    -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python${pyver} \
    -DPYTHON_EXECUTABLE=/usr/bin/python${pyver} \
    -DBUILD_OPENCV_PYTHON3=ON \
    \
    -DENABLE_FAST_MATH=1 \
    -DENABLE_CXX11=ON \
    ${NEON} \
    \
    -DBUILD_opencv_python2=OFF \
    -DBUILD_opencv_python3=ON \
    -DBUILD_NEW_PYTHON_SUPPORT=ON \
    -DBUILD_JAVA=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_PERF_TESTS=OFF \
    \
    -DWITH_EIGEN=ON \
    -DWITH_OPENMP=ON \
    -DWITH_OPENCL_SVM=$JVOPENCL \
    -DWITH_VULKAN=ON \
    -DWITH_OPENVX=ON \
    -DWITH_FFMPEG=ON \
    -DWITH_TBB=ON \
    -DWITH_V4L=ON \
    -DWITH_QT=OFF \
    -DWITH_OPENGL=OFF \
    -DWITH_OPENCL=$JVOPENCL \
    -DWITH_VTK=ON \
    -DWITH_CUDA=OFF \
    \
    -DWITH_TIMVX=ON \
    \
    -DWITH_OPENVINO=ON \
    -DOPENCV_DNN_OPENVINO=ON \
    -DWITH_NGRAPH=ON \
    -DWITH_INF_ENGINE=ON \
    -Dngraph_DIR=/usr/share/jevoispro-openvino-${vino}/runtime/cmake \
    -DInferenceEngine_DIR=/usr/share/jevoispro-openvino-${vino}/runtime/cmake \
    \
    -DBUILD_EXAMPLES=OFF \
    -DINSTALL_C_EXAMPLES=OFF \
    -DINSTALL_PYTHON_EXAMPLES=OFF \
    \
    -DOPENCV_ENABLE_NONFREE=ON \
    -DOPENCV_EXTRA_MODULES_PATH=${ocvroot}/opencv_contrib-${ver}/modules \
    \
    -DENABLE_PRECOMPILED_HEADERS=OFF \
    -DCPACK_BINARY_DEB=ON \
    -DCPACK_BINARY_STGZ=OFF \
    -DCPACK_BINARY_TZ=OFF \
    -DCPACK_BINARY_TGZ=OFF"
    # May want to check this: WITH_PLAIDML, WITH_QT, WITH_OPENGL (likely no, we have opengl-es)

    # Opencv 4.10 fails to find mpi.h, add eplicit path:
    export CXXFLAGS="-I/usr/include/${libarch}-linux-gnu -I/usr/include/${libarch}-linux-gnu/mpi"
    
    cmake ${opts} ..

    # build
    make -j ${job}
    make doc
    
    # Install it for the next steps (compiling jevois software)
    sudo make install
    sudo mkdir -p /usr/share/jevoispro-opencv-${ver}/share/licenses/opencv4
    
    # Description
    cat > description-pak <<EOF
OpenCV ${ver} configured for use with JeVois-Pro smart machine vision
EOF

    # create post install and remove scripts:
    cat > postinstall-pak <<EOF
#!/bin/sh
f=/etc/ld.so.conf.d/jevoispro-opencv.conf
echo "/usr/share/jevoispro-opencv-${ver}/lib" > \${f}

p="/usr/share/jevoispro-opencv-${ver}/lib/python${pyver}/dist-packages/cv2/python-${pyver}"
if [ -d "\${p}" ]; then echo "\${p}" >> \${f}; fi

p="/usr/share/jevoispro-opencv-${ver}/lib/python${pyver}/site-packages/cv2/python-${pyver}"
if [ -d "\${p}" ]; then echo "\${p}" >> \${f}; fi

ldconfig

# Fix compilation error with libjpeg-turbo8
if [ ! -f /usr/lib/${libarch}-linux-gnu/libturbojpeg.so ]; then
    sudo ln -s /usr/lib/${libarch}-linux-gnu/libturbojpeg.so.0.?.0 /usr/lib/${libarch}-linux-gnu/libturbojpeg.so
fi
EOF
    chmod a+x postinstall-pak

    cat > postremove-pak<<EOF
#!/bin/sh
/bin/rm -f /etc/ld.so.conf.d/jevoispro-opencv.conf
ldconfig
EOF
    chmod a+x postremove-pak

    sudo checkinstall \
         --type debian \
         --pkgname jevoispro-opencv \
         --provides jevoispro-opencv \
         --pkglicense "3-clause BSD" \
         --maintainer "Laurent Itti \\<jevois.org@gmail.com\\>" \
         --nodoc \
         --arch "${arch}" \
         --pkgsource="https://github.com/opencv/opencv/archive/${ver}.tar.gz" \
         --pkgaltsource="https://github.com/opencv/opencv/tree/${ver}" \
         --pkgversion "${ver}" \
         --reset-uids \
         --exclude "/home*" \
         --pkggroup "universe" \
         -y \
         --install=no \
         --pkgrelease "${pkgrel}ubuntu${uburel}" \
         --requires "${pkgdeps}, jevoispro-openvino"

    if [ -d ~/jevois/software/jevois/Contrib/npu/include/ ]; then
        question "Update jevois/Contrib/npu with TIM-VX installed here"
        if [ "X$REPLY" != "Xn" ]; then
            /bin/cp -ar 3rdparty/libtim-vx/TIM-VX-*/prebuilt-sdk/x86_64_linux/include/* \
                    ~/jevois/software/jevois/Contrib/npu/include/
            /bin/cp -ar 3rdparty/libtim-vx/TIM-VX-*/prebuilt-sdk/x86_64_linux/lib \
                    ~/jevois/software/jevois/Contrib/npu/x86_64_linux/    
        fi
    fi

    # run the postinstall so that jevois can find opencv:
    sudo ./postinstall-pak
fi
