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

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include "v4l2framegrabber.h"
#include "FrameGrabber.h"
#include "lsl_cpp.h"
#include <unistd.h>
#include <limits.h>
#include <time.h>

void pause(int milliseconds) // Cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end){}
}


FrameGrabber::FrameGrabber()
    : GenericProcessor("Frame2LSL"), camera(NULL), currentFormatIndex(-1),
	  frameCounter(0), Thread("FrameGrabberThread"), colorMode(ColorMode::RGB),
		resetFrameCounter(false){
      setProcessorType (PROCESSOR_TYPE_SOURCE);
    }

FrameGrabber::~FrameGrabber()
{
	stopCamera();
	std::cout << "Camera closed.\n";
	if (this->outlet != NULL){
		delete this->outlet;
		std::cout << "LSL outlet closed.\n";
	}
}

AudioProcessorEditor* FrameGrabber::createEditor()
{
    editor = new FrameGrabberEditor(this, true);
    return editor;
}

void FrameGrabber::updateSettings()
{
	if (editor != NULL)
	  editor->update();
}

void FrameGrabber::process(AudioSampleBuffer& buffer, MidiBuffer& events){}

juce::int64 FrameGrabber::getFrameCount()
{
	int count;
	lock.enter();
	count = frameCounter;
	lock.exit();
	return count;
}


int FrameGrabber::startCamera(int fmt_index)
{
	if (isCameraRunning())
	{
		if (stopCamera())
		  return 1;
	}
	camera = new Camera(fmt_index);
	if (camera->init())
	{
		std::cout <<  "FrameGrabber: could not open camera\n";
		return 1;
	}
	if (camera->start())
	{
		std::cout <<  "FrameGrabber: could not open camera\n";
		return 1;
	}
	if (camera->is_running())
	{
		std::cout << "FrameGrabber: opened camera " << camera->get_format()->to_string() << "\n";
		currentFormatIndex = fmt_index;
		threadRunning = true;
		fps = (int)(camera->get_format()->denominator/camera->get_format()->numerator);
		startThread();
	}
	return 0;
}

int FrameGrabber::stopCamera()
{
	threadRunning = false;
	waitForThreadToExit(1000);
	currentFormatIndex = -1;
	if (camera != NULL)
	{
		delete camera;
		camera = NULL;
	}
}

bool FrameGrabber::isCameraRunning(){
	return (camera != NULL && camera->is_running());
}

std::vector<std::string> FrameGrabber::getFormats()
{
	std::vector<std::string> formats = Camera::list_formats_as_string();
	return formats;
}

int FrameGrabber::getCurrentFormatIndex(){
	return currentFormatIndex;
}

void FrameGrabber::setColorMode(int value)
{
	lock.enter();
	colorMode = value;
	lock.exit();
}

int FrameGrabber::getColorMode()
{
	int mode;
	lock.enter();
	mode = colorMode;
	lock.exit();
	return mode;
}

void FrameGrabber::createOutlet ()
{
	lock.enter();
	bool cMode = colorMode;
	lock.exit();
	cv::Mat frame  = camera->read_frame();
  numberOfPixels = frame.total();
	//numberOfPixels = frame.cols*frame.rows;
	std::cout<< "Channels: "<<frame.channels()<<"\n";

	int elementsPerFrame = (cMode)? numberOfPixels*3: numberOfPixels;

	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	streamName = std::string("Video_")+std::string(hostname);
	sourceID   = std::string("Video_")+std::string(hostname);
	std::cout << "Creating outlet "<< streamName << "...\n";

	// Create LSL info
  lsl::stream_info info(streamName,"RawVideo",elementsPerFrame,(int)fps,lsl::cf_int8,sourceID);
	info.desc().append_child_value("cols", juce::String(frame.cols).toStdString());
	info.desc().append_child_value("rows", juce::String(frame.rows).toStdString());
	info.desc().append_child_value("color", juce::String(cMode).toStdString());

	this->sample = new int[elementsPerFrame];
  for(int k=0;k<elementsPerFrame;k++)
    this->sample[k] = 0;
	// Create LSL outlet
  if(this->outlet != NULL)
    delete this->outlet;
  this->outlet = new lsl::stream_outlet(info);
  std::cout<< "Number of pixels: " << this->numberOfPixels <<"\n";
  std::cout<< "Frames per second: " << fps <<"\n";
  std::cout<< "done\n";
	std::cout<<"Streaming video to LSL...\n";
}

void ReadRGB(cv::Mat frame, int* sample)
{
	int nr = frame.rows;
	int nc = frame.cols;
	cv::Mat rgb[3];
	cv::split(frame,rgb);
	int c = 0;
	sample[1] = (int)rgb[0].data[1*rgb[0].step+0];
	for(int i=0;i<nc;i++)
		for(int j=0;j<nr;j++)
			for(int k=0;k<3;k++)
				sample[c++] = (int)rgb[k].data[j*rgb[k].step+i];
}

void ReadGray(cv::Mat frame, int* sample)
{
	int nr = frame.rows;
	int nc = frame.cols;
	int c = 0;
	for(int i=0;i<nc;i++)
		for(int j=0;j<nr;j++)
			sample[c++] = (int)frame.data[j*frame.step+i];
}

void FrameGrabber::run()
{
	lock.enter();
	bool cMode = colorMode;
	lock.exit();

	if (this->outlet == NULL)
		this->createOutlet();

	std::cout<< "Video capture thread running...\n";
  while (threadRunning){
		if (camera != NULL && camera->is_running()){
			cv::Mat frame = camera->read_frame();
			if (!frame.empty()){
				if(!cMode){
					cv::Mat grayFrame;
					cvtColor( frame, grayFrame, CV_BGR2GRAY );
					frame = grayFrame;
				}

				cv::imshow("Frame2LSL", frame);
				if (frame.channels()>2)
					ReadRGB(frame, this->sample);
				else
					ReadGray(frame, this->sample);
				this->outlet->push_sample(this->sample);
				cv::waitKey(1);
				frameCounter++;
			}
		}
  }
  return;
}


void FrameGrabber::saveCustomParametersToXml(XmlElement* xml)
{
	xml->setAttribute("Type", "Frame2LSL");
  XmlElement* paramXml = xml->createNewChildElement("PARAMETERS");
	paramXml->setAttribute("ColorMode", getColorMode());

	XmlElement* deviceXml = xml->createNewChildElement("DEVICE");
	deviceXml->setAttribute("API", "V4L2");
	if (currentFormatIndex >= 0)
	  deviceXml->setAttribute("Format", Camera::get_format_string(currentFormatIndex));
	else
	  deviceXml->setAttribute("Format", "");
}


void FrameGrabber::loadCustomParametersFromXml()
{
	forEachXmlChildElementWithTagName(*parametersAsXml,	paramXml, "PARAMETERS")
	{
		if (paramXml->hasAttribute("ColorMode"))
			setColorMode(paramXml->getIntAttribute("ColorMode"));
	}

	forEachXmlChildElementWithTagName(*parametersAsXml,	deviceXml, "DEVICE")
	{
		String api = deviceXml->getStringAttribute("API");
		if (api.compareIgnoreCase("V4L2") == 0)
		{
			String format = deviceXml->getStringAttribute("Format");
			int index = Camera::get_format_index(format.toStdString());
			if (index >= 0)
			{
				if (isCameraRunning())
				{
					stopCamera();
				}
				startCamera(index);
			}
		}
		else
		{
			std::cout << "FrameGrabber API " << api << " not supported\n";
		}
	}

	updateSettings();
}
