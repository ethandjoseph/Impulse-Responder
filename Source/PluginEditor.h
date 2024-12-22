#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class IRLoaderAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IRLoaderAudioProcessorEditor (IRLoaderAudioProcessor&);
    ~IRLoaderAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	std::unique_ptr<juce::FileChooser> fileChooser;
	juce::TextButton loadButton;
	juce::Slider drySlider;
	juce::Slider wetSlider;
	juce::Label irName;
	juce::Label drySliderLabel;
	juce::Label wetSliderLabel;
    IRLoaderAudioProcessor& audioProcessor;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> drySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLoaderAudioProcessorEditor)
};