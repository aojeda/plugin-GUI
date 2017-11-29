/*
  ==============================================================================

    LSLOutletEditor.cpp
    Created: 11/22/2017
    Author:  Alejandro Ojeda

  ==============================================================================
*/

#include "LSLOutletEditor.h"
#include "LSLOutlet.h"
#include <unistd.h>
#include <limits.h>
#include "lsl_cpp.h"



LSLOutletEditor::LSLOutletEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors)
    : GenericEditor(parentNode, useDefaultParameterEditors)

{
    desiredWidth = 180;

    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);

    urlLabel = new Label("streamName", "Name:");
    urlLabel->setBounds(1,60,140,25);
    addAndMakeVisible(urlLabel);
    LSLOutlet* p = (LSLOutlet*)getProcessor();

    streamName = new Label("streamName", String("LFP_")+String(hostname));
    streamName->setBounds(50,65,120,18);
    streamName->setFont(Font("Default", 15, Font::plain));
    streamName->setColour(Label::textColourId, Colours::white);
    streamName->setColour(Label::backgroundColourId, Colours::grey);
    streamName->setEditable(false);
    addAndMakeVisible(streamName);
    setEnabledState(false);

    p->streamName = streamName->getText().toStdString();
    p->sourceID = streamName->getText().toStdString();
    p->createOutlet();
}
