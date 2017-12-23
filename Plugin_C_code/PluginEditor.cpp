/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Images.h"


//==============================================================================
UmreverbAudioProcessorEditor::UmreverbAudioProcessorEditor (UmreverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    backgroundImage = ImageCache::getFromMemory(Images::backgroundd_png, Images::backgroundd_pngSize);
    
    ///Set size of GUI
    setSize (750, 422);
    
    addAndMakeVisible(reverse);
    
    // these define the parameters of our slider object
    Volume.setSliderStyle (Slider::Rotary);
    Volume.setRange(0, 1000, 1);
    Volume.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    Volume.setPopupDisplayEnabled (true, false, this);
    Volume.setTextValueSuffix (" ms");
    Volume.setValue(0);
    //Volume.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible (&Volume);
    Volume.setLookAndFeel(&l2);
    
    // these define the parameters of our slider object
    HPF_Cutoff_Slider.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    HPF_Cutoff_Slider.setRange(20, 20000, 1);
    HPF_Cutoff_Slider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    HPF_Cutoff_Slider.setPopupDisplayEnabled (true, false, this);
    HPF_Cutoff_Slider.setTextValueSuffix (" Hz");
    HPF_Cutoff_Slider.setValue(20);
    HPF_Cutoff_Slider.setSkewFactorFromMidPoint (3000);
    HPF_Cutoff_Slider.addListener(this);
    // this function adds the slider to the editor
    HPF_Cutoff_Slider.setLookAndFeel(&l2);
    addAndMakeVisible (&HPF_Cutoff_Slider);
    
    // these define the parameters of our slider object
    LPF_Cutoff_Slider.setSliderStyle (Slider::RotaryVerticalDrag);
    LPF_Cutoff_Slider.setRange(20, 20000, 1);
    LPF_Cutoff_Slider.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    LPF_Cutoff_Slider.setPopupDisplayEnabled (true, false, this);
    LPF_Cutoff_Slider.setTextValueSuffix (" Hz");
    LPF_Cutoff_Slider.setValue(20000);
    LPF_Cutoff_Slider.setSkewFactorFromMidPoint (3000);
    LPF_Cutoff_Slider.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible (&LPF_Cutoff_Slider);
    LPF_Cutoff_Slider.setLookAndFeel(&l2);
    
    // these define the parameters of our slider object
    Wet_Dry.setSliderStyle (Slider::RotaryVerticalDrag);
    Wet_Dry.setRange(0, 100, 1);
    Wet_Dry.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    Wet_Dry.setPopupDisplayEnabled (true, false, this);
    Wet_Dry.setTextValueSuffix (" % Dry");
    Wet_Dry.setValue(0);
    Wet_Dry.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible (&Wet_Dry);
    l2.setColour(Slider::rotarySliderFillColourId, Colours::yellow);
    Wet_Dry.setLookAndFeel(&l2);
    
    // Added button for IR and convolution loading
    LoadIRButton.addListener(this);
    addAndMakeVisible(LoadIRButton);
    formatManager.registerBasicFormats();

}

UmreverbAudioProcessorEditor::~UmreverbAudioProcessorEditor()
{
}

//==============================================================================
void UmreverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.drawImage(backgroundImage, 0, 0, 750, 422, 0, 0, 750, 422);
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("UMVerb " + std::to_string(processor.bufferSize), getLocalBounds(), Justification::centred, 1);
    
    //g.drawFittedText ("UMVerb " + std::to_string(processor.readerIR->lengthInSamples), getLocalBounds(), Justification::centred, 1);
}

void UmreverbAudioProcessorEditor::resized()
{
    // sets the position and size of the slider with arguments (x, y, width, height)
    Volume.setBounds (80, 258, 75, 75);
    HPF_Cutoff_Slider.setBounds (243, 240, 90, 90);
    Wet_Dry.setBounds (600, 258, 75, 75);
    LPF_Cutoff_Slider.setBounds (450, 240, 90, 90);
    
    //LoadIRButton.setBounds(350, 250, 90, 90);
    
    reverse.setBounds(381, 257, 30, 30);
}

