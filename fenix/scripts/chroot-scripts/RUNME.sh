#!/bin/sh

#set -e -o pipefail

export LC_ALL=C
export LANG=C

# Default user name, password and hostname
USERNAME=jevois
USER_PASSWORD=jevois
ROOT_PASSWORD=jevois
HOSTNAME=JeVoisPro

# Setup password for root user
echo root:$ROOT_PASSWORD | chpasswd

# Admin user
USER_PASSWORD_ENCRYPTED=`perl -e 'printf("%s\n", crypt($ARGV[0], "password"))' "$USER_PASSWORD"`
useradd -m -p "$USER_PASSWORD_ENCRYPTED" -s /bin/bash $USERNAME
usermod -aG sudo,adm $USERNAME

# Add group
DEFGROUPS="audio,video,disk,input,tty,root,users,games,dialout,cdrom,dip,plugdev,bluetooth,pulse-access,systemd-journal,netdev,staff,i2c"
IFS=','
for group in $DEFGROUPS; do
	/bin/egrep  -i "^$group" /etc/group > /dev/null
	if [ $? -eq 0 ]; then
		echo "Group '$group' exists in /etc/group"
	else
		echo "Group '$group' does not exists in /etc/group, creating"
		groupadd $group
	fi
done
unset IFS

echo "Add $USERNAME to ($DEFGROUPS) groups."
usermod -a -G $DEFGROUPS $USERNAME

# Setup host
echo $HOSTNAME > /etc/hostname
# set hostname in hosts file
cat <<-EOF > /etc/hosts
127.0.0.1   localhost $HOSTNAME
::1         localhost $HOSTNAME ip6-localhost ip6-loopback
fe00::0     ip6-localnet
ff00::0     ip6-mcastprefix
ff02::1     ip6-allnodes
ff02::2     ip6-allrouters
EOF

cd /

# Build time
LC_ALL="C" date > /etc/fenix-build-time
sync

####################################################################################################
# JeVois final fix:
####################################################################################################

# Set default imgui window locations
cat <<-EOF > /root/imgui.ini
[Window][JeVois-Pro v1.19.0]
Pos=920,358
Size=941,639
Collapsed=0

[Window][pipeline:NPU:Detect:yolov7-tiny-NPU]
Pos=24,159
Size=464,877
Collapsed=0
EOF

# Need to update protobuf if we want to run the latest mediapipe:
#pbver="21.2"
#wget https://github.com/protocolbuffers/protobuf/releases/download/v${pbver}/protoc-${pbver}-linux-aarch_64.zip
#unzip protoc-${pbver}-linux-aarch_64.zip -d /usr/local
#/bin/rm protoc-${pbver}-linux-aarch_64.zip

# may be needed by mediapipe? or maybe it will find the jevois version already...
#ocvc="opencv_contrib_python-4.5.2.52-cp38-cp38-linux_aarch64.whl"
#wget http://jevois.org/pkg/$ocvc
#sudo pip3 install $ocvc
#rm $ocvc

#mp="mediapipe-0.8-cp38-none-linux_aarch64.whl" # version that shipped with jevois < 1.18
mp=mediapipe-0.8-cp38-cp38-linux_aarch64.whl # works with jevois 1.18, objectron segfaults, no selfie
#mp="mediapipe-0.8.10.2-cp38-cp38-linux_aarch64.whl" # works but twice slower...
wget http://jevois.org/pkg/$mp
sudo pip3 install pip --upgrade
sudo pip3 install numpy --upgrade
sudo pip3 install testresources
sudo pip3 install setuptools --upgrade
sudo pip3 install cpython
sudo pip3 install pillow
sudo pip3 install dataclasses
sudo pip3 install protobuf==3.19.4
sudo pip3 install $mp
rm $mp

# install onnxruntime which may be useful for CPU deep nets:
sudo pip3 install onnxruntime

# Disable hostapd as it pollutes our logs:
systemctl disable hostapd

# Pre-install Hailo drivers (Note: hailort deb is already installed from jevois.usc.edu):
hailo="4.13.0"
hrtwheel="hailort-${hailo}-cp38-cp38-linux_aarch64.whl"
wget http://jevois.org/pkg/${hrtwheel}
sudo pip3 install ${hrtwheel}
/bin/rm ${hrtwheel}
sudo pip3 install numpy --upgrade # hailo just downgraded numpy, try to upgrade it back...

# Kernel to use for dkms and Hailo PCIe driver:
export KVER="4.9.241"
export ARCH="arm64"

# Hailo makefile runs "depmod -a" and "modprobe" but that uses the host kernel...
mv /usr/sbin/depmod /usr/sbin/depmod.orig
mv /usr/sbin/modprobe /usr/sbin/modprobe.orig
cp /usr/bin/true /usr/sbin/depmod
cp /usr/bin/true /usr/sbin/modprobe

hpcie="hailort-pcie-driver_${hailo}_all.deb"
wget http://jevois.org/pkg/${hpcie}
export kernelver="${KVER}"
export KERNEL_DIR="/lib/modules/${KVER}/build"
export Q=""
yes N | dpkg -i ${hpcie}
/bin/rm ${hpcie}

/bin/rm /usr/sbin/depmod
/bin/rm /usr/sbin/modprobe
mv /usr/sbin/depmod.orig /usr/sbin/depmod
mv /usr/sbin/modprobe.orig /usr/sbin/modprobe

# now run depmod manually on our platform kernel:
depmod -a ${KVER}

# quiet down some hailo messages:
echo "options hailo_pci force_desc_page_size=256" >> /share/opt/hailo/linux/pcie/hailo_pci.conf

# Pre-install some wifi drivers:
wget http://jevois.org/pkg/rtl8812au-jevois.tbz
tar jxf rtl8812au-jevois.tbz
/bin/rm rtl8812au-jevois.tbz
ARCH=arm64 dkms add -k ${KVER} -m 8812au -v 4.2.3
ARCH=arm64 dkms build -k ${KVER} -m 8812au -v 4.2.3
ARCH=arm64 dkms install -k ${KVER} -m 8812au -v 4.2.3

# tim-vx and galcore NPU driver (see https://github.com/opencv/opencv/wiki/TIM-VX-Backend-For-Running-OpenCV-On-NPU):
#mkdir /opt
#cd /opt
#wget https://github.com/VeriSilicon/TIM-VX/releases/download/v1.1.34.fix/aarch64_A311D_6.4.8.tgz
#tar xvf aarch64_A311D_6.4.8.tgz
#echo "export VIVANTE_SDK_DIR=/opt/aarch64_A311D_6.4.8" >> /root/.bashrc
#/bin/cp /opt/aarch64_A311D_6.4.8/lib/galcore.ko /usr/lib/modules/4.9.241/kernel/drivers/amlogic/npu/galcore.ko
#/bin/cp -a /opt/aarch64_A311D_6.4.8/lib/* /usr/lib/
#/bin/rm /usr/lib/galcore.ko

# Boot into jevois GUI by default:
systemctl set-default jevoispro.target

# Clean up
apt-get update
apt-get -y upgrade # upgrade NPU and GPU packages from jevois repo
apt -y install --reinstall ca-certificates
apt-get -y clean
apt-get -y autoclean
history -c

/bin/rm -rf /jevois # not sure how that one got created in the first place...

# Self-deleting
rm $0
