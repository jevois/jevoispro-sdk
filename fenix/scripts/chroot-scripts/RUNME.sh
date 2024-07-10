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

# Install stuff needed for venv:
apt-get -y install python3.12-venv

# Install mali gpu drivers and others from jevois.usc.edu
apt-get -y install linux-gpu-mali-fbdev aml-npu libamvenc libmultienc wiringpi python3-wiringpi \
    glmark2-es2-fbdev mali-examples-aml tengine-libs libplayer-aml libcec

# Setup a virtual environment as we cannot just globally use pip in noble anymore:
python3 -m venv /root/jvenv
source /root/jvenv/bin/activate
pip install pip --upgrade

# Activate our virtual env on any bash:
/usr/bin/bash -c true
cat >>/root/.bashrc <<EOF

# JeVois: activate virtual env:
if [ "X\$VIRTUAL_ENV" = "X" -a -d /root/jvenv ]; then
  source /root/jvenv/bin/activate
fi
EOF

# install onnxruntime which may be useful for CPU deep nets:
pip install onnxruntime

# openai whisper speech to text
#pip install -U openai-whisper

# Apriltag for python: the default pip install does not compile properly for aarch64, get ours, which was obtained by
# running 'pip install apriltag' on a running jevois-pro (the resuling wheel is in ~/.cache/pip/...):
pip install http://jevois.org/pkg/apriltag-0.0.16-cp312-cp312-linux_aarch64.whl

# transformers:
pip install -U transformers

# misc python packages:
pip install psutil

# numpy 1.x needed as 2.x has init problems with pycoral:
npver="1.26.4"
pip install numpy==${npver}

# mediapipe compiled for jevois:
pip install http://jevois.org/pkg/mediapipe-0.10.14-cp312-cp312-linux_aarch64.whl

# tflite and pycoral compiled by jevois for python 3.12:
pip install http://jevois.org/pkg/tflite_runtime-2.5.0.post1-cp312-cp312-linux_aarch64.whl
pip install http://jevois.org/pkg/pycoral-2.0.0-cp312-cp312-linux_aarch64.whl

# Disable hostapd as it pollutes our logs:
systemctl disable hostapd

# Pre-install Hailo drivers (Note: hailort deb is already installed from jevois.usc.edu):
hailo="4.17.0"
# JEVOIS: NEED TO WAIT UNTIL CP312 available...
# pip install http://jevois.org/pkg/hailort-${hailo}-cp312-cp312-linux_aarch64.whl

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

# Get ollama and a couple models; we need to patch the install script to use aarch64 instead of autodetect:
wget https://ollama.com/install.sh
sed -i -e 'x;/./{x;b};x;/ARCH=/h;//a ARCH=arm64' install.sh # force arm64
sed -i -e "x;/./{x;b};x;/KERN=/h;//a KERN=${KVER}" install.sh # pass correct kernel version
sed -i -e 'x;/./{x;b};x;/check_gpu/h;//a return 1' install.sh # disable check_gpu() function
chmod a+x install.sh
./install.sh
/bin/rm install.sh

usermod -a -G ollama jevois
systemctl enable ollama

pip install ollama

# this does not seem to download to the correct location... will have to run it manually on a running camera:
#ollama pull moondream
#ollama pull tinydolphin

# Boot into jevois GUI by default:
if [ -f /usr/bin/jevoispro.sh ]; then
   systemctl set-default jevoispro.target
fi

# Update everything
apt-get update
apt-get -y upgrade # upgrade NPU and GPU packages from jevois repo
apt-get -y install --reinstall ca-certificates
ssh-keygen -A # for problems with openssh-server not starting...
pip install numpy==${npver} # hailo may have downgraded numpy...

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
