#include "Bitcrusher.h"
#include <iostream>
#include <exception>

int main() {
    try {
        // Test parameters
        int bitDepth = 6; // Bit depth for quantization
        int sampleRateReductionFactor = 4; // Factor by which the sample rate is reduced
        std::string inputPath = "C:/Users/krono/Desktop/Repos/Bitcrusher/Bitcrusher/test.wav"; // Full path to input
        std::string outputPath = "C:/Users/krono/Desktop/Repos/Bitcrusher/Bitcrusher/output.wav"; // Full path to output


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
