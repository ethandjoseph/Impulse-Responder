#pragma once
#include <JuceHeader.h>

class ImpulseResponder
{

public:
    ImpulseResponder() {};

    ~ImpulseResponder() {};

    void reset();

    void loadImpulseResponse(const juce::File& file, const juce::dsp::Convolution::Stereo isStereo, const juce::dsp::Convolution::Trim needsTrim, int size);
	//loadButton.onClick = [this]
	//	{
	//		fileChooser = std::make_unique<juce::FileChooser>("Select .wav IR file to load", audioProcessor.root, "*.wav");
	//		const auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
	//		fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& fc)
	//			{
	//				auto file = fc.getResult();
	//				if (file.exists())
	//				{
	//					audioProcessor.savedFile = file;
	//					audioProcessor.root = file.getParentDirectory().getFullPathName();
	//					//audioProcessor.variableTree.setProperty("IR", file.getFullPathName(), nullptr);
	//					//audioProcessor.variableTree.setProperty("root", file.getParentDirectory().getFullPathName(), nullptr);
	//					audioProcessor.ir.reset();
	//					audioProcessor.ir.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
	//					irName.setText(file.getFileName().substring(0, 6), juce::dontSendNotification);
	//					irName.setJustificationType(juce::Justification::centred);
	//				}
	//			});
	//	};

    void prepare(juce::dsp::ProcessSpec spec);

    void setParameters(float newDry, float newWet);

    void process(juce::AudioBuffer<float>& buffer);

private:
	juce::dsp::Convolution conv;
    juce::AudioBuffer<float> dryBuffer;
    juce::AudioBuffer<float> wetBuffer;
    float dry = 100.0;
    float wet = 50.0;
};