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
apt-get -y purge unattended-upgrades

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

# Install stuff needed for venv, pipx
apt-get -y install python3.12-venv pipx

# Install mali gpu drivers and others from jevois.usc.edu
apt-get -y install linux-gpu-mali-fbdev aml-npu libamvenc libmultienc wiringpi python3-wiringpi \
    glmark2-es2-fbdev mali-examples-aml tengine-libs libplayer-aml libcec

# Need to update protobuf if we want to run the latest mediapipe:
#pbver="26.1"
#wget https://github.com/protocolbuffers/protobuf/releases/download/v${pbver}/protoc-${pbver}-linux-aarch_64.zip
#unzip protoc-${pbver}-linux-aarch_64.zip -d /usr/local
#/bin/rm protoc-${pbver}-linux-aarch_64.zip

# install onnxruntime which may be useful for CPU deep nets:
pipx install onnxruntime

# openai whisper speech to text
#pipx install -U openai-whisper

# Setup a virtual environment as we cannot just globally use pip in noble anymore:
python3 -m venv /root/jvenv
source /root/jvenv/bin/activate
pip install pip --upgrade

# Activate our virtual env on any bash:
cat >>/root/.bashrc <<EOF

# JeVois: activate virtual env:
if [ "X$VIRTUAL_ENV" = "X" -a -d /root/jvenv ]; then
  source /root/jvenv/bin/activate
fi
EOF

# Apriltag for python:
pip install apriltag

# transformers:
pip install -U transformers

#mp="mediapipe-0.8-cp38-none-linux_aarch64.whl" # version that shipped with jevois < 1.18
#mp="mediapipe-0.8-cp38-cp38-linux_aarch64.whl" # works with jevois 1.18, objectron segfaults, no selfie, fast
#mp="mediapipe-0.8.10.2-cp38-cp38-linux_aarch64.whl" # works but twice slower...
mp="mediapipe-0.10.14-cp312-cp312-linux_aarch64.whl" # jevois 1.21.0 on noble, fast, objectron, selfie ok
wget http://jevois.org/pkg/$mp
pip install numpy --upgrade
pip install setuptools --upgrade
pip install absl-py testresources 'attrs>=19.1.0' 'flatbuffers>=2.0' jax jaxlib matplotlib \
    opencv-contrib-python 'protobuf>=4.25.3,<5' 'sounddevice>=0.4.4' cpython pillow dataclasses CFFI \
    ml-dtypes opt-einsum scipy contourpy cycler fonttools kiwisolver packaging pillow pyparsing python-dateutil \
    pycparser six
pip install $mp
rm $mp

# Disable hostapd as it pollutes our logs:
systemctl disable hostapd

# Pre-install Hailo drivers (Note: hailort deb is already installed from jevois.usc.edu):
hailo="4.17.0"

# JEVOIS: NEED TO WAIT UNTIL CP312 available...
#hrtwheel="hailort-${hailo}-cp38-cp38-linux_aarch64.whl"
#wget http://jevois.org/pkg/${hrtwheel}
#pip install ${hrtwheel}
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
apt-get update
apt-get -y upgrade # upgrade NPU and GPU packages from jevois repo
apt-get -y install --reinstall ca-certificates
ssh-keygen -A # for problems with openssh-server not starting...

# Clean up
apt-get -y clean
apt-get -y autoclean
history -c

/bin/rm -rf /jevois # not sure how that one got created in the first place...

# Because /jevoispro will be a VFAT partition, symlinks are not allowed in there. Yet, at some point, ldconfig
# (which we do not explicitly run, but likely it is in a postinst of some package) creates one for libonnxruntime.so:
find /jevoispro -type l -delete

# Self-deleting
rm $0
