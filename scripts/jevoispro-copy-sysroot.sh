#!/bin/bash

# Need to run with root permissions

set -e # exit on any error

# Check location:
r="${JEVOISPRO_SDK_ROOT}"
if [ "X${r}" = "X" ]; then r="/usr/share/jevoispro-sdk"; fi
echo "Using JeVoisPro SDK root: ${r}"
if [ ! -d "fenix" ]; then echo "Cannot find fenix/ -- ABORT"; exit 2; fi

bdir="fenix/build/rootfs-jevois/rootfs-JVPRO-noble-xfce/"
if [ ! -d "$bdir" ]; then
    bdir="fenix/build/rootfs-jevois/rootfs-JVPRO-focal-xfce/"
    if [ ! -d "$bdir" ]; then
        echo "Cannot find fenix/build/rootfs-jevois/rootfs-JVPRO-[focal|noble]-xfce/ -- run rebuild-os.sh -- ABORT";
        exit 3
    fi
fi
    
echo "Nuke old jevoispro-sysroot ..."
if [ ! -d "${r}" ]; then mkdir -p "${r}"; fi
sr="${r}/jevoispro-sysroot"
/bin/rm -rf "${sr}"

echo "Create new jevoispro-sysroot ..."
mkdir "${sr}"

cd "${bdir}"
echo "Copy system ..."
tar cpf - . | ( cd "${sr}"; tar xpf - )

cd "${sr}"

# Now fix all symlinks that point to absolute paths, so that our compiler does not get confused trying to follow them.
# For example, libblas.so.3 is a symlink to /etc/alternatives/.../libblas.so.3 but we do want to get the aarch64 lib
# and not the x86_64 version that may (or not) be installed on our host system.
didit=1
while [ $didit -eq 1 ]; do
    echo "Convert absolute symlinks to relative in ${sr} ..."
    didit=0
    find . -type l | while read l; do
        dest="$(readlink "$l")"
        if [[ ${dest:0:1} == "/" ]]; then
            #echo "   checking $l -> $dest"
            srcdir="$(dirname "$(realpath -s "$l")")"
            #echo "      SRC $srcdir"
            if [ -e ".$dest" -o -h ".$dest" ]; then
                destdir="$(dirname "$(realpath -s ".$dest")")"
                #echo "      DEST $destdir"
                mod="$(realpath --relative-to="$srcdir" -s ".$dest")"
                echo "$l -> $dest CHANGED TO -> ${mod}"
                ln -fs "${mod}" "$l"
                didit=1
            else
                echo "Skipping broken link $l -> $dest"
            fi
        fi
    done
done
