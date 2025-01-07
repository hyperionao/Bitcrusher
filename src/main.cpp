#include "Bitcrusher.h"
#include <iostream>
#include <exception>

int main() {
    try {
        // Test parameters
        int bitDepth = 6; // Bit depth for quantization
        int sampleRateReductionFactor = 4; // Factor by which the sample rate is reduced
        std::string inputPath = "test.wav"; // Path to the input WAV file
        std::string outputPath = "output.wav"; // Path to the output WAV file

        // Create a Bitcrusher instance
        Bitcrusher bitcrusher(bitDepth, sampleRateReductionFactor, inputPath, outputPath);

        // Run the bitcrushing process
        bitcrusher.run();

        std::cout << "Test completed successfully! Output file: " << outputPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
    }

    return 0;
}
