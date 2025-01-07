#include "Bitcrusher.h"
#include <iostream> 
#include <filemanager.h>


// Constructor
Bitcrusher::Bitcrusher(int bitDepth, int sampleRateReductionFactor, const std::string& inputPath, const std::string& outputPath)
    : bitDepth(bitDepth),
      sampleRateReductionFactor(sampleRateReductionFactor),
      inputFile(inputPath, SFM_READ), 
      outputFile(outputPath, SFM_WRITE, [&]() -> SF_INFO {
          SF_INFO sfinfo_in = inputFile.getSFInfo();
          SF_INFO sfinfo_out = sfinfo_in;
          sfinfo_out.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
          sfinfo_out.samplerate /= sampleRateReductionFactor; 
          return sfinfo_out; 
      }()) {

    // Validate parameters
    if (bitDepth <= 0) {
        throw std::invalid_argument("Bit depth must be a positive integer.");
    }
    if (sampleRateReductionFactor <= 0) {
        throw std::invalid_argument("Sample rate reduction factor must be a positive integer.");
    }

    std::cout << "Input and output files initialized successfully!" << std::endl;
}

// Run method
void Bitcrusher::run() {
    SF_INFO sfinfo = inputFile.getSFInfo();

    float originalSampleRate = sfinfo.samplerate;
    float newSampleRate = originalSampleRate / sampleRateReductionFactor;
    float cutoffFrequency = (newSampleRate / 2) / originalSampleRate; // Nyquist frequency

    size_t filterLength = 51; // Filter length
    auto coefficients = FilterManager::calculateLowPass(cutoffFrequency, filterLength);

    AudioProcessor::process(inputFile.getFile(), outputFile.getFile(), sampleRateReductionFactor, bitDepth, coefficients);

    std::cout << "Bitcrushing process completed successfully!" << std::endl;
}
