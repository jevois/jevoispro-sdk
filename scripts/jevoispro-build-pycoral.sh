#!/bin/bash
# Build pycoral from source
# USAGE: jevoispro-build-pycoral.sh [-y]

set -e

# no release since 2021, but a few bugfixes on master branch, so we will just use the master branch

# Architecture and paths:
libarch=`uname -p` # x86_64 or aarch64
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture` # amd64 or arm64

# Helper function to ask a question with possibly using -y default from command line:
if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "JEVOIS: ${1}? [Y/n] "; fi }

####################################################################################################
question "Clone pycoral source from github"
if [ "X$REPLY" != "Xn" -o ! -d pycoral ]; then
    /bin/rm -rf pycoral
    git clone --recurse-submodules --depth 1 https://github.com/google-coral/pycoral

    for f in `find pycoral -name update_sources.sh`; do
        cat > $f <<FULLEOF
#!/bin/sh

cat > /etc/apt/sources.list.d/ubuntu-arm64.sources <<EOF
Types: deb
Architectures: arm64 armhf
URIs: http://ports.ubuntu.com/ubuntu-ports/
Suites: noble noble-updates noble-backports
Components: main restricted universe multiverse
Signed-By: /usr/share/keyrings/ubuntu-archive-keyring.gpg
EOF

sed -i "/Types:/a Architectures: amd64 i386" /etc/apt/sources.list.d/ubuntu.sources
FULLEOF
        chmod a+x $f
    done

    cat > pycoral/libcoral/docker/Dockerfile <<EOF 
ARG IMAGE
FROM \${IMAGE}

COPY update_sources.sh /
RUN /update_sources.sh

RUN dpkg --add-architecture arm64
RUN echo 'APT::Immediate-Configure false;' >> /etc/apt/apt.conf

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
      libc6-dev:arm64 \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
      sudo \
      debhelper \
      python3-all \
      python3-numpy \
      python3-setuptools \
      python3-six \
      python3-wheel \
      libpython3-dev \
      libpython3-dev:arm64 \
      build-essential \
      crossbuild-essential-arm64 \
      libusb-1.0-0-dev \
      libusb-1.0-0-dev:arm64 \
      zlib1g-dev \
      zlib1g-dev:arm64 \
      pkg-config \
      p7zip-full \
      zip \
      unzip \
      curl \
      wget \
      git \
      vim \
      mc \
      software-properties-common python-is-python3 python3-pybind11

# On older Ubuntu these packages can't be installed in a multi-arch fashion.
# Instead we download the debs and extract them for build time linking.
RUN mkdir /debs && chmod a=rwx /debs && cd /debs && apt-get update && apt-get download \
  libglib2.0 \
  libglib2.0:arm64 \
  libglib2.0-dev \
  libglib2.0-dev:arm64 \
  libgstreamer1.0-0 \
  libgstreamer1.0-0:arm64 \
  libgstreamer1.0-dev \
  libgstreamer1.0-dev:arm64 \
  libgstreamer-plugins-base1.0-0 \
  libgstreamer-plugins-base1.0-0:arm64 \
  libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-base1.0-dev:arm64

RUN for d in /debs/*.deb; do dpkg -x \$d /usr/system_libs; done

RUN git clone https://github.com/raspberrypi/tools.git && \
    cd tools && \
    git reset --hard 4a335520900ce55e251ac4f420f52bf0b2ab6b1f

ARG BAZEL_VERSION=4.0.0
RUN wget -O /bazel https://github.com/bazelbuild/bazel/releases/download/\${BAZEL_VERSION}/bazel-\${BAZEL_VERSION}-installer-linux-x86_64.sh && \
    bash /bazel && \
    rm -f /bazel

# Jevois: install old gcc for compat
ARG GCCVER=10
RUN apt-get -y install g++-\${GCCVER}-aarch64-linux-gnu gfortran-\${GCCVER}-aarch64-linux-gnu && \
     update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc \
     /usr/bin/aarch64-linux-gnu-gcc-\${GCCVER} 100 \
     --slave /usr/bin/aarch64-linux-gnu-g++ aarch64-linux-gnu-g++ \
       /usr/bin/aarch64-linux-gnu-g++-\${GCCVER} \
     --slave /usr/bin/aarch64-linux-gnu-gfortran \
       aarch64-linux-gnu-gfortran /usr/bin/aarch64-linux-gnu-gfortran-\${GCCVER} && \
     update-alternatives --set aarch64-linux-gnu-gcc /usr/bin/aarch64-linux-gnu-gcc-\${GCCVER}

RUN apt-get -y install g++-\${GCCVER} gfortran-\${GCCVER} && \
     update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-\${GCCVER} 100 \
     --slave /usr/bin/g++ g++ /usr/bin/g++-\${GCCVER} \
     --slave /usr/bin/gfortran  gfortran /usr/bin/gfortran-\${GCCVER} && \
     update-alternatives --set gcc /usr/bin/gcc-\${GCCVER}
EOF
fi
  
####################################################################################################
cd pycoral

question "Make pycoral wheel"
if [ "X$REPLY" != "Xn" ]; then
    bazel clean --expunge
    sudo /bin/rm -rf ~/.cache/bazel

    sed -i -e 's/3.10/3.12/' setup.py
    sed -i -e 's/36 37 38 39 310/312/g' scripts/build.sh
    sed -i -e 's/310/312/g' scripts/build.sh
    sed -i -e 's/22.04/24.04/' scripts/build.sh
    sed -i -e 's/ armv7a//' scripts/build.sh
    
    cat > jevois-build-wheels.sh <<EOF
#!/bin/bash
make wheel # will fail because of obsolete pybind dep in tensorflow

echo "\n\n\nJEVOIS: no worries, we will fix that error now...\n\n\n"

for f in \`find ~/.cache/bazel -name workspace2.bzl\`; do
  sed -i -e 's/2.6.0/2.12.0/g' \$f
  sed -i -e 's/90b705137b69ee3b5fc655eaca66d0dc9862ea1759226f7ccd3098425ae69571/bf8f242abd1abcd375d516a7067490fb71abd79519a282d22b6e4d19282185a7/' \$f
done

make wheel
make tflite-wheel

CPU=aarch64 make wheel
CPU=aarch64 make tflite-wheel
EOF
chmod a+x jevois-build-wheels.sh

    make DOCKER_SHELL_COMMAND="/workspace/jevois-build-wheels.sh" DOCKER_IMAGE="ubuntu:24.04" docker-shell
    echo "JEVOIS: done. Wheels are in pycoral/dist/"
fi
