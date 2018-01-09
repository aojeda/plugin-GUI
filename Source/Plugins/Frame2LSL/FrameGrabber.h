/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef FRAMEGRABBER_H_INCLUDED
#define FRAMEGRABBER_H_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>
#include "FrameGrabberEditor.h"
#include "lsl_cpp.h"

//class DiskThread;
class Camera;
class CameraFormat;

enum ImageWriteMode {NEVER = 0, RECORDING = 1, ACQUISITION = 2};
enum ColorMode {GRAY = 0, RGB = 1};


class FrameGrabber : public GenericProcessor ,  public Thread
{
public:

    FrameGrabber();
    ~FrameGrabber();
    bool isSource(){return true;}
    bool isSink()  {return false;}
    void process(AudioSampleBuffer& buffer, MidiBuffer& events);

	  //void startRecording();
	  //void stopRecording();
    void createOutlets();
	  void run();
	  AudioProcessorEditor* createEditor();
	  void updateSettings();

	  int startCamera(int fmt_index);
	  int stopCamera();
	  bool isCameraRunning();

	  std::vector<std::string> getFormats();
	  int getCurrentFormatIndex();
	  void setColorMode(int value);
	  int getColorMode();
	  float getFrameCounter();
    void countFrame();
	  void saveCustomParametersToXml(XmlElement* parentElement);
	  void loadCustomParametersFromXml();
    juce::int64 fps;
    std::string getStreamLastName();
    std::string getFilename();
    void setFilename(std::string streamName);

private:
  std::string filename;
  std::string streamName;
  std::string sourceID;
  int numberOfPixels;
  int* sample;
  float mrk[1];
  lsl::stream_outlet* rawVideoOutlet = NULL;
  lsl::stream_outlet* fameMarkerOutlet = NULL;

	Camera* camera;
	float frameCounter=1;
	bool threadRunning;
	int colorMode;
	int currentFormatIndex;
	CriticalSection lock;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrameGrabber);
};


#endif  // FrameGrabber_H_INCLUDED
