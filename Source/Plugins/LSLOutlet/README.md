# LSLOutlet
![fig1](https://github.com/aojeda/plugin-GUI/blob/master/Source/Plugins/LSLOutlet/fig1.png)

The LSLOutlet plugin allows broadcasting LFP data collected with Intan
acquisition boards supported by the [Open Ephys](http://www.open-ephys.org) project
to the local area network using the lab streaming layer library ([LSL](https://github.com/sccn/labstreaminglayer)).

The LSL library can handle the synchronization between different acquisition
and stimulation devices commonly used in neuroscientific experiments.
By using this plugin, researchers can pipe LFP data to the network for building fully
customized signal processing and visualizations using the languages that support
LSL (C/C++, C#, Python, MATLAB, Java), as well as saving data using the multimodal
file format *xdf* for offline analysis. *xdf* files can be loaded into [MATLAB or Python](https://github.com/sccn/xdf)
and can be reviewed and further processed within the EEGLAB environment using the [MoBILAB](https://sccn.ucsd.edu/wiki/MoBILAB) toolbox.

By dropping the plugin onto the GUI signal chain area, an LSL outlet is created, which is named using the following convention "LFP"-`hostname`, where `hostname` is the name of the computer running the GUI. By default, the outlet will send to LSL all the channels received from the source node connected to it. All the metadata such as channel names and sampling rate are obtained automatically from the source node and passed on to LSL.

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
The **LSLOutlet** plugin was created and is maintained by engineers and scientists at [NEATLabs](http://neatlabs.ucsd.edu/index.html) with the University of California San Diego, and is part of a wider suite of programs created for supporting closed-loop brain-machine interfaces for humans and implanted animals.
