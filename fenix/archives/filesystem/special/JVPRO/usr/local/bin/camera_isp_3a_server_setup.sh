#!/bin/bash

DAEMON=/usr/local/bin/iv009_isp

. /etc/fenix-release

if [ "$BOARD" != "JVPRO" ]; then
	echo "$BOARD does not support ISP!"
	systemctl stop camera_isp_3a_server.service
	exit
fi

$DAEMON

exit 0
