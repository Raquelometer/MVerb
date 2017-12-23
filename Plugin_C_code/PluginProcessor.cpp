/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>



//==============================================================================
UmreverbAudioProcessor::UmreverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

UmreverbAudioProcessor::~UmreverbAudioProcessor()
{
}

//==============================================================================
const String UmreverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UmreverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UmreverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UmreverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UmreverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UmreverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UmreverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UmreverbAudioProcessor::setCurrentProgram (int index)
{
}

const String UmreverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void UmreverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void UmreverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
}

void UmreverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UmreverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


// This is where we would write our own convolution
void UmreverbAudioProcessor::OA_convolve(AudioSampleBuffer& buffer){
    // Calculate FFT of sample
    
    int window_size = 212;
    
    //AudioSampleBuffer& sample
    
    //do FFT, multiply and inverse FFT
    
    float* channelData = buffer.getWritePointer (0);
    for(int sample = 0; sample < buffer.getNumSamples(); ++sample){
        //
    }
    
    //
}

double UmreverbAudioProcessor::HPF (double inputSample)
{
    HPF_xn = inputSample;
    
    HPF_xn_1 = HPF_xz1;
    HPF_xn_2 = HPF_xz2;
    HPF_yn_1 = HPF_yz1;
    HPF_yn_2 = HPF_yz2;
    
    HPF_y = HPF_a0*HPF_xn + HPF_a1*HPF_xn_1 + HPF_a2*HPF_xn_2 - HPF_b1*HPF_yn_1 - HPF_b2*HPF_yn_2;
    
    HPF_xz2 = HPF_xz1;
    HPF_xz1 = HPF_xn;
    HPF_yz2 = HPF_yz1;
    HPF_yz1 = HPF_y;
    
    return HPF_y;
}

double UmreverbAudioProcessor::LPF (double inputSample)
{
    LPF_xn = inputSample;
    
    LPF_xn_1 = LPF_xz1;
    LPF_xn_2 = LPF_xz2;
    LPF_yn_1 = LPF_yz1;
    LPF_yn_2 = LPF_yz2;
    
    LPF_y = LPF_a0*LPF_xn + LPF_a1*LPF_xn_1 + LPF_a2*LPF_xn_2 - LPF_b1*LPF_yn_1 - LPF_b2*LPF_yn_2;
    
    LPF_xz2 = LPF_xz1;
    LPF_xz1 = LPF_xn;
    LPF_yz2 = LPF_yz1;
    LPF_yz1 = LPF_y;
    
    return LPF_y;
}

//void UmreverbAudioProcessor::readIR(const File & file){
    
//}

void UmreverbAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        float* channelData = buffer.getWritePointer (channel);
        
        // First HPF

        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] =  HPF(channelData[sample]);
            
        }
        
       // --- CONVOLUTION HAPPENS HERE --- //
        
        // Create an audioblock based on the current buffer audio
        juce::dsp::AudioBlock<float> convBlock = juce::dsp::AudioBlock<float>(buffer);
        
        // convolve with our loaded IR
        convolver.process(juce::dsp::ProcessContextReplacing<float>(convBlock));
        
        // Finish with LPF
       
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample){
             channelData[sample] = LPF(channelData[sample]);
        }
        
    }
}


//==============================================================================
bool UmreverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* UmreverbAudioProcessor::createEditor()
{
    return new UmreverbAudioProcessorEditor (*this);
}

//==============================================================================
void UmreverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void UmreverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UmreverbAudioProcessor();
}
