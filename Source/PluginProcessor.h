/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define DELAYTIME_ID "delayTime"
#define DELAYTIME_NAME "DelayTime"

#define OFFSET_ID "offset"
#define OFFSET_NAME "OffSet"

//==============================================================================
/**
*/
class MajorProjectAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    MajorProjectAudioProcessor();
    ~MajorProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void fillDoublerBuffer(int channel, const float* bufferData, const float* doublerBufferData, const int bufferLength, const int doublerBufferLength);
	void delayTime(const int doublerBufferLength, AudioBuffer<float>& buffer, int channel, const int bufferLength, const float* bufferData, const float* doublerBufferData, const float* doublerBufferData2);

	void updateFilter();

	double mdelayTime;
	double moffSetTime;
	AudioProcessorValueTreeState parameters;

private:
	AudioBuffer<float> mDoublerBuffer;
	AudioBuffer<float> mDoublerBuffer2;
	int mWritePosition{};
	int mSampleRate{ 41000 };

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highShelfFilter;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MajorProjectAudioProcessor)
};
