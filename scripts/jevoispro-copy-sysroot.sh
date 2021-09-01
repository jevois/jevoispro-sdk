#!/bin/bash
set -e # exit on any error

r="${JEVOISPRO_SDK_ROOT}"
if [ "X${r}" = "X" -o ! -d "${r}" ]; then echo "JeVoisPro SDK root [${r}] not found -- ABORT"; exit 1; fi
echo "Using JeVoisPro SDK root: ${r}"
sr="${r}/jevoispro-sysroot"

echo "Nuke old jevoispro-sysroot ..."
cd "${r}"
sudo /bin/rm -rf jevoispro-sysroot

echo "Create new jevoispro-sysroot ..."
sudo mkdir jevoispro-sysroot
cd "${r}/fenix/build/rootfs-jevois/rootfs-JVPRO-focal-xfce/"

echo "Copy system ..."
sudo tar cpf - . | ( cd "${sr}"; sudo tar xpf - )

cd "${sr}"

# Now fix all symlinks that point to absolute paths, so that our compiler does not get confused trying to follow them.
# For example, libblas.so.3 is a symlink to /etc/alternatives/.../libblas.so.3 but we do want to get the aarch64 lib
# and not the x86_64 version that may (or not) be installed on our host system.
didit=1
while [ $didit -eq 1 ]; do
    echo "Convert absolute symlinks to relative ..."
    didit=0
    sudo find . -type l | while read l; do
        dest="$(readlink "$l")"
        if [[ ${dest:0:1} == "/" ]]; then
            #echo "   checking $l -> $dest"
            srcdir="$(dirname "$(realpath -s "$l")")"
            #echo "      SRC $srcdir"
            if [ -e ".$dest" ]; then
                destdir="$(dirname "$(realpath -s ".$dest")")"
                #echo "      DEST $destdir"
                mod="$(realpath --relative-to="$srcdir" -s ".$dest")"
                echo "$l -> $dest CHANGED TO -> ${mod}"
                sudo ln -fs "${mod}" "$l"
                didit=1
            else
                echo "Skipping broken link $l -> $dest"
            fi
        fi
    done
done
