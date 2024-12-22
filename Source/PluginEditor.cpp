#include "PluginProcessor.h"
#include "PluginEditor.h"

IRLoaderAudioProcessorEditor::IRLoaderAudioProcessorEditor (IRLoaderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
	loadButton.setButtonText("Load IR");
	loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(84, 58, 20));
	loadButton.onClick = [this]
		{
			fileChooser = std::make_unique<juce::FileChooser>("Select .wav IR file to load",audioProcessor.root, "*.wav");
			const auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
			fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& fc)
				{
					auto file = fc.getResult();
					if (file.exists())
					{
						audioProcessor.savedFile = file;
						audioProcessor.root = file.getParentDirectory().getFullPathName();
						//audioProcessor.variableTree.setProperty("IR", file.getFullPathName(), nullptr);
						//audioProcessor.variableTree.setProperty("root", file.getParentDirectory().getFullPathName(), nullptr);
						audioProcessor.ir.reset();
						audioProcessor.ir.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
						irName.setText(file.getFileName().substring(0, 6), juce::dontSendNotification);
						irName.setJustificationType(juce::Justification::centred);
					}
				});
		};

    drySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
	wetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

	drySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
	wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);

	drySlider.setTextValueSuffix(" %");
	wetSlider.setTextValueSuffix(" %");

	drySliderLabel.setText("Dry", juce::dontSendNotification);
	wetSliderLabel.setText("Wet", juce::dontSendNotification);

	drySliderLabel.attachToComponent(&drySlider, true);
	wetSliderLabel.attachToComponent(&wetSlider, true);

	addAndMakeVisible(loadButton);
	addAndMakeVisible(drySlider);
	addAndMakeVisible(wetSlider);
	addAndMakeVisible(drySliderLabel);
	addAndMakeVisible(wetSliderLabel);
	addAndMakeVisible(irName);

	drySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRY", drySlider);
	wetSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WET", wetSlider);

    setSize (440, 200);
}

IRLoaderAudioProcessorEditor::~IRLoaderAudioProcessorEditor()
{
}

void IRLoaderAudioProcessorEditor::paint (juce::Graphics& g)
{
	g.fillAll(juce::Colour::fromRGB(84, 58, 20));
}

void IRLoaderAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(20);
	int sliderWidth = area.getWidth() / 3;
	int sliderHeight = area.getHeight() - 20;
	loadButton.setBounds(area.removeFromLeft(sliderWidth).reduced(20));
	irName.setBounds(loadButton.getBounds().removeFromBottom(32));
	drySlider.setBounds(area.removeFromLeft(sliderWidth).reduced(20));
	wetSlider.setBounds(area.removeFromLeft(sliderWidth).reduced(20));
}