#pragma once

#include <JuceHeader.h>
#include "faustMinimalInlined.h"

class SpaceboxAudioProcessor : public juce::AudioProcessor
{
public:
    SpaceboxAudioProcessor();
    ~SpaceboxAudioProcessor() override;

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

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState apvts;

private:
    std::unique_ptr<mydsp> fDSP;
    std::unique_ptr<MapUI> fUI;

    std::vector<std::vector<float>> inputStorage;
    std::vector<std::vector<float>> outputStorage;
    std::vector<float*> inputs;
    std::vector<float*> outputs;

    std::atomic<float>* mixParam   = nullptr;
    std::atomic<float>* depthParam = nullptr;
    std::atomic<float>* rateParam  = nullptr;
    std::atomic<float>* gainParam  = nullptr;
    std::atomic<float>* themeParam = nullptr; // 0 = Red, 1 = Blue

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaceboxAudioProcessor)
};
