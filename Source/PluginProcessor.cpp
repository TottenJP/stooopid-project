/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MajorProjectAudioProcessor::MajorProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),

	mdelayTime(),
	moffSetTime(),
	parameters(*this, nullptr),
	highShelfFilter(dsp::IIR::Coefficients<float>::makeHighShelf(44100, 20000.0f, 1.0f, 0.5f))
	


#endif
{
	NormalisableRange<float> delayTimeRange(0.0, 25.0, 0.5);
	NormalisableRange<float> offSetRange(0.0, 15, 0.5);

	parameters.createAndAddParameter(DELAYTIME_ID, DELAYTIME_NAME, DELAYTIME_NAME, delayTimeRange, 4.0, nullptr, nullptr);
	parameters.createAndAddParameter(OFFSET_ID, OFFSET_NAME, OFFSET_NAME, offSetRange, 6.0, nullptr, nullptr);

	parameters.state = ValueTree("savedParams");
}

MajorProjectAudioProcessor::~MajorProjectAudioProcessor()
{
}

//==============================================================================
const String MajorProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MajorProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MajorProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MajorProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MajorProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MajorProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MajorProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MajorProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String MajorProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void MajorProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MajorProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	const int doublerBufferSize = 2 * (sampleRate * samplesPerBlock);
	mSampleRate = sampleRate;

	mDoublerBuffer.setSize(getTotalNumInputChannels(), doublerBufferSize, false, true, true);
	mDoublerBuffer2.setSize(getTotalNumInputChannels(), doublerBufferSize, false, true, true);

	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = doublerBufferSize;
	spec.numChannels = getTotalNumOutputChannels();


	highShelfFilter.prepare(spec);
	highShelfFilter.reset();

}

void MajorProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MajorProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MajorProjectAudioProcessor::updateFilter()
{
	//float GAIN = *tree.getRawParameterValue("filterGain");
	//float FREQ = *tree.getRawParameterValue("filterFreq");
	// float Q = *tree.getRawParameterValue("filterQ");

	*highShelfFilter.state = *dsp::IIR::Coefficients<float>::makeHighShelf(mSampleRate, 1000.0f, 0.5, 0.35);
}

void MajorProjectAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

																			 // In case we have more outputs than inputs, this code clears any output
																			 // channels that didn't contain input data, (because these aren't
																			 // guaranteed to be empty - they may contain garbage).
																			 // This is here to avoid people getting screaming feedback
																			 // when they first compile a plugin, but obviously you don't need to keep
																			 // this code if your algorithm always overwrites all the output channels.



    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	const int bufferLength = buffer.getNumSamples();
	const int doublerBufferLength = mDoublerBuffer.getNumSamples();
	const int doublerBufferLenth2 = mDoublerBuffer2.getNumSamples();

																			 // This is the place where you'd normally do the guts of your plugin's
																			 // audio processing...
																			 // Make sure to reset the state if your inner loop is processing
																			 // the samples and the outer loop is handling the channels.
																			 // Alternatively, you can process the samples with the channels
																			 // interleaved by keeping the same state. 
	dsp::AudioBlock<float> blockL(mDoublerBuffer);
	updateFilter();
	//highShelfFilter.process(dsp::ProcessContextReplacing<float>(blockL.getSingleChannelBlock(0)));


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		const float* bufferData = buffer.getReadPointer(channel);
		const float* doublerBufferData = mDoublerBuffer.getReadPointer(channel);
		const float* doublerBufferData2 = mDoublerBuffer2.getReadPointer(channel);
        auto* channelData = buffer.getWritePointer (channel);

		fillDoublerBuffer(channel, bufferData, doublerBufferData, bufferLength, doublerBufferLength);
		delayTime(doublerBufferLength, buffer, channel, bufferLength, bufferData, doublerBufferData, doublerBufferData2);
    }

	mWritePosition += bufferLength;
	mWritePosition %= doublerBufferLength;

}

