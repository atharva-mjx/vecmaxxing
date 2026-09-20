#pragma once
#include <any>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace vectorstore {

    struct Metadata {
    std::unordered_map<std::string, std::any> data;
};

template <typename T>
struct VectorRecord {
    int id = -1;
    std::vector<T> vector;
    Metadata metadata;

    size_t dimension() const {
        return vector.size();
    }
};

} 
