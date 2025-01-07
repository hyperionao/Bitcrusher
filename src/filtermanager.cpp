#include <filtermanager.h>
#include <lowpasscoefs.h>

std::vector<float> FilterManager::calculateLowPass(float cutoffFrequency, size_t filterLength) {
        return calculateLowPassCoefficients(cutoffFrequency, filterLength);
}

