#include "AudioProcessor.h"
#include <vector>

void AudioProcessor::process(SNDFILE* input, SNDFILE* output, int sampleRateReductionFactor, int bitDepth, const std::vector<float>& coefficients) {
    size_t chunkFrames = 1024; // Number of frames to process at a time
    SF_INFO sfinfo;
    sf_command(input, SFC_GET_CURRENT_SF_INFO, &sfinfo, sizeof(SF_INFO));

    size_t inputBufferSize = chunkFrames * sfinfo.channels;
    size_t outputBufferSize = inputBufferSize / sampleRateReductionFactor;

    std::vector<float> inputBuffer(inputBufferSize);
    std::vector<short> outputBuffer(outputBufferSize);

    FilterInput filterInput;

    process_audio_chunks(input, output, inputBuffer.data(), outputBuffer.data(),
                         chunkFrames, sampleRateReductionFactor, bitDepth, sfinfo.channels, filterInput, const_cast<std::vector<float>&>(coefficients));
}
