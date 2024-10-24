#!/bin/bash
set -e

CI_BUILD_USER=$(id -u -n)
CI_BUILD_UID=$(id -u)
CI_BUILD_GROUP=$(id -g -n)
CI_BUILD_GID=$(id -g)
CI_BUILD_HOME="/home/${CI_BUILD_USER}"

echo "Welcome to JeVoisPro-SDK module compilation container\n"
echo "Create new C++ module: jevoispro-create-module <VendorName> <ModuleName>"
echo "Create new Python module: jevoispro-create-python-module <VendorName> <ModuleName>\n"
echo "Then cd to module directory, edit code, and ./rebuild-pro-platform-pdeb.sh\n"
echo "Then copy the resulting .deb file to your microSD,"
echo "and install it using 'dpkg -i package.deb' on the running camera.\n\n"

docker run --rm -it --name jevoispro-compile-modules --pid=host --network=host \
       -v $(pwd):/jevoispro-sdk --env "CI_BUILD_HOME=${CI_BUILD_HOME}" \
       --workdir /jevoispro-sdk \
       --env "CI_BUILD_USER=${CI_BUILD_USER}" --env "CI_BUILD_UID=${CI_BUILD_UID}" \
       --env "CI_BUILD_GROUP=${CI_BUILD_GROUP}" --env "CI_BUILD_GID=${CI_BUILD_GID}" \
       -v /etc/localtime:/etc/localtime:ro -v /etc/timezone:/etc/timezone:ro \
       jevois/jevoispro-compile-modules:1.22.0 \
       /with_the_same_user /bin/bash -l
