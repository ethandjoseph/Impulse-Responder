#include "ImpulseResponder.h"

void ImpulseResponder::reset()
{
	conv.reset();
}

void ImpulseResponder::loadImpulseResponse(const juce::File& file, const juce::dsp::Convolution::Stereo isStereo, const juce::dsp::Convolution::Trim needsTrim, int size)
{
	conv.loadImpulseResponse(file, isStereo, needsTrim, size);
}

void ImpulseResponder::prepare(juce::dsp::ProcessSpec spec)
{
	conv.reset();
	conv.prepare(spec);
	dryBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
	wetBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
}

void ImpulseResponder::setParameters(float newDry, float newWet)
{	
	dry = newDry;
	wet = newWet;
}

void ImpulseResponder::process(juce::AudioBuffer<float>& buffer)
{
	dryBuffer = buffer;
	wetBuffer = buffer;
	buffer.clear();

	juce::dsp::AudioBlock<float> audioBlock (wetBuffer);
	juce::dsp::ProcessContextReplacing<float> context(audioBlock);
	if (conv.getCurrentIRSize() > 0)
	{
		conv.process(context);
	}

	dryBuffer.applyGain(0, buffer.getNumSamples(), dry / 100.0f);
	wetBuffer.applyGain(0, buffer.getNumSamples(), wet / 100.0f);

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.addFrom(channel, 0, dryBuffer, channel, 0, buffer.getNumSamples());
		buffer.addFrom(channel, 0, wetBuffer, channel, 0, buffer.getNumSamples());
	}
}