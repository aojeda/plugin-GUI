#!/bin/bash

# This script installs required packages using apt-get
# It must be run with sudo. Example:
# sudo bash install_linux_dependencies.sh

# install g++
apt-get -y install build-essential

# install Juce dependencies
apt-get -y install freeglut3-dev libfreetype6-dev libxinerama-dev libxcursor-dev libasound2-dev
apt-get -y install libhdf5-10 libhdf5-cpp-11 libhdf5-dev hdf5-helpers
apt-get -y install libzmq1 libzmq3-dev libzmq5
apt-get -y install libv4l-0 libv4l-dev
apt-get -y install libcv2.4 libhighgui2.4 libcv-dev libopencv-dev ffmpeg
cp ../../Source/Plugins/LSLOutlet/liblsl64.so /usr/lib/liblsl64.so.1.0.0
