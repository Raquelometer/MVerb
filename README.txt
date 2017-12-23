README

This repository includes includes the main script, apply_MVerb.m, and various functions, such as the HPF and LPF.

There are also two subdirectories:

Impulse_responses. Contains the IRS that we collected 

MVerb_reverse. Contains code for generating an IR from sine sweeps, and our script for the Reverse windowed reverb. 

To run the standard reverb:

Open the Mverb_MATLAB folder in MATLAB. Click on the script entitled ‘apply_MVERB.m’ Run this script. You will be prompted to enter an audio file, and then the IR. 
For the Audio, enter ‘TestAudio.wav’
For the IR, choose a desired IR from the Impulse_responses folder and enter ‘Impulse_responses/<some_IR>.wav’

If you want to compare our runtime with MATLAB’s convolution, uncomment line 95.


To run the reverse reverb:

Open the MVerb_reverse directory. For this example, the test audio and IR have been hardcoded in for you, so you just have click on ‘reverb.m.’ Then, go to line 12, select which mode you want (no reverse, full, or windowed), and run the script. This script uses the MATLAB built in convolution, so it takes some time to process. The sound plays a few seconds after the script has finished running. 

Filter Design and script: Aboud Shehadeh
Convolution algorithm: Daphna Raz
Reverse reverb: Jack Nonnenmacher
IR collection: Mitchell Graham



