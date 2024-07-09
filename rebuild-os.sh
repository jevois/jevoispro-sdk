#!/bin/bash

# USAGE: rebuild-os.sh [-deb|-shell [-zip]]
#
# Will rebuild the JeVois-Pro operating system and create a bootable microSD image as well as a sanitized
# copy of the operating system to be used for cross-compilation of JeVois software.
#
# Optional -deb argument also create a deb package that contains all the results.
# Optional -shell argument just runs a bash shell, useful to debug running rebuild-os.sh in docker
# Optional -zip argument also computes the md5sum of the image and zips the image+md5 for flashing using Balena Etcher

if [ "X$1" = "X-shell" ]; then
    echo "Dropping into a shell. Run ./rebuild-os.sh -deb -zip";
    /bin/bash
    echo "Exiting rebuild-os.sh..."
    exit 0
fi

ver="1.21.0"
gitbranch="master"
pkgrel="1"
uburel=`lsb_release -rs`
arch=`dpkg --print-architecture`
pkgversion="${ver}-${pkgrel}ubuntu${uburel}"

# Delete any old images generated by this script:
sudo /bin/rm -f JVPRO_Ubuntu-xfce-*_Linux-4.9_arm64_SD-USB_V*-develop.img
if [ ! -d "fenix" ]; then echo "Cannot find fenix/ -- ABORT"; exit 2; fi
basedir=`pwd`
echo "Cleaning old build... (will take a while)"
sudo /bin/rm -rf fenix/build
cd fenix

# install some baseline deps:
packages=( git dialog lsb-release binutils wget ca-certificates device-tree-compiler pv bc lzop zip \
     binfmt-support build-essential ccache debootstrap ntpdate gawk \
     qemu-user-static u-boot-tools uuid-dev zlib1g-dev unzip libusb-1.0-0-dev parted pkg-config libncurses5-dev \
     whiptail debian-keyring debian-archive-keyring f2fs-tools libfile-fcntllock-perl rsync libssl-dev \
     nfs-kernel-server btrfs-progs ncurses-term p7zip-full kmod dosfstools libc6-dev-armhf-cross fakeroot curl \
     patchutils python3 liblz4-tool linux-base swig aptly acl systemd-container udev \
     lib32stdc++6 libc6-i386 lib32ncurses6 lib32tinfo6 locales ncurses-base \
     pixz bison libbison-dev flex libfl-dev lib32z1 tzdata cpio libglib2.0-dev libtool gettext bsdmainutils \
     bash-completion coreutils )

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
    read -p "Are you sure you want to continue [Y/n] "
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
       DISTRIB_RELEASE=noble \
       DISTRIB_TYPE=xfce \
       DISTRIB_ARCH=arm64 \
       INSTALL_TYPE=SD-USB \
       COMPRESS_IMAGE=no

make clean # otherwise we get some weird error on first make, which disappears when trying make again...
make

# Make a copy of the system tree with all symlinks sanitized, will be used to cross-compile JeVois code:
cd ..
sudo ./scripts/jevoispro-copy-sysroot.sh

# cleanup a bit:
sudo /bin/rm -rf /usr/share/jevoispro-sdk/jevoispro-sysroot/jevoispro
sudo /bin/rm -rf /usr/share/jevoispro-sdk/jevoispro-sysroot/usr/doc/*
sudo /bin/rm -rf /usr/share/jevoispro-sdk/jevoispro-sysroot/lab
sudo /bin/rm -rf /usr/share/jevoispro-sdk/jevoispro-sysroot/jevois

# Write the version of this SDK, when compiling JeVois we will check it:
echo $ver | sudo tee "/usr/share/jevoispro-sdk/jevoispro-sdk-version.txt"

# Optionally create a deb:
if [ "X$1" = "X-deb" ]; then
    echo "##### Creating JeVois-Pro SDK deb package #####"
    sudo /bin/rm -rf deb
    sudo /usr/bin/mkdir -p deb/usr/share
    sudo /bin/cp -ar /usr/share/jevoispro-sdk deb/usr/share/

    # Create the deb:
    sudo mkdir deb/DEBIAN

    cat >/tmp/control <<EOF
Package: jevoispro-sdk-dev
Architecture: ${arch}
Maintainer: Laurent Itti
Priority: optional
Section: universe
Version: ${pkgversion}
Depends: ${pkgdeps}, jevoispro-host (>= $ver), jevoispro-platform (>= $ver), jevoisprobase-host (>= $ver), jevoisprobase-platform (>= $ver)
Description: JeVois-Pro Smart Machine Vision, Software Development Kit development system
 Development utilities and libraries needed to cross-compile JeVois-Pro smart machine vision software
 .
 Provides libraries compiled for the ARM processor inside JeVois-Pro, embedded Linux kernel bootloaders,
 operating system, and utilities to allow building machine vision modules for the JeVois-Pro hardware platform.
EOF

    sudo mv /tmp/control deb/DEBIAN/
    
    sudo dpkg-deb --build deb jevoispro-sdk-dev_${pkgversion}_${arch}.deb
    sudo /bin/rm -rf deb
    echo "SDK DEBIAN PACKAGE IS: jevoispro-sdk-dev_${pkgversion}_${arch}.deb"
fi

img=`/bin/ls fenix/build/images/JVPRO_Ubuntu-xfce-*_Linux-4.9_arm64_SD-USB*.img`
imgname=`basename $img`
sudo mv $img .

echo "DONE: IMAGE TO FLASH TO SD: $imgname"

# Optionally create a zip:
if [ "X$2" = "X-zip" ]; then
    echo "##### Creating JeVois-Pro SDK zip image package #####"
    stem="jevoispro-${ver}-62G"
    /bin/rm "${stem}.zip" "${stem}.img" "${stem}.md5"
    /bin/mv "$imgname" "${stem}.img"
    sha256sum "${stem}.img" > "${stem}.sha256"
    zip -9 "${stem}.zip" "${stem}.img" "${stem}.sha256"
    /bin/rm "${stem}.img" "${stem}.sha256"
fi

echo "ALL DONE: IMAGE TO FLASH TO SD: ${stem}.img"
echo "ALL DONE: DISTRO ZIP: ${stem}.zip"
