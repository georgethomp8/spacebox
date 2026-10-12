#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

namespace
{
    static juce::Font getSpaceFont (float height)
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor (
            BinaryData::EurostileBold_ttf,
            BinaryData::EurostileBold_ttfSize);

        return juce::Font (typeface).withHeight (height);
    }
}

class SpaceboxMixKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SpaceboxMixKnobLookAndFeel()
    {
        knobStrip = juce::ImageCache::getFromMemory (BinaryData::knob_filmstrip_png,
                                                     BinaryData::knob_filmstrip_pngSize);
    }

    void drawRotarySlider (juce::Graphics& g,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           juce::Slider& slider) override;

private:
    juce::Image knobStrip;
};

class SpaceboxAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                      private juce::Timer
{
public:
    explicit SpaceboxAudioProcessorEditor (SpaceboxAudioProcessor&);
    ~SpaceboxAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseUp (const juce::MouseEvent& event) override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    SpaceboxAudioProcessor& audioProcessor;
    SpaceboxMixKnobLookAndFeel knobLookAndFeel;

    juce::Slider mixSlider;
    juce::Slider depthSlider;
    juce::Slider speedSlider;
    juce::Slider gainSlider;

    juce::Label mixLabel;
    juce::Label depthLabel;
    juce::Label speedLabel;
    juce::Label gainLabel;

    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> speedAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;

    juce::Rectangle<int> titleBoxBounds;

    void setupKnob (juce::Slider& slider);
    void setupLabel (juce::Label& label, const juce::String& text);
    bool isRedTheme() const;
    void toggleThemeFromLogo();
    void timerCallback() override;

    juce::Image backgroundImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaceboxAudioProcessorEditor)
};
