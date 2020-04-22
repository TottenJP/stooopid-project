/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MajorProjectAudioProcessorEditor  : public AudioProcessorEditor,
										  Slider::Listener
{
public:
    MajorProjectAudioProcessorEditor (MajorProjectAudioProcessor&);
    ~MajorProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MajorProjectAudioProcessor& processor;

	//slider1 = mdelayTime
	Slider slider1;
	Label label1;

	//slider2 = Channel off set / moffSetTime
	Slider slider2;
	Label label2;

	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> slider1Attach;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> slider2Attach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MajorProjectAudioProcessorEditor)
};
