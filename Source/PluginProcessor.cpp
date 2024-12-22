#include "PluginProcessor.h"
#include "PluginEditor.h"

IRLoaderAudioProcessor::IRLoaderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

IRLoaderAudioProcessor::~IRLoaderAudioProcessor()
{
}

const juce::String IRLoaderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IRLoaderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IRLoaderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IRLoaderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IRLoaderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IRLoaderAudioProcessor::getNumPrograms()
{
    return 1;
}

int IRLoaderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IRLoaderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IRLoaderAudioProcessor::getProgramName (int index)
{
    return {};
}

void IRLoaderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void IRLoaderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	spec.sampleRate = sampleRate;
	ir.prepare(spec);
}

void IRLoaderAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IRLoaderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// Process Block ==============================================================================
void IRLoaderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	dry = *apvts.getRawParameterValue("DRY");
	wet = *apvts.getRawParameterValue("WET");

	ir.setParameters(dry, wet);
	ir.process(buffer);
}
//==============================================================================

bool IRLoaderAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* IRLoaderAudioProcessor::createEditor()
{
    return new IRLoaderAudioProcessorEditor (*this);
}

void IRLoaderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void IRLoaderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void IRLoaderAudioProcessor::loadImpulseResponse(const juce::File& file, const juce::dsp::Convolution::Stereo isStereo, const juce::dsp::Convolution::Trim needsTrim, int size)
{
	ir.loadImpulseResponse(file, isStereo, needsTrim, size);
}

juce::AudioProcessorValueTreeState::ParameterLayout IRLoaderAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY", "Dry", 0.0f, 100.0f, 100.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("WET", "Wet", 0.0f, 100.0f, 50.0f));
	return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IRLoaderAudioProcessor();
}