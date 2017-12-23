% Script that applies a custom convolution reverb to any sound file
% Spaces in Impulse_responses folder are all from the University of
% Michigan

% Set window size
B = 500;

% load IR
filename = input('Please enter audio file name: ');
ir_audio = input('Please enter the IR file name: ');
[h, fs] = audioread(ir_audio);

% use only mono
h = h(:,1);
N = length(h);
% trim IR to closest lower power of 2
cut = pow2(floor(log2(N + 500)));
cut = cut - B;
h = h(1:cut);

% load sample audio, mono
[x, fs] = audioread(filename);
x = x(:,1);

% HPF input audio
x = second_order_HPF(x, fs, 50);
plot_sound(x, fs, 3);
title('HP filtered signal at 50 Hz');
% set variables

M = length(x);

% find optimal K value for FFT

p = nextpow2(B + N - 1);
p = p-1;
K = pow2(p);

% calculate and add zero padding 

Pad_length_B = abs(K - B);
Pad_length_h = abs(K - N);

Pad_B = zeros(1,Pad_length_B);
Pad_h = zeros(1,Pad_length_h);

h_padded = [h' Pad_h];

% calculate FFT zero padded IR

FFT_h_padded = fft(h_padded);

% create output buffer
y_buffer = zeros(1, 2*(N + M - 1));

i = 1;

% Enter while loop: run algorithm while you're still looking at signal
% in a realtime situation, condition loop based on whether reverb is on/off

tic
while(i < M)
    
    % select input block and pad
    
    segment_end_x = min(i + B - 1, M);
    x_curr = x(i:segment_end_x);
    x_curr_pad = [x_curr' Pad_B];
    
    % compute FFT
    x_curr_FFT = fft(x_curr_pad);
    
    % compute Y(x) = H(x)X(x)
    Y_i = FFT_h_padded(1:length(x_curr_pad)) .* x_curr_FFT;
    
    % do inverse fft, discard extra imaginary parts that MATLAB adds
    y_i = real(ifft(Y_i));
    
    % find end index
    segment_end_y = min(i + K - 1, N + M - 1);
   
    y_i_end = segment_end_y - i + 1;
    
    if y_i_end > length(y_i)
        break
    end
    y_buffer(i:segment_end_y) = y_buffer(i:segment_end_y) + y_i(1:y_i_end);
    
    i = i + B;
end
toc

% You can comment/uncomment this convolution to compare runtimes
tic
%conv(x,h);
toc

y_buffer_scaled = rescale_sound(y_buffer', 1);
plot_sound(y_buffer_scaled, fs, 2);
title('Audio signal after convolution')

%LPF y_buffer_scaled
y_filtered_reverb = second_order_LPF(y_buffer_scaled, fs, 17000);

%wet/dry mix

sound(y_filtered_reverb, fs);

