#pragma once

#include <cstddef>
#include <queue>
#include <utility>
#include <vector>
#include "vectorstore/distance.hpp"
#include "vectorstore/vector_record.hpp"

namespace vectorstore {

inline std::vector<int> flat_search(const std::vector<float>& query,
                                     const std::vector<VectorRecord<float>>& base,
                                     size_t k) {
    std::priority_queue<std::pair<float, int>> pq;

    for (const auto& record : base) {
        float dist = squared_l2(query, record.vector);

        if (pq.size() < k) {
            pq.push({dist, record.id});
        } else if (dist < pq.top().first) {
            pq.pop();
            pq.push({dist, record.id});
        }
    }

    std::vector<int> result(pq.size());
    for (int i = static_cast<int>(result.size()) - 1; i >= 0; --i) {
        result[i] = pq.top().second;
        pq.pop();
    }
    return result;
}

}  
