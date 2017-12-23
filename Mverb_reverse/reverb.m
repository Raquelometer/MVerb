%% Parameters

% Test audio filename
testAudioName = 'TestAudio.wav';

% Original sine sweep filename
originalSineSweepName = '3s_sweep_20Hz_20000Hz_-1dBFS.wav';

% Measured sine sweep filename
measuredSineSweepName = 'IR_HillAuditorium_BF2_Sweep_3s.wav';

% Reverse mode -- EITHER 'full', 'windowed', or 'off'
reverseMode = 'windowed';

% Window time in milliseconds
windowTimeMs = 230;

%% Processing
disp('Getting Impulse Response....');
getIR;
disp('Performing Convolution....');
convolveWithAudio;
disp('Preparing to play sound....');
sound(audioOut, 44100);
audioOut = rescale_sound(audioOut, 2);
audiowrite(['reverberated_', testAudioName], audioOut, 44100);
plot_sound(audioOut, 44100, 1);
title('Sound signal with applied Reverse Reverb');
