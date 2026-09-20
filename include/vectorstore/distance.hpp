#pragma once
#include <cstddef>
#include <vector>

namespace vectorstore {
    
inline float squared_l2(const std::vector<float>& a, const std::vector<float>& b) {
    float dist = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        float diff = a[i] - b[i];
        dist += diff * diff;
    }
    return dist;
}

}  
