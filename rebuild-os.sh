#!/bin/bash

r="${JEVOISPRO_SDK_ROOT}"
if [ "X${r}" = "X" -o ! -d "${r}" ]; then echo "JeVoisPro SDK root [${r}] not found -- ABORT"; exit 1; fi
echo "Using JeVoisPro SDK root: ${r}"

if [ ! -d "${r}/fenix" ]; then echo "Cannot find ${r}/fenix -- ABORT"; exit 2; fi

cd ${r}/fenix

sudo /bin/rm -rf build

source env/setenv.sh -q -s  KHADAS_BOARD=JVPRO LINUX=4.9 UBOOT=2015.01 DISTRIBUTION=Ubuntu DISTRIB_RELEASE=focal DISTRIB_TYPE=xfce DISTRIB_ARCH=arm64 INSTALL_TYPE=SD-USB COMPRESS_IMAGE=no

make clean # otherwise we get some weird error on first make, which disappears when trying make again...

make

cd ..

./scripts/jevoispro-copy-sysroot.sh

echo "ALL DONE: image to flash to SD is: fenix/build/images/JVPRO_Ubuntu-xfce-focal_Linux-4.9_arm64_SD-USB*.img"
