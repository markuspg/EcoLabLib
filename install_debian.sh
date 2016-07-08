#!/bin/bash

set -e

echo -e "\e[32mAdding the 'i386' architecture needed by 'Wine' on 64bit systems\e[0m"
sudo dpkg --add-architecture i386

echo -e '\e[32mResynchronizing the package index files\e[0m'
sudo apt-get -q update

read -p 'Shall the entire KDE task be installed for best functionality (y/n)?: ' installKDETask
if [ "$installKDETask" = 'y' ] || [ "$installKDETask" = 'yes' ]
then
    sudo apt-get -q install task-kde-desktop
fi

echo -e '\e[32mInstalling all required dependencies for building and running the EcoLabLib suite\e[0m'
sudo apt-get -q install build-essential coreutils dolphin ghostscript git iceweasel iputils-ping konsole krdc libqt5websockets5-dev lpr okular openssh-client psmisc qt5-default texlive-binaries texlive-latex-base util-linux wakeonlan wine wmctrl

echo -e '\e[32mCloning all relevant repositories into a dedicated working directory\e[0m'
mkdir -p EcoLabLibInstallationDirectory
cd EcoLabLibInstallationDirectory
git clone https://github.com/markuspg/EcoLabLib.git
git clone https://github.com/markuspg/Labcontrol.git
git clone https://github.com/markuspg/WebcamDisplay.git

echo -e '\e[32mBuilding and installing the needed software\e[0m'
mkdir -p build-EcoLabLib
cd build-EcoLabLib
qmake ../EcoLabLib
make
sudo make install
cd ..
mkdir -p build-Labcontrol
cd build-Labcontrol
qmake ../Labcontrol
make
sudo cp -v Labcontrol /usr/local/bin
cd ..
mkdir -p build-WebcamDisplay
cd build-WebcamDisplay
qmake  ../WebcamDisplay/
make
sudo cp -v WebcamDisplay /usr/local/bin
cd ..

echo -e '\e[32mInstalling configuration files\e[0m'
sudo mkdir -pv /etc/xdg/Economic\ Laboratory
sudo cp -v EcoLabLib/data/EcoLabLib.conf /etc/xdg/Economic\ Laboratory
sudo cp -v EcoLabLib/data/EcoLabLibDependencies.conf /etc/xdg/Economic\ Laboratory

echo -e "\e[32mCreating the data storage directory for 'EcoLabLib'\e[0m"
sudo mkdir -pv /usr/local/share/ecolablib

echo -e '\e[32mInstalling the example LaTeX header file. Adjust and rename this as you like\e[0m'
echo -e "\e[32mBe aware that any header files must end on '_header.tex' to be recognized\e[0m"
sudo cp -v EcoLabLib/data/example_header.tex /usr/local/share/ecolablib

echo -e "\e[32mCreating the 'preprints' folder. Store any frequently printed files there\e[0m"
sudo mkdir -pv /usr/local/share/ecolablib/preprints

echo -e '\e[32mCreating the needed SSL certificate and key files\e[0m'
openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ecolablib.key -out ecolablib.cert
sudo cp -v ecolablib.cert /usr/local/share/ecolablib
sudo cp -v ecolablib.key /usr/local/share/ecolablib

echo -e "\e[32mInstalling 'Labcontrol' '.desktop' file\e[0m"
sudo cp -v Labcontrol/labcontrol.desktop /usr/share/applications

echo -e '\e[32mCreating the main directory for all z-Tree executables\e[0m'
sudo mkdir -pv /opt/z-Leaves
echo -e "\e[32mAfter finish of this script please copy all 'zleaf.exe' versions to separate\e[0m"
echo -e "\e[32msubdirectories of '/opt/z-Leaves/' following the naming scheme\e[0m"
echo -e "\e[32m'/opt/z-Leaves/zTree_X.Y.Z/zleaf.exe' or '/opt/z-Leaves/zTree_X.Y.ZZ/zleaf.exe'\e[0m"

exit 0
