#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "vectorstore/flat_search.hpp"
#include "vectorstore/vector_record.hpp"
#include "vectorstore/vector_store_io.hpp"

int main() {
    const std::string base_path = "data/siftsmall_base.fvecs";
    const std::string query_path = "data/siftsmall_query.fvecs";
    const std::string gt_path = "data/siftsmall_groundtruth.ivecs";

    std::vector<vectorstore::VectorRecord<float>> base_vectors;
    try {
        base_vectors = vectorstore::VectorStoreIO::read_vecs<float>(base_path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading base vectors: " << e.what() << "\n";
        return 1;
    }

    if (base_vectors.size() != 10000) {
        std::cerr << "Dataset size mismatch! Expected 10000 vectors, got "
                   << base_vectors.size() << "\n";
        return 1;
    }

    for (size_t i = 0; i < base_vectors.size(); ++i) {
        if (base_vectors[i].dimension() != 128) {
            std::cerr << "Dimension mismatch at vector " << i << "! Expected 128, got "
                       << base_vectors[i].dimension() << "\n";
            return 1;
        }
    }

    std::vector<vectorstore::VectorRecord<float>> queries;
    std::vector<vectorstore::VectorRecord<int32_t>> ground_truth;

    try {
        queries = vectorstore::VectorStoreIO::read_vecs<float>(query_path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading queries: " << e.what() << "\n";
        return 1;
    }

    try {
        ground_truth = vectorstore::VectorStoreIO::read_vecs<int32_t>(gt_path);
    } catch (const std::exception& e) {
        std::cerr << "Error loading ground truth: " << e.what() << "\n";
        return 1;
    }

    if (queries.size() != 100) {
        std::cerr << "Query dataset must have 100 vectors, got " << queries.size() << "\n";
        return 1;
    }

    if (ground_truth.size() != 100) {
        std::cerr << "Ground truth dataset must have 100 vectors, got "
                   << ground_truth.size() << "\n";
        return 1;
    }

    const size_t k = 100;
    size_t total_match = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < queries.size(); ++i) {
        std::vector<int> retrieved_ids = vectorstore::flat_search(queries[i].vector, base_vectors, k);

        const auto& gt_ids = ground_truth[i].vector;
        std::set<int> gt_set(gt_ids.begin(), gt_ids.begin() + static_cast<long>(k));

        for (int id : retrieved_ids) {
            if (gt_set.count(id)) {
                ++total_match;
            }
        }

        if ((i + 1) % 20 == 0) {
            std::cout << "processed " << (i + 1) << " / " << queries.size() << " queries\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double total_seconds = std::chrono::duration<double>(end - start).count();
    double avg_ms = (total_seconds * 1000.0) / static_cast<double>(queries.size());
    double recall = static_cast<double>(total_match) / static_cast<double>(queries.size() * k);

    std::cout << "\n=== Results ===\n";
    std::cout << "Total search time      : " << total_seconds << " s\n";
    std::cout << "Avg time / query        : " << avg_ms << " ms\n";
    std::cout << "Recall@" << k << "               : " << recall << "\n";

    if (recall >= 0.999999) {
        std::cout << "[PASS] Brute-force search matches ground truth (Recall@" << k
                   << " == 1.0 within tolerance)\n";
        return 0;
    } else {
        std::cout << "[FAIL] Recall@" << k << " below expected threshold\n";
        return 1;
    }
}