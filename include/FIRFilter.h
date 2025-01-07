#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <cstddef>
#include <vector>

// Struct to represent filter input
struct FilterInput {
    const float* x;      // Input signal
    size_t inputLength;  // Length of the input signal
    const float* c;      // Filter coefficients
    size_t filterLength; // Number of filter coefficients
    float* y;            // Output signal
    size_t outputLength; // Length of the output signal
};

// Function prototype for FIR filter
void firFilter(FilterInput& input);
#endif // FIRFILTER_H
