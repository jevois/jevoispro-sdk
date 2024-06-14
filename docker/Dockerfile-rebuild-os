# This Docker image is to allow you to compile the operating system for JeVois-Pro from scratch
#
# MOST USERS SHOULD NOT NEED TO BUILD THIS IMAGE, DONWLOAD IT FROM DOCKER HUB INSTEAD (SEE README.md)
#
# Install docker as described here (we chose the apt-based install for Ubuntu):
# https://docs.docker.com/engine/install/ubuntu/
#
# Then build with: cd jevoispro-sdk/docker/ && docker build -t jevois/jevoispro-sdk:1.21.0 -f Dockerfile-rebuild-os .
#
# Once built: docker run -it jevois/jevoispro-sdk:1.21.0

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Update:
RUN apt-get update && apt-get -y install apt-utils && apt-get -y upgrade && apt-get -y dist-upgrade

# Use bash only to avoid env issues:
RUN ln -snf /bin/bash /bin/sh
SHELL ["/bin/bash", "-c"]

# Packages from fenix Dockerfile:
RUN apt-get install -y sudo git dialog lsb-release binutils wget ca-certificates \
  device-tree-compiler pv bc lzop zip binfmt-support build-essential ccache debootstrap ntpdate gawk \
  qemu-user-static u-boot-tools uuid-dev zlib1g-dev unzip libusb-1.0-0-dev parted pkg-config libncurses5-dev \
  whiptail debian-keyring debian-archive-keyring f2fs-tools libfile-fcntllock-perl rsync libssl-dev btrfs-progs \
  ncurses-term p7zip-full kmod dosfstools fakeroot curl patchutils python3 liblz4-tool linux-base swig aptly acl \
  systemd-container udev lib32stdc++6 libc6-i386 lib32ncurses6 lib32tinfo6 locales ncurses-base pixz bison \
  flex libfl-dev lib32z1 tzdata cpio libglib2.0-dev libtool gettext bsdmainutils bash-completion
  
# Base packages:
RUN apt-get install -y git git-lfs dialog lsb-release binutils wget ca-certificates \
    device-tree-compiler pv bc lzop zip binfmt-support ccache debootstrap ntpdate gawk \
    qemu-user-static u-boot-tools uuid-dev zlib1g-dev unzip libusb-1.0-0-dev parted pkg-config libncurses5-dev \
    whiptail debian-keyring debian-archive-keyring f2fs-tools libfile-fcntllock-perl rsync libssl-dev \
    nfs-kernel-server btrfs-progs ncurses-term p7zip-full kmod dosfstools libc6-dev-armhf-cross fakeroot curl \
    patchutils python3 liblz4-tool linux-base swig aptly acl udev \
    lib32stdc++6 libc6-i386 lib32ncurses6 lib32tinfo6 locales ncurses-base \
    pixz bison libbison-dev flex libfl-dev lib32z1 tzdata cpio libglib2.0-dev libtool gettext bsdmainutils \
    bash-completion sudo

# Stuff that is os-version-dependent:
RUN apt-get -y install build-essential gcc-14 g++-14 gfortran-14 python3.12-dev
    
