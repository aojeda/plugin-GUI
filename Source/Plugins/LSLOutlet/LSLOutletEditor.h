/*
  ==============================================================================

    LSLOutletEditor.h
    Created: 11/22/2017
    Author:  Alejandro Ojeda

  ==============================================================================
*/

#ifndef LSLOUTLETEDITOR_H_INCLUDED
#define LSLOUTLETEDITOR_H_INCLUDED 

#include <EditorHeaders.h>


/**

 User interface for the "LSLOutlet" source node.

 @see LSLOutlet

 */

class LSLOutletEditor : public GenericEditor
{
public:
    LSLOutletEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);

    //void buttonEvent(Button* button) override;

private:
    ScopedPointer<UtilityButton> restartConnection;
    ScopedPointer<Label> urlLabel;
    ScopedPointer<Label> streamName;

    //ScopedPointer<Label> nChanLabel;
    //ScopedPointer<Label> numberOfChannels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LSLOutletEditor);

};


#endif  // LSLOUTLETEDITOR_H_INCLUDED
