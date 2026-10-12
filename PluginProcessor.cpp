#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "faustMinimalInlined.h"

//==============================================================================
SpaceboxAudioProcessor::SpaceboxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
    mixParam   = apvts.getRawParameterValue ("mix");
    depthParam = apvts.getRawParameterValue ("lfo_depth_scale");
    rateParam  = apvts.getRawParameterValue ("lfo_rate_scale");
    gainParam  = apvts.getRawParameterValue ("output_gain");
    themeParam = apvts.getRawParameterValue ("logo_theme");
}

SpaceboxAudioProcessor::~SpaceboxAudioProcessor() = default;

//==============================================================================
const juce::String SpaceboxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpaceboxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpaceboxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpaceboxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpaceboxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpaceboxAudioProcessor::getNumPrograms()
{
    return 1;
}

int SpaceboxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpaceboxAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String SpaceboxAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void SpaceboxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout SpaceboxAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "mix", 1 },
        "Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f),
        0.50f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "lfo_depth_scale", 1 },
        "Depth",
        juce::NormalisableRange<float> (0.0f, 4.0f, 0.001f),
        1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "lfo_rate_scale", 1 },
        "Speed",
        juce::NormalisableRange<float> (0.0f, 4.0f, 0.001f,0.5f),
        1.0f));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "output_gain", 1 },
        "Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f),
        0.0f));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "logo_theme", 1 },
        "Theme",
        juce::StringArray { "Red", "Blue" },
        0)); // default = Red

    return { params.begin(), params.end() };
}

//==============================================================================
void SpaceboxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fDSP = std::make_unique<mydsp>();
    fUI  = std::make_unique<MapUI>();

    fDSP->init ((int) sampleRate);
    fDSP->buildUserInterface (fUI.get());

    const int numIn  = juce::jmax (1, getTotalNumInputChannels());
    const int numOut = juce::jmax (1, getTotalNumOutputChannels());

    inputStorage.assign  ((size_t) numIn,  std::vector<float> ((size_t) samplesPerBlock, 0.0f));
    outputStorage.assign ((size_t) numOut, std::vector<float> ((size_t) samplesPerBlock, 0.0f));

    inputs.resize  ((size_t) numIn);
    outputs.resize ((size_t) numOut);

    for (int ch = 0; ch < numIn; ++ch)
        inputs[(size_t) ch] = inputStorage[(size_t) ch].data();

    for (int ch = 0; ch < numOut; ++ch)
        outputs[(size_t) ch] = outputStorage[(size_t) ch].data();

    // initialize Faust params once at startup
    if (fUI != nullptr)
    {
        fUI->setParamValue ("mix", *mixParam);
        fUI->setParamValue ("lfo_depth_scale", *depthParam);
        fUI->setParamValue ("lfo_rate_scale", *rateParam);
        fUI->setParamValue ("output_gain", *gainParam);
        fUI->setParamValue ("bypass", 0.0f);

        const bool isRedTheme = ((int) themeParam->load() == 0);
        fUI->setParamValue ("mode", isRedTheme ? 6.0f : 1.0f);
    }
}

void SpaceboxAudioProcessor::releaseResources()
{
    fDSP.reset();
    fUI.reset();

    inputStorage.clear();
    outputStorage.clear();
    inputs.clear();
    outputs.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpaceboxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SpaceboxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples             = buffer.getNumSamples();

    if (fUI != nullptr)
    {
        fUI->setParamValue ("mix", *mixParam);
        fUI->setParamValue ("lfo_depth_scale", *depthParam);
        fUI->setParamValue ("lfo_rate_scale", *rateParam);
        fUI->setParamValue ("output_gain", *gainParam);
        fUI->setParamValue ("bypass", 0.0f);

        const bool isRedTheme = ((int) themeParam->load() == 0);
        fUI->setParamValue ("mode", isRedTheme ? 6.0f : 1.0f);
    }

    const int numIn  = juce::jmin (totalNumInputChannels,  (int) inputs.size());
    const int numOut = juce::jmin (totalNumOutputChannels, (int) outputs.size());

    for (int ch = 0; ch < numIn; ++ch)
        juce::FloatVectorOperations::copy (inputs[(size_t) ch], buffer.getReadPointer (ch), numSamples);

    if (fDSP != nullptr)
        fDSP->compute (numSamples, inputs.data(), outputs.data());

    for (int ch = 0; ch < numOut; ++ch)
        juce::FloatVectorOperations::copy (buffer.getWritePointer (ch), outputs[(size_t) ch], numSamples);

    for (int ch = numOut; ch < totalNumOutputChannels; ++ch)
        buffer.clear (ch, 0, numSamples);
}

//==============================================================================
bool SpaceboxAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SpaceboxAudioProcessor::createEditor()
{
    return new SpaceboxAudioProcessorEditor (*this);
}

//==============================================================================
void SpaceboxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void SpaceboxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpaceboxAudioProcessor();
}
