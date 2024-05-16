#!/bin/bash
# USAGE: build.sh [uburel]

make

ver="1.2.0"
pkgrel="1"
arch=`dpkg --print-architecture`

if [ "$#" -ne 1 ]; then
    uburel=`lsb_release -rs`
else
    uburel="$1"
fi


sudo checkinstall \
	--type debian \
	--pkgname jevoispro-fan \
	--provides jevoispro-fan \
	--pkglicense "GPL v3" \
	--maintainer "Laurent Itti \\<jevois.org@gmail.com\\>" \
	--nodoc \
	--arch "${arch}" \
	--pkgsource="https://github.com/jevois/jevoispro-fan/archive/${ver}.tar.gz" \
	--pkgaltsource="https://github.com/jevois/jevoispro-fan/tree/${ver}" \
	--pkgversion "${ver}" \
	--reset-uids \
	--exclude "/home*" \
	--pkggroup "universe" \
	-y \
	--install=no \
	--pkgrelease "${pkgrel}ubuntu${uburel}" \
	make install
    
