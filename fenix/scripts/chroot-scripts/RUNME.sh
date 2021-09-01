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
pip3 install pip --upgrade
pip3 install numpy --upgrade
pip3 install dataclasses
pip3 install mediapipe-0.8-cp38-none-linux_aarch64.whl
rm mediapipe-0.8-cp38-none-linux_aarch64.whl

wget http://jevois.org/pkg/rtl8812au-jevois.tbz
tar jxf rtl8812au-jevois.tbz
/bin/rm rtl8812au-jevois.tbz
dkms add -m 8812au -v 4.2.3

systemctl disable hostapd

# this does not work (wrong kernel detected), will have to run manually on platform:
cat <<-EOF > jevoispro-fixup.sh
#!/bin/bash
cd /usr/src/linux-headers-4.9.*/arch
sudo ln -s arm64 aarch64
cd /
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
apt-get -y clean
apt-get -y autoclean
#history -c

# Self-deleting
rm $0
