#!/bin/bash

# Download, build, and pack opencv for jevois-pro, native or platform
# USAGE: jevoispro-build-opencv.sh [aarch64|-y]

# Note: on platform, we do not install dependency packages, they should have been pre-installed when building the
# platform OS

if [ "X$1" = "Xaarch64" ]; then
    cross=1
    echo "----- Cross-compiling for $1 -----"
fi

# OpenCV version to build:
ma=4
mi=6
pa=0

vino="2022.1" # release
vinobranch="releases/2022/1" # github branch

# note: for vino 2021.4 use -DINF_ENGINE_RELEASE=${vino//./0}0000 \
# note: for vino 2021.4.2 use -DINF_ENGINE_RELEASE=${vino//./0}00 \
infrel=${vino//./0}0000

# Our deb package release number:
pkgrel=1

# Architecture and paths:
libarch=`uname -p` # x86_64 or aarch64
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture` # amd64 or aarch64
ncpu=`grep -c processor /proc/cpuinfo`
root=""

job=""
if [ $arch != "amd64" ]; then job="2"; fi

if [ "X$cross" = "X1" ]; then
    libarch="aarch64"
    arch="aarch64"
    root="/home/${USER}/jevois/software/jevoispro-sdk/jevoispro-sysroot"
fi

# NOTE: 4.6.0 disable OpenCL on host build as it leads to an undefined symbol in python??
if [ $arch = "amd64" ]; then JVOPENCL=OFF; else JVOPENCL=ON; fi

if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "${1}? [Y/n] "; fi }

####################################################################################################
# Install boost first and then decide on python version:
sudo apt install -y libboost-all-dev
if [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python38.so" ]; then pyver="3.8";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python3-py37.so" ]; then pyver="3.7";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python-py36.so" ]; then pyver="3.6";
elif [ -f "${root}/usr/lib/${libarch}-linux-gnu/libboost_python-py35.so" ]; then pyver="3.5";
else echo "Cannot find libboost_python py35, py36, py37 or 38 -- ABORT"; exit 5; fi

####################################################################################################
# Compiler packages and dependencies
if [ ${uburel} = "18.04" -o ${uburel} = "19.04" ]; then
    gccver=8
    qt4lib="libqt4-dev"
else
    gccver=10
    qt4lib="" # no more qt4 on 20.04; we already have qt5-default in the list below
fi

packages=( build-essential python2.7-dev gcc-${gccver} g++-${gccver} gfortran-${gccver} cmake libboost-all-dev autoconf
           libeigen3-dev libgtk2.0-dev libdc1394-22 libdc1394-22-dev libjpeg-dev libpng-dev libtiff5-dev libavcodec-dev
           libavformat-dev libswscale-dev libxine2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libv4l-dev
           libtbb2 libtbb-dev ${qt4lib} libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev
           libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils unzip qt5-default python${pyver}-dev python3-numpy
           python3-pip libgtk-3-dev libatlas-base-dev libturbojpeg checkinstall protobuf-compiler libprotobuf-dev
           libtesseract-dev tesseract-ocr-all libleptonica-dev liblapacke-dev libopenblas-dev libopenblas-openmp-dev
           libavresample-dev pylint libopenjp2-7-dev libopenjpip7 libopenjp3d7 libopenjp2-tools libopenjp3d-tools
           libcaffe-cpu-dev curl wget libssl-dev ca-certificates git pkg-config automake libtool shellcheck python
           libcairo2-dev libpango1.0-dev libglib2.0-dev libgstreamer1.0-0 gstreamer1.0-plugins-base libusb-1.0-0-dev 
           # for openvino:
           clang-format libssl-dev ca-certificates git-lfs libboost-regex-dev libgtk2.0-dev pkg-config
           unzip automake libtool autoconf shellcheck python libcairo2-dev libpango1.0-dev libglib2.0-dev libgtk2.0-dev
           libgstreamer1.0-0 gstreamer1.0-plugins-base libusb-1.0-0-dev libopenblas-dev scons )

# libopenjpip-dec-server libopenjpip-server libopenjpip-viewer

# openvino uses gcc-multilib g++-multilib but this removes arm g++-10

####################################################################################################
question "Install/update packages"
if [ "X$REPLY" != "Xn" ]; then

    # Install packages if we are compiling natively
    if [ "X$cross" != "X1" ]; then
        
        failed=""
        for pack in "${packages[@]}"; do
            sudo apt-get --assume-yes install $pack
            if [ $? -ne 0 ]; then failed="$failed $pack"; fi
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
    fi
fi

####################################################################################################
# Set some defs needed by openvino and opencv:
if [ "X$arch" != "Xamd64" ]; then
    NEON="-DENABLE_NEON=ON"
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
if [ "X$cross" != "X1" ]; then
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${gccver} 100 \
         --slave /usr/bin/g++ g++ /usr/bin/g++-${gccver} \
         --slave /usr/bin/gfortran gfortran /usr/bin/gfortran-${gccver}
    sudo update-alternatives --set gcc /usr/bin/gcc-${gccver}
fi

####################################################################################################
# install openvino
question "Compile and install OpenVino"
if [ "X$REPLY" != "Xn" ]; then

    sudo rm -rf openvino-${vino} openvino
    sudo rm -rf openvino_contrib-${vino} openvino_contrib
    sudo rm -rf /usr/local/share/ade
    sudo rm -rf ${root}/usr/share/jevoispro-openvino-${vino}

    # Get openvino_contrib:
    git clone --recursive --recurse-submodules --branch ${vinobranch} https://github.com/openvinotoolkit/openvino_contrib.git
    mv openvino_contrib openvino_contrib-${vino}
    
    # Get openvino:
    git clone --recursive --recurse-submodules --branch ${vinobranch} https://github.com/openvinotoolkit/openvino.git
    mv openvino openvino-${vino}

    cd openvino-${vino}

    # Install dependencies
    ./install_build_dependencies.sh -y
    
    # Ok, build it:
    mkdir build && cd build

    cmake -DCMAKE_BUILD_TYPE=Release -DNGRAPH_ONNX_IMPORT_ENABLE=ON \
          -DIE_EXTRA_MODULES=../openvino_contrib-${vino}/modules -DBUILD_java_api=OFF \
          -DENABLE_OPENCV=ON \
          -DCMAKE_INSTALL_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
          -DCPACK_PACKAGING_PREFIX=${root}/usr/share/jevoispro-openvino-${vino} \
          -DCPACK_SET_DESTDIR=ON \
          ..
    
    make -j ${job}

    sudo cmake --install . --prefix /usr/share/jevoispro-openvino-${vino}

    # Description
    cat > description-pak <<EOF
OpenVino ${vino} configured for use with JeVois-Pro smart machine vision
EOF

    # create post install and remove scripts:
    cat > postinstall-pak <<EOF
#!/bin/sh
f=/etc/ld.so.conf.d/jevoispro-openvino.conf
echo "/usr/share/jevoispro-openvino-${vino}/runtime/lib/${VINOARCH}" >> \${f}
echo "/usr/share/jevoispro-openvino-${vino}/runtime/3rdparty/tbb/lib" >> \${f}

ldconfig
EOF
    chmod a+x postinstall-pak

    cat > postremove-pak<<EOF
#!/bin/sh
/bin/rm -f /etc/ld.so.conf.d/jevoispro-openvino.conf
ldconfig
EOF
    chmod a+x postremove-pak

    if [ -d ../inference-engine/temp/tbb_yocto ]; then
        # needed by checkinstall and make install on aarch64
        touch ../inference-engine/temp/tbb_yocto/LICENSE
        #touch ../inference-engine/temp/tbb_yocto/COPYING
        #touch ../inference-engine/temp/tbb_yocto/INSTALL
        #touch ../inference-engine/temp/tbb_yocto/README
    fi

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

    cd ../..

    #curl https://apt.repos.intel.com/openvino/2021/GPG-PUB-KEY-INTEL-OPENVINO-2021 | sudo apt-key add -
    #echo "deb https://apt.repos.intel.com/openvino/2021 all main" | sudo tee /etc/apt/sources.list.d/intel-openvino-2021.list
    #sudo apt update
    #sudo apt install intel-openvino-runtime-ubuntu20-2021.2.200
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
    cd build

    # this messes things up?    
    
    opts="-DCMAKE_BUILD_TYPE=RELEASE \
    -DCMAKE_INSTALL_PREFIX=${root}/usr/share/jevoispro-opencv-${ver} \
    -DCPACK_PACKAGING_PREFIX=${root}/usr/share/jevoispro-opencv-${ver} \
    -DCPACK_SET_DESTDIR=ON \
    -DOPENCV_VCSVERSION=\"${ver}-${pkgrel}ubuntu${uburel}\" \
    -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python${pyver} \
    -DPYTHON_EXECUTABLE=/usr/bin/python${pyver} \
    -DPYTHON2_EXECUTABLE=/usr/bin/python2.7 \
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
    -DWITH_INF_ENGINE=ON \
    -DWITH_NGRAPH=ON \
    -Dngraph_DIR=/usr/share/jevoispro-openvino-${vino}/runtime/cmake \
    -DInferenceEngine_DIR=/usr/share/jevoispro-openvino-${vino}/runtime/cmake \
    -DINF_ENGINE_RELEASE=${infrel} \
    -DINF_ENGINE_VERSION=${vino} \
    -DHAVE_INF_ENGINE=1 \
    -DCMAKE_FIND_ROOT_PATH=/usr/share/jevoispro-openvino-${vino}/ \
    \
    -DBUILD_EXAMPLES=ON \
    -DINSTALL_C_EXAMPLES=ON \
    -DINSTALL_PYTHON_EXAMPLES=ON \
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


    # Use the opencv cross-compile options if cross-compiling:
    if [ "X$cross" = "X1" ]; then
        opts="${opts} \
          -DCMAKE_SYSTEM_NAME=Linux \
          -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
          -DGNU_MACHINE=aarch64-linux-gnu \
          -DCMAKE_SYSROOT=${root} \
          -DFFMPEG_ROOT=${root}/usr \
          -DCMAKE_FIND_ROOT_PATH=${root} \
          -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
          -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
          -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
          -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
          -DCMAKE_INCLUDE_PATH=${root}/usr/include;${root}/usr/lib/aarch64-linux-gnu/glib-2.0/include/ \
          -DCMAKE_LIBRARY_PATH=${root}/usr/lib \
          -DGCC_COMPILER_VERSION=${gccver} \
          -DPYTHON2_INCLUDE_PATH=${root}/usr/include/python2.7 \
          -DPYTHON2_LIBRARIES=${root}/usr/lib/python2.7 \
          -DPYTHON3_INCLUDE_PATH=${root}/usr/include/python${pyver} \
          -DPYTHON3_LIBRARIES=${root}/usr/lib/python${pyver} \
          -DPYTHON3_NUMPY_INCLUDE_PATH=${root}/usr/lib/python3/dist-packages/numpy/core/include \
          -DCMAKE_TOOLCHAIN_FILE=../opencv-${ver}/platforms/linux/aarch64-gnu.toolchain.cmake"

        LD_LIBRARY_PATH="${root}/lib/aarch64-linux-gnu:${LD_LIBRARY_PATH}" \
                       PKG_CONFIG_PATH=${root}/usr/lib/aarch64-linux-gnu/pkgconfig \
                       PKG_CONFIG_LIBDIR=${root}/usr/lib/aarch64-linux-gnu \
                       cmake ${opts} ..
    else
        cmake ${opts} ..
    fi



    # build
    make -j ${job}
    
    # Description
    cat > description-pak <<EOF
OpenCV ${ver} configured for use with JeVois-Pro smart machine vision
EOF

    # create post install and remove scripts:
    cat > postinstall-pak <<EOF
#!/bin/sh
f=/etc/ld.so.conf.d/jevoispro-opencv.conf
echo "/usr/share/jevoispro-opencv-${ver}/lib" > \${f}
echo "/usr/share/jevoispro-opencv-${ver}/lib/python${pyver}/site-packages/cv2/python-${pyver}" >> \${f}

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
         --requires "${pkgdeps}"

    question "Install OpenCv"
    if [ "X$REPLY" != "Xn" ]; then
        # Install it for the next steps (compiling jevois software)
        sudo make install
    fi

    question "Update jevois/Contrib/npu with TIM-VX installed here"
    if [ "X$REPLY" != "Xn" ]; then
        /bin/cp -ar 3rdparty/libtim-vx/TIM-VX-*/prebuilt-sdk/x86_64_linux/include/* \
                ~/jevois/software/jevois/Contrib/npu/include/
        /bin/cp -ar 3rdparty/libtim-vx/TIM-VX-*/prebuilt-sdk/x86_64_linux/lib \
                ~/jevois/software/jevois/Contrib/npu/x86_64_linux/    
    fi
fi

