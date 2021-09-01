#!/bin/bash

make

ver="1.0.0"
pkgrel="1"
arch=`dpkg --print-architecture`
uburel=`lsb_release -rs`


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
    
