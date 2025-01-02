#include <iostream>

//file to implement the FIR filter by convolution

struct FilterInput {
    const float* x; //input signal
    size_t inputLength; // its length
    const float* c; //reversed filter coefficients
    size_t filterLength; // its length
    float* y; //output signal
    size_t outputLength; // its length, should be same as inputLength
};

float* firFilter(FilterInput& input){
    const auto* x = input.x;
    const auto* c = input.c;
    auto* y = input.y;

    for (auto i = 0u; i < input.outputLength; ++i){
        y[i] = 0.f;
        for (auto j = 0u; j < input.filterLength; ++j){
            y[i] += x[i + j] * c[j];
        }
    }
    return y;
}