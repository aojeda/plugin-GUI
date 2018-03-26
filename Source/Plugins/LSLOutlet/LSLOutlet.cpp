/*
  ==============================================================================

    LSLOutlet.cpp
    Created: 11/22/2017
    Author:  Alejandro Ojeda

  ==============================================================================
*/

#include "LSLOutlet.h"
#include "LSLOutletEditor.h"
#include "lsl_cpp.h"


LSLOutlet::LSLOutlet()
    : GenericProcessor  ("LSL Outlet")
{
    setProcessorType (PROCESSOR_TYPE_SINK);
}

LSLOutlet::~LSLOutlet()
{
  if(this->outlet != NULL)
      delete this->outlet;
}


AudioProcessorEditor* LSLOutlet::createEditor()
{
    editor = new LSLOutletEditor(this, true);
    return editor;
}

void LSLOutlet::createOutlet ()
{
  numberOfChannels = getNumInputs();
  if(numberOfChannels==0)
    return;

  std::cout << "Creating outlet "<< streamName << "...\n";

  // Create LSL info
  lsl::stream_info info(streamName,"LFP",this->numberOfChannels,this->getSampleRate(),lsl::cf_float32,sourceID);
  info.desc().append_child_value("manufacturer", "Open Ephys");
  lsl::xml_element chns = info.desc().append_child("channels");
  std::cout<< "Channel labels:\n";
  for (int k=0; k < this->numberOfChannels; k++)
  {
    std::cout<< this->getInputChannelName(k).toStdString()+std::to_string(k+1)<<"\n";
    chns.append_child("channel")
    .append_child_value("label", this->getInputChannelName(k).toStdString()+"_"+std::to_string(k+1))
    .append_child_value("unit","microvolts")
    .append_child_value("type","LFP");
  }
  this->sample = new float[this->numberOfChannels];
  for(int k=0;k<this->numberOfChannels;k++)
    this->sample[k] = 0;

  // Create LSL outlet
  if(this->outlet != NULL)
    delete this->outlet;
  this->outlet = new lsl::stream_outlet(info);
  this->outlet->push_sample(sample);
  std::cout<< "Number of channels: " << this->numberOfChannels <<"\n";
  std::cout<< "Sampling rate: " << this->getSampleRate() <<"\n";
  std::cout<< "done\n";
}


void LSLOutlet::process(AudioSampleBuffer& buffer)
{
  if(this->outlet == NULL)
    this->createOutlet();
  const int numberOfSamples = getNumSamples(0);
  if(numberOfSamples<1)
    return;
  const float** dataChunk = buffer.getArrayOfReadPointers();
  float* bufPtr;

  if(!this->streaming){
    std::cout << "Streaming..." << '\n';
    this->streaming = true;
  }
  for(int k=0; k<numberOfSamples; k++){
    for(int ch=0; ch<numberOfChannels;ch++){
      bufPtr = (float*)buffer.getReadPointer(ch);
      sample[ch] = bufPtr[k];
    }
    outlet->push_sample(sample);
  }
}
