# Frame2LSL

The purpose of the Frame2LSL plugin is twofold:
1. Stream to [LSL](https://github.com/sccn/labstreaminglayer) raw frames captured by a camera that supports [v4l2](http://linuxtv.org/downloads/v4l-dvb-apis/) for online  behavior tracking.
2. Save the captured frames to a video file in a compressed format and sends to LSL frame markers to automatically synchronize the electrophysiological data and the video for offline analysis.

The Frame2LSL plugin is based on [FrameGrabber](https://github.com/arnefmeyer/FrameGrabberPlugin), we deeply thank the authors of the latter.

By dropping the plugin onto the GUI signal chain area two LSL outlets are created: 1) one named "Video"-`hostname` that broadcasts the raw frames for online processing, 2) one named "FrameMarker"-`hostname` that sends frame markers that allow synchronizing the video and LFP data. Here we use `hostname` as the name of the computer running the GUI.

To save LSL data to a file we can use  [OpenEphysLSLRecorder](https://bitbucket.org/neatlabs/lslrecorder/src/eb6db559790e11b1abf8bdf4c3dfd874e16a06b3/build/OpenEphysLSLRecorder.sh?at=master&fileviewer=file-view-default).

## Installation ##
At the moment the plugin supports out of the box the GNU/Linux OS only, although
it can be compiled for other architectures using the OS-specific LSL library. The
LSL binary for Linux is already included. To compile the project run the following commands in a bash terminal:
```bash
cd path/to/plugin-GUI
cd Resources/Scripts/
./install_linux_dependencies.sh
cd ../../Builds/Linux/
./setup.sh
```
The `setup.sh` script above will compile `open-ephys` and all the plugins (including ours). At this point you are free to copy/move the newly created `build/` folder out of the `plugin-GUI` project folder to any other location for deployment.  

## Authors ##
The **Frame2LSL** plugin was created and is maintained by engineers and scientists at [NEATLabs](http://neatlabs.ucsd.edu/index.html) with the University of California San Diego, and is part of a wider suite of programs created for supporting closed-loop brain-machine interfaces for humans and implanted animals.
