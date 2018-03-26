/*
  ==============================================================================

    LSLOutlet.h
    Created: 11/22/2017
    Author:  Alejandro Ojeda

  ==============================================================================
*/

#ifndef LSLOUTLET_H_INCLUDED
#define LSLOUTLET_H_INCLUDED

#include <ProcessorHeaders.h>
#include <memory>
#include "lsl_cpp.h"


class LSLOutlet : public GenericProcessor
{
public:
    LSLOutlet();
    ~LSLOutlet();

    AudioProcessorEditor* createEditor() override;

    void createOutlet ();
    void process (AudioSampleBuffer& buffer) override;

    std::string streamName;
    std::string sourceID;
    int numberOfChannels;
    float* sample;
    lsl::stream_outlet* outlet = NULL;

    //void saveCustomParametersToXml (XmlElement* parentElement) override;
    //void loadCustomParametersFromXml() override;


private:
  bool streaming = false;
};


#endif  // LSLOUTLET_H_INCLUDED
