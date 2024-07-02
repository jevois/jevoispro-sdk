#!/bin/bash
# USAGE: install-dependencies.sh [-y]
# Will install some python packages, docker, compilers, etc

set -e # Exit on any error

gccver=14 # gcc major version to use
numpyver="1.26.4" # numpy version to use

###################################################################################################
if [ "x$1" = "x-y" ]; then usedef=1; else usedef=0; fi
function question { if [ $usedef -eq 1 ]; then REPLY="y"; else read -p "JEVOIS: ${1}? [Y/n] "; fi }

###################################################################################################
question "Install gcc/g++/gfortran v${gccver} for host"
if [ "X$REPLY" != "Xn" ]; then
    sudo apt-get -y install g++-${gccver} gfortran-${gccver}
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${gccver} 100 \
         --slave /usr/bin/g++ g++ /usr/bin/g++-${gccver} \
         --slave /usr/bin/gfortran  gfortran /usr/bin/gfortran-${gccver}
    sudo update-alternatives --set gcc /usr/bin/gcc-${gccver}
fi

###################################################################################################
question "Install gcc/g++/gfortran v${gccver} for JeVois-Pro (aarch64) platform"
if [ "X$REPLY" != "Xn" ]; then
    sudo apt-get -y install g++-${gccver}-aarch64-linux-gnu gfortran-${gccver}-aarch64-linux-gnu
    sudo update-alternatives --install /usr/bin/aarch64-linux-gnu-gcc aarch64-linux-gnu-gcc \
         /usr/bin/aarch64-linux-gnu-gcc-${gccver} 100 \
         --slave /usr/bin/aarch64-linux-gnu-g++ aarch64-linux-gnu-g++ \
         /usr/bin/aarch64-linux-gnu-g++-${gccver} \
         --slave /usr/bin/aarch64-linux-gnu-gfortran aarch64-linux-gnu-gfortran \
         /usr/bin/aarch64-linux-gnu-gfortran-${gccver}
    sudo update-alternatives --set aarch64-linux-gnu-gcc /usr/bin/aarch64-linux-gnu-gcc-${gccver}
fi

###################################################################################################
docker --version &>/dev/null || true
if [ $? -ne 0 ]; then
    question "Install docker, needed to compile contribs"
    if [ "X$REPLY" != "Xn" ]; then
        # Add Docker's official GPG key:
        sudo apt-get update
        sudo apt-get install ca-certificates curl
        sudo install -m 0755 -d /etc/apt/keyrings
        sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
        sudo chmod a+r /etc/apt/keyrings/docker.asc
        
        # Add the repository to Apt sources:
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
        sudo apt-get update
        
        # Install the docker packages:
        sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
    fi
fi

###################################################################################################
if [ ! -d ~/jvenv ]; then
    question "Create ~/jvenv python virtual env for JeVois-Pro (for pycoral, onnx, mediapipe, apriltag, etc)"
    if [ "X$REPLY" != "Xn" ]; then
        sudo apt-get -y install python3.12-venv
        python3 -m venv ~/jvenv
    fi
fi

###################################################################################################
if [ -d ~/jvenv ]; then
    source ~/jvenv/bin/activate
    question "Install numpy ${numpyver}, onnx, pycoral, mediapipe, apriltag wheels for JeVois-Pro in ~/jvenv"
    if [ "X$REPLY" != "Xn" ]; then
        pip install pip --upgrade
        pip install numpy==${numpyver}
        pip install onnxruntime
        pip install apriltag
        pip install -U transformers
        pip install http://jevois.org/pkg/tflite_runtime-2.5.0.post1-cp312-cp312-linux_x86_64.whl
        pip install http://jevois.org/pkg/pycoral-2.0.0-cp312-cp312-linux_x86_64.whl
        pip install http://jevois.org/pkg/mediapipe-0.10.14-cp312-cp312-linux_x86_64.whl
        pip install psutil
        
        curl -fsSL https://ollama.com/install.sh | sh
        pip install ollama
        ollama pull moondream
        ollama pull tinydolphin
    fi
    echo "Remember to run 'source ~/jvenv/bin/activate' before running jevoispro-daemon"
fi
