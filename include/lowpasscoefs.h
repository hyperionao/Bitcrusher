#ifndef LOWPASSCOEFS_H
#define LOWPASSCOEFS_H

#include <cstddef>
#include <vector>

std::vector<float> calculateLowPassCoefficients(
    float cutoffFrequency,  // 0.0 to 0.5, where 0.5 = Nyquist (half the sample rate)
    size_t filterLength     // # of filter coefficients (should be odd for symmetry!!)
);

#endif // LOWPASSCOEFS_H

