#ifndef BITCRUSHER_H
#define BITCRUSHER_H

#include <string>
#include "FileManager.h"
#include "FilterManager.h"
#include "AudioProcessor.h"

class Bitcrusher {
    int bitDepth;
    int sampleRateReductionFactor;
    FileManager inputFile;
    FileManager outputFile;

public:
    Bitcrusher(int bitDepth, int sampleRateReductionFactor, const std::string& inputPath, const std::string& outputPath);
    void run();
};

#endif // BITCRUSHER_H
