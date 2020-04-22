/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MajorProjectAudioProcessorEditor::MajorProjectAudioProcessorEditor (MajorProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 400);


	//---------------------------------------------------------------------------------------------------------------
	slider1Attach = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, DELAYTIME_ID, slider1);

	slider1.addListener(this);
	addAndMakeVisible(slider1);
	slider1.setSliderStyle(Slider::SliderStyle::Rotary);
	slider1.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
	slider1.setRange(0.0, 30.0);
	slider1.setTextValueSuffix(" ms");
	slider1.setNumDecimalPlacesToDisplay(1);

	addAndMakeVisible(slider1);
	label1.setText("DelayTime", dontSendNotification);
	label1.attachToComponent(&slider1, false);

	//--------------------------------------------------------------------------------------------------------------

	slider2Attach = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, OFFSET_ID, slider2);

	slider2.addListener(this);
	addAndMakeVisible(slider2);
	slider2.setSliderStyle(Slider::SliderStyle::Rotary);
	slider2.setTextBoxStyle(Slider::TextBoxBelow, false, 75, 20);
	slider2.setRange(0.0, 15.0);
	slider2.setTextValueSuffix(" ms");
	slider2.setNumDecimalPlacesToDisplay(1);

	addAndMakeVisible(slider2);
	label2.setText("Channel Offset", dontSendNotification);
	label2.attachToComponent(&slider2, false);

}

MajorProjectAudioProcessorEditor::~MajorProjectAudioProcessorEditor()
{
}

//==============================================================================
void MajorProjectAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("FUCK U!", getLocalBounds(), Justification::centred, 1);
}

void MajorProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	slider1.setBounds(((getWidth() / 2) * 1) - 225, (getHeight() / 2) - 100, 200, 200);
	slider2.setBounds(((getWidth() / 2) * 2) - 225, (getHeight() / 2) - 100, 200, 200);
}

void MajorProjectAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &slider1)
	{
		processor.mdelayTime = slider1.getValue();
	}
	if (slider == &slider2)
	{
		processor.moffSetTime = slider2.getValue();
	}
}