void MajorProjectAudioProcessor::fillDoublerBuffer(int channel, const float* bufferData, const float* doublerBufferData, const int bufferLength, const int doublerBufferLength)
{
	if (doublerBufferLength > bufferLength + mWritePosition)
	{
		mDoublerBuffer.addFrom(channel, mWritePosition, bufferData, bufferLength, 1.0F);

		mDoublerBuffer2.addFrom(channel, mWritePosition, bufferData, bufferLength, 1.0F);
	}
	else
	{
		const int bufferRemaining = doublerBufferLength - mWritePosition;

		mDoublerBuffer.copyFrom(channel, mWritePosition, bufferData, bufferRemaining, 1.0F);
		mDoublerBuffer.copyFrom(channel, 0, bufferData, bufferLength - bufferRemaining, 1.0F);

		mDoublerBuffer2.copyFrom(channel, mWritePosition, bufferData, bufferRemaining, 1.0F);
		mDoublerBuffer2.copyFrom(channel, 0, bufferData, bufferLength - bufferRemaining, 1.0F);
	}
}

void MajorProjectAudioProcessor::delayTime(const int doublerBufferLength, AudioBuffer<float>& buffer, int channel, const int bufferLength, const float* bufferData, const float* doublerBufferData, const float* doublerBufferData2) 
{
	auto DelayTime = mdelayTime;
	auto lagOffset = 15.0;

	auto channelOffset = moffSetTime;
	

	//doubler 1
	const int readPosition = static_cast<int> (doublerBufferLength + mWritePosition - (mSampleRate * DelayTime / 1000)) % doublerBufferLength;
	const int readPositionLag = static_cast<int> (doublerBufferLength + mWritePosition - (mSampleRate * (DelayTime + lagOffset) / 1000)) % doublerBufferLength;
	
	//doubler 2
	const int readPosition2 = static_cast<int> (doublerBufferLength + mWritePosition - (mSampleRate * (DelayTime + channelOffset) / 1000)) % doublerBufferLength;
	const int readPositionLag2 = static_cast<int> (doublerBufferLength + mWritePosition - (mSampleRate * (DelayTime + channelOffset + lagOffset) / 1000)) % doublerBufferLength;

	auto lagGain = 0.65f;

	if (doublerBufferLength > mWritePosition + bufferLength)
	{
		// Doubler 1
		buffer.copyFrom(0, 0, doublerBufferData + readPosition, bufferLength, 1.0F);
		buffer.copyFrom(1, 0, doublerBufferData + readPositionLag, bufferLength, lagGain);

		// Doubler 2
		buffer.addFrom(0, 0, doublerBufferData2 + readPositionLag2, bufferLength, lagGain);
		buffer.addFrom(1, 0, doublerBufferData2 + readPosition2, bufferLength, 1.0f);
	}
	else
	{
		//doubler 1
		const int bufferRemaining = doublerBufferLength - readPosition;

		//doubler 2
		const int bufferRemaining2 = doublerBufferLength - readPosition2;
		
		//Doubler 1
		buffer.copyFrom(0, 0, doublerBufferData + readPosition, bufferRemaining, 1.0F);
		buffer.copyFrom(0, bufferRemaining, doublerBufferData, bufferLength - bufferRemaining, 1.0F);

		buffer.copyFrom(1, 0, doublerBufferData + readPositionLag, bufferRemaining, lagGain);
		buffer.copyFrom(1, bufferRemaining, doublerBufferData, bufferLength - bufferRemaining, lagGain);


		//Doubler 2
		buffer.addFrom(0, 0, doublerBufferData2 + readPositionLag2, bufferRemaining2, lagGain);
		buffer.addFrom(0, bufferRemaining2, doublerBufferData2, bufferLength - bufferRemaining2, lagGain);

		buffer.addFrom(1, 0, doublerBufferData2 + readPosition2, bufferRemaining2, 1.0F);
		buffer.addFrom(1, bufferRemaining2, doublerBufferData2, bufferLength - bufferRemaining2, 1.0F);
	}

}

//==============================================================================
bool MajorProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MajorProjectAudioProcessor::createEditor()
{
    return new MajorProjectAudioProcessorEditor (*this);
}

//==============================================================================
void MajorProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	auto xml(parameters.state.createXml());
	copyXmlToBinary(*xml, destData);

}

void MajorProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	auto theParams(getXmlFromBinary(data, sizeInBytes));

	if (theParams != nullptr);
	{
		if (theParams->hasTagName(parameters.state.getType()))
		{
			parameters.state = ValueTree::fromXml(*theParams);
		}
			
	}

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MajorProjectAudioProcessor();
}