# To compile opencv and openvino:
RUN apt-get -y install \
    cmake libboost-all-dev autoconf libeigen3-dev libgtk2.0-dev libdc1394-dev libjpeg-dev libpng-dev libtiff5-dev \
    libavcodec-dev libavformat-dev libswscale-dev libxine2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    libv4l-dev libtbb-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev \
    libvorbis-dev libxvidcore-dev x264 v4l-utils unzip qtbase5-dev python3-numpy python3-pip libgtk-3-dev \
    liblapacke-dev libturbojpeg checkinstall protobuf-compiler libprotobuf-dev libtesseract-dev tesseract-ocr-all \
    libleptonica-dev libopenblas-openmp-dev pylint libopenjp2-7-dev libopenjpip7 libopenjp2-tools curl wget \
    libssl-dev pkg-config automake libtool shellcheck libcairo2-dev libpango1.0-dev libglib2.0-dev libgstreamer1.0-0 \
    gstreamer1.0-plugins-base libusb-1.0-0-dev libopenmpi-dev clang libclang-dev clang-format ccache libssl-dev \ 
    libgtk2.0-dev pkg-config libtool autoconf shellcheck libcairo2-dev libpango1.0-dev libglib2.0-dev libgtk2.0-dev \
    libgstreamer1.0-0 gstreamer1.0-plugins-base libusb-1.0-0-dev scons patchelf lintian file gzip libpugixml-dev \
    libva-dev python3-pip python3-venv python3-enchant python3-setuptools libpython3-dev libgflags-dev zlib1g-dev \
    libudev1 libusb-1.0-0 libusb-1.0-0-dev nlohmann-json3-dev python3-yaml python3-setuptools python3-wheel \
    cython3

# To compile JeVois:
RUN apt-get -y install \
    libeigen3-dev libgtk2.0-dev libjpeg-dev libpng-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev \
    libxine2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libv4l-dev libtbb-dev \
    libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev \
    libxvidcore-dev x264 v4l-utils unzip python3-numpy python3-pip libgtk-3-dev \
    libturbojpeg checkinstall protobuf-compiler libprotobuf-dev libprotoc-dev libtesseract-dev tesseract-ocr-all \
    libleptonica-dev checkinstall mercurial doxygen-gui graphviz libboost-all-dev \
    libjpeg-turbo8-dev autoconf libeigen3-dev screen gawk u-boot-tools libgtk2.0-dev pkg-config \
    libjpeg-dev libpng-dev libtiff5-dev libavcodec-dev libavformat-dev libswscale-dev libxine2-dev \
    libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libv4l-dev libfaac-dev \
    libmp3lame-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 \
    v4l-utils unzip python3-dev libgtk-3-dev libturbojpeg \
    libgles2-mesa-dev ncftp flex libturbojpeg-dev u-boot-tools dpkg-dev flex curl wput libdmtx-dev \
    subversion python3-scipy emacs tree \
    libflac-dev libogg-dev libvorbis-dev libopenal-dev libjpeg8-dev libfreetype6-dev libudev-dev libdrm-dev libgbm-dev \
    libglm-dev libusb-dev libsdl2-dev sysstat strace

# Install jevois.usc.edu apt repo:
RUN gpg --keyserver keyserver.ubuntu.com --recv-keys DD24C027 && \
    gpg --export DD24C027 > /etc/apt/trusted.gpg.d/jevois.gpg
RUN echo "deb https://jevois.usc.edu/devapt noble main" > /etc/apt/sources.list.d/jevois.list

# Update:
RUN apt-get update && apt-get -y upgrade && apt-get -y dist-upgrade

# Clean up:
RUN apt-get -y autoremove

# Generate a US locale:
RUN locale-gen en_US.UTF-8
ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8' TERM=screen

# Instructions to user:
RUN echo 'echo "Welcome to JeVoisPro-SDK build container\n"' > /etc/profile.d/welcome.sh
RUN echo 'echo "To rebuild everything, just run: ./rebuild-os.sh -deb -zip"' >> /etc/profile.d/welcome.sh
RUN echo 'echo "The resulting .deb and .zip packages will be in the debs/ directory.\n"' >> /etc/profile.d/welcome.sh

# Switch to normal user:
RUN useradd -c 'jevois' -m -d /home/jevois -s /bin/bash jevois
RUN sed -i -e '/\%sudo/ c \%sudo ALL=(ALL) NOPASSWD: ALL' /etc/sudoers
RUN usermod -a -G sudo jevois
USER jevois
WORKDIR /home/jevois

# Enable terminal colors:
run echo 'TERM=xterm-256color' >> .bashrc

ENTRYPOINT ["/bin/bash", "-l"]
