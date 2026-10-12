#include "PluginProcessor.h"
#include "PluginEditor.h"

#define JUCE_DISPLAY_SPLASH_SCREEN 0

//==============================================================================
void SpaceboxMixKnobLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                                   int x, int y, int width, int height,
                                                   float sliderPosProportional,
                                                   float /*rotaryStartAngle*/,
                                                   float /*rotaryEndAngle*/,
                                                   juce::Slider& /*slider*/)
{
    if (knobStrip.isNull())
        return;

    // Frames are square and stacked vertically.
    const int frameSize = knobStrip.getWidth();
    const int numFrames = knobStrip.getHeight() / frameSize;

    if (numFrames <= 0)
        return;

    const int frameIndex = juce::jlimit (0, numFrames - 1,
        (int) std::round (sliderPosProportional * (numFrames - 1)));

    // Draw the selected frame into the slider's bounds.
    g.drawImage (knobStrip,
                 x, y, width, height,                  // destination
                 0, frameIndex * frameSize,            // source x, y
                 frameSize, frameSize);                // source w, h
}
//==============================================================================
SpaceboxAudioProcessorEditor::SpaceboxAudioProcessorEditor (SpaceboxAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const float scale = 0.65f;   // 65% of original
//setSize ((int) (668 * scale), (int) (918 * scale));
setSize (320, 440);
    setupKnob (mixSlider);
    setupKnob (depthSlider);
    setupKnob (speedSlider);
    setupKnob (gainSlider);

    setupLabel (mixLabel,   "MIX");
    setupLabel (depthLabel, "Depth");
    setupLabel (speedLabel, "Speed");
    setupLabel (gainLabel,  "Gain");

    mixAttachment   = std::make_unique<SliderAttachment> (audioProcessor.apvts, "mix", mixSlider);
    depthAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, "lfo_depth_scale", depthSlider);
    speedAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, "lfo_rate_scale", speedSlider);
    gainAttachment  = std::make_unique<SliderAttachment> (audioProcessor.apvts, "output_gain", gainSlider);

    startTimerHz (20);
    
     backgroundImage = juce::ImageCache::getFromMemory (BinaryData::background_image_png,
                                                        BinaryData::background_image_pngSize);
}

SpaceboxAudioProcessorEditor::~SpaceboxAudioProcessorEditor()
{
    mixSlider.setLookAndFeel (nullptr);
    depthSlider.setLookAndFeel (nullptr);
    speedSlider.setLookAndFeel (nullptr);
    gainSlider.setLookAndFeel (nullptr);
}

//==============================================================================
void SpaceboxAudioProcessorEditor::setupKnob (juce::Slider& slider)
{
    addAndMakeVisible (slider);
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setPopupDisplayEnabled (false, false, this);
    slider.setLookAndFeel (&knobLookAndFeel);
}

void SpaceboxAudioProcessorEditor::setupLabel (juce::Label& label, const juce::String& text)
{
    addAndMakeVisible (label);
    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
    label.setFont (getSpaceFont (16.0f));
}

bool SpaceboxAudioProcessorEditor::isRedTheme() const
{
    if (auto* raw = audioProcessor.apvts.getRawParameterValue ("logo_theme"))
        return ((int) raw->load() == 0);

    return true;
}

void SpaceboxAudioProcessorEditor::toggleThemeFromLogo()
{
    auto* param = audioProcessor.apvts.getParameter ("logo_theme");
    auto* raw   = audioProcessor.apvts.getRawParameterValue ("logo_theme");

    if (param == nullptr || raw == nullptr)
        return;

    const int currentIndex = (int) raw->load();
    const int nextIndex    = (currentIndex == 0) ? 1 : 0;

    param->beginChangeGesture();
    param->setValueNotifyingHost (param->convertTo0to1 ((float) nextIndex));
    param->endChangeGesture();

    repaint();
}

void SpaceboxAudioProcessorEditor::timerCallback()
{
    repaint();
}

//==============================================================================
void SpaceboxAudioProcessorEditor::paint (juce::Graphics& g)
{
   g.drawImage (backgroundImage, getLocalBounds().toFloat());
   // g.drawRect(titleBoxBounds,2);
}

void SpaceboxAudioProcessorEditor::resized()
{
    const float scaleX = getWidth()  / 668.0f;
    const float scaleY = getHeight() / 918.0f;

    auto place = [&] (juce::Slider& s, int centerX, int centerY, int sizePx)
    {
        const int w = (int) (sizePx * scaleX);
        const int h = (int) (sizePx * scaleY);
        s.setBounds ((int) (centerX * scaleX) - w / 2,
                     (int) (centerY * scaleY) - h / 2,
                     w, h);
    };

    place (mixSlider,   215, 160, 250);   // )
    place (gainSlider,  453, 160, 250);   // -215)
    place (speedSlider, 215, 416, 250);   //
    place (depthSlider, 453, 416, 250);   // 
    
    titleBoxBounds = juce::Rectangle<int> (124, 275, 70, 40);
    
}
void SpaceboxAudioProcessorEditor::mouseUp (const juce::MouseEvent& event)
{
    if (titleBoxBounds.contains (event.getPosition()))
    {
        toggleThemeFromLogo();
        return;
    }

    AudioProcessorEditor::mouseUp (event);
}
