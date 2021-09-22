#!/bin/bash

# USAGE: rebuild-os.sh [-deb]
#
# Will rebuild the JeVois-Pro operating system and create a bootable microSD image as well as a sanitized
# copy of the operating system to be used for cross-compilation of JeVois software.
#
# Optional -deb argument also create a deb package that contains all the results.

# These definitions are used to create the deb package only:
ver="1.16.0"
gitbranch="master"
pkgrel="1"
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture`
pkgversion="${ver}-${pkgrel}ubuntu${uburel}"

# Check location:
r="${JEVOISPRO_SDK_ROOT}"
if [ "X${r}" = "X" ]; then r="/usr/share/jevoispro-sdk"; fi
echo "Using JeVoisPro SDK root: ${r}"

if [ ! -d "fenix" ]; then echo "Cannot find fenix/ -- ABORT"; exit 2; fi
basedir=`pwd`
sudo /bin/rm -rf fenix/build
cd fenix

# install some baseline deps:
packages=( git dialog lsb-release binutils wget ca-certificates device-tree-compiler pv bc lzop zip \
     binfmt-support build-essential ccache debootstrap ntpdate gawk gcc-arm-linux-gnueabihf gcc-8-arm-linux-gnueabihf \
     qemu-user-static u-boot-tools uuid-dev zlib1g-dev unzip libusb-1.0-0-dev parted pkg-config libncurses5-dev \
     whiptail debian-keyring debian-archive-keyring f2fs-tools libfile-fcntllock-perl rsync libssl-dev \
     nfs-kernel-server btrfs-progs ncurses-term p7zip-full kmod dosfstools libc6-dev-armhf-cross fakeroot curl \
     patchutils python2 python3 liblz4-tool libpython2.7-dev linux-base swig aptly acl systemd-container udev \
     g++-10-arm-linux-gnueabihf lib32stdc++6 libc6-i386 lib32ncurses6 lib32tinfo6 locales ncurses-base zlib1g:i386 \
     pixz bison libbison-dev flex libfl-dev lib32z1 tzdata cpio libglib2.0-dev libtool gettext bsdmainutils \
     bash-completion )

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
    question "Are you sure you want to continue"
    if [ "X$REPLY" = "Xn" ]; then exit 2; fi
fi

# Create the dependencies from the packages:
printf -v pkgdeps '%s,' "${packages[@]}"
pkgdeps="${pkgdeps%,}"

# Build the bootloader, kernel, Ubuntu:
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

# Make a copy of the system tree with all symlinks sanitized, will be used to cross-compile JeVois code:
cd ..
./scripts/jevoispro-copy-sysroot.sh

# Write the version of this SDK, when compiling JeVois we will check it:
echo $ver | sudo tee "${r}/jevoispro-sdk-version.txt"

# Optionally create a deb:
if [ "X$1" = "X-deb" ]; then
    sudo rm -rf deb
    sudo mkdir deb
    sudo cp -ar /usr/share/jevoispro-sdk deb/

    sudo mkdir deb/DEBIAN

    cat >/tmp/control <<EOF
Package: jevoispro-sdk-dev
Architecture: ${arch}
Maintainer: Laurent Itti
Priority: optional
Section: universe
Version: ${pkgversion}
Depends: ${pkgdeps}
Description: JeVois-Pro Smart Machine Vision, Software Development Kit development system
 Development utilities and libraries needed to cross-compile JeVois-Pro smart machine vision software
 .
 Provides libraries compiled for the ARM processor inside JeVois-Pro, embedded Linux kernel bootloaders,
 operating system, and utilities to allow building machine vision modules for the JeVois-Pro hardware platform.
EOF

    sudo mv /tmp/control deb/DEBIAN/
    echo "==== control file is:"
    cat deb/DEBIAN/control
    echo "==== control file end"
    
    sudo dpkg-deb --build deb jevoispro-sdk-dev_${pkgversion}_${arch}.deb
    sudo rm -rf deb
    echo "SDK DEBIAN PACKAGE IS: jevoispro-sdk-dev_${pkgversion}_${arch}.deb"
fi

img=`/bin/ls fenix/build/images/JVPRO_Ubuntu-xfce-focal_Linux-4.9_arm64_SD-USB*.img`
imgname=`basename $img`
sudo mv $img .
echo "ALL DONE: IMAGE TO FLASH TO SD: $imgname"
