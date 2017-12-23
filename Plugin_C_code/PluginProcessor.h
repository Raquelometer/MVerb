/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define PI 3.14159265

//==============================================================================
/**
*/
class UmreverbAudioProcessor  : public AudioProcessor
{
public:
    
    //==============================================================================
    UmreverbAudioProcessor();
    ~UmreverbAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    ///////Here I made the variables needed for the HPF & LPF
    ////These 3 are values we read from the Sliders
    double volumeValues;
    double LPF_Cutoff_Value;
    double HPF_Cutoff_Value;
    
    double currentSampleRate = 44100;
    
    ///HPF coeff
    double HPF_C = 0;
    double HPF_a0 = 1;
    double HPF_a1 = -2;
    double HPF_a2 = 1;
    double HPF_b1 = -2;
    double HPF_b2 = 1;
    ///HPF delays
    double HPF_xz1 = 0.0;
    double HPF_xz2 = 0.0;
    double HPF_yz1 = 0.0;
    double HPF_yz2 = 0.0;
    ///HPF I/O
    double HPF_xn = 0.0;
    double HPF_xn_1 = 0.0;
    double HPF_xn_2 = 0.0;
    double HPF_y = 0.0;
    double HPF_yn_1 = 0.0;
    double HPF_yn_2 = 0.0;
    
    ///LPF ceoff
    double LPF_C = 1/tan(PI*20000/currentSampleRate);
    double LPF_a0 = 1/(1+ 0.707*LPF_C + LPF_C*LPF_C);
    double LPF_a1 = 2*LPF_a0;
    double LPF_a2 = LPF_a0;
    double LPF_b1 = 2*LPF_a0*(1 - LPF_C*LPF_C);
    double LPF_b2 = LPF_a0*(1-0.707*LPF_C + LPF_C*LPF_C);
    ///LPF delays & I/O
    double LPF_xz1 = 0.0;
    double LPF_xz2 = 0.0;
    double LPF_yz1 = 0.0;
    double LPF_yz2 = 0.0;
    ///HPF I/O
    double LPF_xn = 0.0;
    double LPF_xn_1 = 0.0;
    double LPF_xn_2 = 0.0;
    double LPF_y = 0.0;
    double LPF_yn_1 = 0.0;
    double LPF_yn_2 = 0.0;
    
    double bufferSize = 0;
    
    ///Functions needed
    double HPF (double inputSample);
    double LPF (double inputSample);
    
    
    /// Variables and functions for Convolution
    File currentIR;
    dsp::Convolution convolver;
    AudioFormatReader* readerIR;
    AudioSampleBuffer IRbuffer;
    void OA_convolve(AudioSampleBuffer& buffer);
    
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UmreverbAudioProcessor)
};
