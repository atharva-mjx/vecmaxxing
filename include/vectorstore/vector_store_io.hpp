#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "vectorstore/vector_record.hpp"

namespace vectorstore {

class VectorStoreIO {
public:
    template <typename T>
    static std::vector<VectorRecord<T>> read_vecs(const std::string& file_path) {
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("VectorStoreIO: failed to open file: " + file_path);
        }

        std::vector<VectorRecord<T>> records;
        int32_t dim = 0;
        int next_id = 0;

        while (file.read(reinterpret_cast<char*>(&dim), sizeof(dim))) {
            VectorRecord<T> record;
            record.id = next_id++;
            record.vector.resize(static_cast<size_t>(dim));

            std::streamsize bytes_to_read =
                static_cast<std::streamsize>(dim) * static_cast<std::streamsize>(sizeof(T));
            if (!file.read(reinterpret_cast<char*>(record.vector.data()), bytes_to_read)) {
                throw std::runtime_error(
                    "VectorStoreIO: unexpected EOF while reading vector body in file: " +
                    file_path);
            }

            records.push_back(std::move(record));
        }
        return records;
    }
};

} 