// Button that when clicked, loads IR and prepares convolution

void UmreverbAudioProcessorEditor::LoadIRButtonClicked() {
    
    // Change this to your direct filepath
    File loadedIR = File("/Users/aboudeshehadeh/Documents/UMReverb/IR_HillAuditorium_FF.wav");
    currentIR = loadedIR;
    processor.readerIR = formatManager.createReaderFor(currentIR);
    
    
    //const File file (chooser.getResult());
    //ScopedPointer<AudioFormatReader> reader (formatManager.createReaderFor (loadedIR)); // [2]
    
    if (processor.readerIR != nullptr)
    {
        // --- Code for reading an audio file into a buffer. Not used currently
        
        processor.IRbuffer.setSize (processor.readerIR->numChannels, processor.readerIR->lengthInSamples);
        processor.readerIR->read (&processor.IRbuffer, 0,
                          processor.readerIR->lengthInSamples, 0, true, true);
        
    
       // Code for doing convolution ourselves with FFT
        /*
         float* IR_pointer = processor.IRbuffer.getWritePointer(0);
        
        dsp::FFT IRFFT(19);
        IRFFT.performRealOnlyForwardTransform(IR_pointer);
         
         */
        
       
        // Set up convolver. The process Spec just lets the convolver know what to expect
        dsp::ProcessSpec convSpec;
        convSpec.sampleRate = 441000;
        convSpec.maximumBlockSize = 50000;
        convSpec.numChannels = 1;
        // Prepare the convolver and load IR
        processor.convolver.prepare(convSpec);
        processor.convolver.loadImpulseResponse (loadedIR, false, true, loadedIR.getSize());
        
        
    }
    
    
    
    
    
}
void UmreverbAudioProcessorEditor::buttonClicked (Button* button)
{
    std::cout << "click" << std::endl;
    if (button == &LoadIRButton )  LoadIRButtonClicked();
    //processor.bufferSize = processor.readerIR->lengthInSamples;
    
    
    
}

///This is where you would specify what each slider does
///NOTE: processor is a given object of class PluginProcessor to access all its variables
void UmreverbAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if(slider == &Volume)
    {
        processor.volumeValues = Volume.getValue();
    }
    
    else if(slider == &HPF_Cutoff_Slider)
    {
        processor.HPF_Cutoff_Value = HPF_Cutoff_Slider.getValue();
        
        processor.HPF_C = tan(PI*processor.HPF_Cutoff_Value/processor.currentSampleRate);
        processor.HPF_a0 = 1/(1+ 0.707*processor.HPF_C + processor.HPF_C*processor.HPF_C);
        processor.HPF_a1 = -2*processor.HPF_a0;
        processor.HPF_a2 = processor.HPF_a0;
        processor.HPF_b1 = 2*processor.HPF_a0*(-1 + processor.HPF_C*processor.HPF_C);
        processor.HPF_b2 = processor.HPF_a0*(1-0.707*processor.HPF_C + processor.HPF_C*processor.HPF_C);
    }
    
    else if(slider == &LPF_Cutoff_Slider)
    {
        processor.LPF_Cutoff_Value = LPF_Cutoff_Slider.getValue();
        
        processor.LPF_C = 1/tan(PI*processor.LPF_Cutoff_Value/processor.currentSampleRate);
        processor.LPF_a0 = 1/(1+ 0.707*processor.LPF_C + processor.LPF_C*processor.LPF_C);
        processor.LPF_a1 = 2*processor.LPF_a0;
        processor.LPF_a2 = processor.LPF_a0;
        processor.LPF_b1 = 2*processor.LPF_a0*(1 - processor.LPF_C*processor.LPF_C);
        processor.LPF_b2 = processor.LPF_a0*(1-0.707*processor.LPF_C + processor.LPF_C*processor.LPF_C);
    }
}
















