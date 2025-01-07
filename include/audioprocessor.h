#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <sndfile.h>
#include <vector>
#include <firfilter.h>
#include <processaudiochunks.h>

class AudioProcessor {
public:
    static void process(SNDFILE* input, SNDFILE* output, int sampleRateReductionFactor, int bitDepth, const std::vector<float>& coefficients);
};

#endif // AUDIOPROCESSOR_H
