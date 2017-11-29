#!/bin/bash

# install g++
apt-get -y install build-essential

# install Juce dependencies
apt-get -y install freeglut3-dev libfreetype6-dev libxinerama-dev libxcursor-dev libasound2-dev
apt-get -y install libhdf5-10 libhdf5-cpp-11 libhdf5-dev hdf5-helpers
apt-get -y install libzmq1 libzmq3-dev libzmq5

cp ../../Source/Plugins/LSLOutlet/liblsl64.so /usr/lib/liblsl64.so.1.0.0
