%% Read in test audio

[testAudio, ~] = audioread(testAudioName);
%testAudio = testAudio(:, 1);

%% Record maximum volume 

volume = max(testAudio);

%% Convolve test audio with the IR

switch reverseMode
    case 'full'
        audioOut = flip(conv(flip(testAudio(:, 1)), IR));
    case 'windowed'
        windowTime = windowTimeMs / 1000 * 44100;
        audioOut = zeros(length(testAudio) + length(IR) - 1, 1);
        idx = 1;
        while idx < length(testAudio)
            temp = testAudio(idx:min(idx+windowTime, length(testAudio)));
            tempOut = flip(conv(flip(temp(:, 1)), IR));
            audioOut = audioOut + vertcat(zeros(idx, 1), tempOut, zeros(length(audioOut) - idx - length(tempOut), 1));
            idx = idx + windowTime;
        end
        audioOut = audioOut ./ max(audioOut) .* volume;
        audioOut = audioOut + vertcat(zeros(length(audioOut) - length(testAudio(:, 1)), 1), testAudio(:, 1));
    otherwise
        audioOut = conv(testAudio(:, 1), IR);
end

%% Set maximum volume to what it was before convolution

audioOut = audioOut ./ max(audioOut) .* volume;
