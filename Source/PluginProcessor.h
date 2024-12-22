#pragma once
#include <JuceHeader.h>
#include "ImpulseResponder.h"

class IRLoaderAudioProcessor  : public juce::AudioProcessor
{
public:
    IRLoaderAudioProcessor();
    ~IRLoaderAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void loadImpulseResponse(const juce::File& file, const juce::dsp::Convolution::Stereo isStereo, const juce::dsp::Convolution::Trim needsTrim, int size);

    ImpulseResponder ir;
    juce::AudioProcessorValueTreeState apvts;
	juce::File root, savedFile;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	float dry = 100.0;
	float wet = 50.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLoaderAudioProcessor)
};