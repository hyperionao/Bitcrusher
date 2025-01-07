#include <FIRFilter.h>
#include <stdexcept> 

void firFilter(FilterInput& input) {
    if (input.inputLength < input.filterLength) {
        throw std::invalid_argument("Input length must be greater than or equal to filter length.");
    }

    for (size_t i = 0; i < input.outputLength; ++i) {
        input.y[i] = 0.0f; 
        for (size_t j = 0; j < input.filterLength; ++j) {
            input.y[i] += input.x[i + j] * input.c[j]; // Convolution
        }
    }
}
