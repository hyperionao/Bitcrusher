#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <vector>
#include <cstddef> 

class FilterManager {
public:
    static std::vector<float> calculateLowPass(float cutoffFrequency, size_t filterLength);
};

#endif // FILTERMANAGER_H
