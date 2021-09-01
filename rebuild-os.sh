#!/bin/bash

r="${JEVOISPRO_SDK_ROOT}"
if [ "X${r}" = "X" -o ! -d "${r}" ]; then echo "JeVoisPro SDK root [${r}] not found -- ABORT"; exit 1; fi
echo "Using JeVoisPro SDK root: ${r}"

if [ ! -d "${r}/fenix" ]; then echo "Cannot find ${r}/fenix -- ABORT"; exit 2; fi

cd ${r}/fenix

sudo /bin/rm -rf build

sudo apt install git dialog lsb-release binutils wget ca-certificates device-tree-compiler pv bc lzop zip \
     binfmt-support build-essential ccache debootstrap ntpdate gawk gcc-arm-linux-gnueabihf gcc-8-arm-linux-gnueabihf \
     qemu-user-static u-boot-tools uuid-dev zlib1g-dev unzip libusb-1.0-0-dev parted pkg-config libncurses5-dev \
     whiptail debian-keyring debian-archive-keyring f2fs-tools libfile-fcntllock-perl rsync libssl-dev \
     nfs-kernel-server btrfs-progs ncurses-term p7zip-full kmod dosfstools libc6-dev-armhf-cross fakeroot curl \
     patchutils python2 python3 liblz4-tool libpython2.7-dev linux-base swig aptly acl systemd-container udev \
     g++-10-arm-linux-gnueabihf lib32stdc++6 libc6-i386 lib32ncurses6 lib32tinfo6 locales ncurses-base zlib1g:i386 \
     pixz bison libbison-dev flex libfl-dev lib32z1 tzdata cpio libglib2.0-dev libtool gettext bsdmainutils \
     bash-completion

source env/setenv.sh -q -s \
       KHADAS_BOARD=JVPRO \
       LINUX=4.9 \
       UBOOT=2015.01 \
       DISTRIBUTION=Ubuntu \
       DISTRIB_RELEASE=focal \
       DISTRIB_TYPE=xfce \
       DISTRIB_ARCH=arm64 \
       INSTALL_TYPE=SD-USB \
       COMPRESS_IMAGE=no

make clean # otherwise we get some weird error on first make, which disappears when trying make again...

make

cd ..

./scripts/jevoispro-copy-sysroot.sh

echo "ALL DONE: image to flash to SD is: fenix/build/images/JVPRO_Ubuntu-xfce-focal_Linux-4.9_arm64_SD-USB*.img"
