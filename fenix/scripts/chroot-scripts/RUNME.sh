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

# Clean ssh keys
rm -f /etc/ssh/ssh_host*

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

# We do not want unattended upgrades as those could likely break our software:
apt -y purge unattended-upgrades

# Set default imgui window locations
cat <<-EOF > /root/imgui.ini
[Window][JeVois-Pro v1.21.0]
Pos=920,358
Size=941,639
Collapsed=0

[Window][pipeline:NPU:Detect:yolov7-tiny-NPU]
Pos=24,159
Size=464,877
Collapsed=0
EOF

# Install mali gpu drivers and others from jevois.usc.edu
apt -y install linux-gpu-mali-fbdev aml-npu libamvenc libmultienc wiringpi python3-wiringpi \
    glmark2-es2-fbdev mali-examples-aml tengine-libs libplayer-aml libcec \
    encoder-libs-aml

# Need to update protobuf if we want to run the latest mediapipe:
#pbver="21.2"
#wget https://github.com/protocolbuffers/protobuf/releases/download/v${pbver}/protoc-${pbver}-linux-aarch_64.zip
#unzip protoc-${pbver}-linux-aarch_64.zip -d /usr/local
#/bin/rm protoc-${pbver}-linux-aarch_64.zip

if false; then
#mp="mediapipe-0.8-cp38-none-linux_aarch64.whl" # version that shipped with jevois < 1.18
######## JEVOIS NOBLE: need a cp312 version...
mp=mediapipe-0.8-cp38-cp38-linux_aarch64.whl # works with jevois 1.18, objectron segfaults, no selfie
#mp="mediapipe-0.8.10.2-cp38-cp38-linux_aarch64.whl" # works but twice slower...
wget http://jevois.org/pkg/$mp
pipx install pip --upgrade
pipx install numpy --upgrade
pipx install testresources
pipx install setuptools --upgrade
pipx install cpython
pipx install pillow
pipx install dataclasses
pipx install protobuf==3.19.4
pipx install $mp
rm $mp
fi

# openai whisper speech to text
#pipx install -U openai-whisper

# install onnxruntime which may be useful for CPU deep nets:
pipx install onnxruntime

# Disable hostapd as it pollutes our logs:
systemctl disable hostapd

# Pre-install Hailo drivers (Note: hailort deb is already installed from jevois.usc.edu):
hailo="4.17.0"

# JEVOIS: NEED TO WAIT UNTIL CP312 available...
#hrtwheel="hailort-${hailo}-cp38-cp38-linux_aarch64.whl"
#wget http://jevois.org/pkg/${hrtwheel}
#pipx install ${hrtwheel}
#/bin/rm ${hrtwheel}
#pipx install numpy --upgrade # hailo just downgraded numpy, try to upgrade it back...

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

# Boot into jevois GUI by default:
if [ -f /usr/bin/jevoispro.sh ]; then
   systemctl set-default jevoispro.target
fi

# Update everything
pipx upgrade-all
apt update
apt -y upgrade # upgrade NPU and GPU packages from jevois repo
apt -y install --reinstall ca-certificates
ssh-keygen -A # for problems with openssh-server not starting...

# Install unsnapped firefox
ff='firefox_126.0+build2-0ubuntu0.24.04.1~mt1_arm64.deb'
wget "http://jevois.org/pkg/${ff}"
dpkg -i "${ff}"
rm "${ff}"

# Clean up
apt -y clean
apt -y autoclean
history -c

/bin/rm -rf /jevois # not sure how that one got created in the first place...

# Because /jevoispro will be a VFAT partition, symlinks are not allowed in there. Yet, at some point, ldconfig
# (which we do not explicitly run, but likely it is in a postinst of some package) creates one for libonnxruntime.so:
find /jevoispro -type l -delete

# Self-deleting
rm $0
