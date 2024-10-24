#!/bin/bash
set -e

# We need to install a few things on the host since docker cannot write to /sys and such
sudo apt install qemu-user-static mount

BUILD_ARGS=("$@")
echo "Args passed to rebuild-os.sh: ${BUILD_ARGS[@]}"

CI_BUILD_USER=$(id -u -n)
CI_BUILD_UID=$(id -u)
CI_BUILD_GROUP=$(id -g -n)
CI_BUILD_GID=$(id -g)
CI_BUILD_HOME="/home/${CI_BUILD_USER}"
JEVOISPRO_SDK_ROOT="" # we will use the default /usr/share/jevoispro-sdk in docker

docker run --rm -it --name jevoispro-sdk --pid=host --network=host \
       -v $(pwd):/jevoispro-sdk --env "CI_BUILD_HOME=${CI_BUILD_HOME}" \
       --workdir /jevoispro-sdk \
       --env "CI_BUILD_USER=${CI_BUILD_USER}" --env "CI_BUILD_UID=${CI_BUILD_UID}" \
       --env "CI_BUILD_GROUP=${CI_BUILD_GROUP}" --env "CI_BUILD_GID=${CI_BUILD_GID}" \
       --env "JEVOISPRO_SDK_ROOT=${JEVOISPRO_SDK_ROOT}" \
       -v /etc/localtime:/etc/localtime:ro -v /etc/timezone:/etc/timezone:ro \
       --privileged \
       --device=/dev/loop-control:/dev/loop-control \
       --device=/dev/loop0:/dev/loop0 \
       --cap-add SYS_ADMIN \
       jevois/jevoispro-sdk-build:1.22.0 \
       /with_the_same_user "/jevoispro-sdk/rebuild-os.sh ${BUILD_ARGS[@]}"
