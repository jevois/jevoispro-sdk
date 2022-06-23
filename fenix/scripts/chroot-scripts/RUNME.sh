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

# JeVois final fix:
wget http://jevois.org/pkg/mediapipe-0.8-cp38-none-linux_aarch64.whl
sudo pip3 install pip --upgrade
sudo pip3 install numpy --upgrade
sudo pip3 install dataclasses
sudo pip3 install mediapipe-0.8-cp38-none-linux_aarch64.whl
rm mediapipe-0.8-cp38-none-linux_aarch64.whl

wget http://jevois.org/pkg/rtl8812au-jevois.tbz
tar jxf rtl8812au-jevois.tbz
/bin/rm rtl8812au-jevois.tbz

systemctl disable hostapd

# tim-vx and galcore NPU driver (see https://github.com/opencv/opencv/wiki/TIM-VX-Backend-For-Running-OpenCV-On-NPU):
#mkdir /opt
#cd /opt
#wget https://github.com/VeriSilicon/TIM-VX/releases/download/v1.1.34.fix/aarch64_A311D_6.4.8.tgz
#tar xvf aarch64_A311D_6.4.8.tgz
#echo "export VIVANTE_SDK_DIR=/opt/aarch64_A311D_6.4.8" >> /root/.bashrc
#/bin/cp /opt/aarch64_A311D_6.4.8/lib/galcore.ko /usr/lib/modules/4.9.241/kernel/drivers/amlogic/npu/galcore.ko
#/bin/cp -a /opt/aarch64_A311D_6.4.8/lib/* /usr/lib/
#/bin/rm /usr/lib/galcore.ko

# this does not work (wrong kernel detected), will have to run manually on platform:
cd /
cat <<-EOF > jevoispro-fixup.sh
#!/bin/bash
cd /usr/src/linux-headers-4.9.*/arch
sudo ln -s arm64 aarch64
cd /
dkms add -m 8812au -v 4.2.3
sudo dkms build -m 8812au -v 4.2.3
sudo dkms install -m 8812au -v 4.2.3
sudo systemctl set-default jevoispro.target

sync
sudo /bin/rm /jevoispro-fixup.sh
sync
shutdown -r now
EOF

chmod a+x jevoispro-fixup.sh

# Clean up
apt-get update
apt-get -y upgrade # upgrade NPU and GPU packages from jevois repo
apt-get -y clean
apt-get -y autoclean
#history -c

# Self-deleting
rm $0
