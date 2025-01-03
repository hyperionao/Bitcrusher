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

// int main(){
//     //example
//     float x[5] = {1, 2, 3, 4, 5};
//     float c[3] = {0.1, 0.2, 0.3};
//     float y[5] = {0};

//     FilterInput input = {x, 5, c, 3, y, 5};
//     firFilter(input);

//     for (auto i = 0u; i < 5; ++i){
//         std::cout << y[i] << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }