/*
You're working on a data processing system that needs to search
through large datasets efficiently. The current linear search is too slow for production requirements.
Using the code below, implement and compare search algorithms :
Complete the binary search implementation with proper bounds checking
Build hash map for constant - time lookups
Test all three approaches on datasets of 1, 000 and 100, 000 elements
Measure and compare execution times to verify expected complexity improvements
Calculate speedup ratios(baseline_time / optimized_time)
*/

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>  
#include <chrono>
#include <iomanip>
#include <iostream>

class SearchOptimization {
public:
    // Current inefficient approach
    static int linearSearch(const std::vector<int>& data, int target) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == target) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    // Implement O(log n) binary search
    static int binarySearch(const std::vector<int>& sortedData, int target) {
        int left = 0, right = static_cast<int>(sortedData.size()) - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (sortedData[mid] == target) return mid;
            else if (sortedData[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

	// Implement O(1) average hash lookup   
    static void buildHashMap(const std::vector<int>& data,
        std::unordered_map<int, int>& hashMap) {
        // Build value-to-index mapping
        for (size_t i = 0; i < data.size(); ++i) {
            hashMap[data[i]] = static_cast<int>(i);
        }
    }

    static int hashSearch(const std::unordered_map<int, int>& hashMap, int target) {
        // Implement O(1) average hash lookup
        auto it = hashMap.find(target);
        return it != hashMap.end() ? it->second : -1;
    }
};

// Generate test data, it is not ordered and may contain duplicates 
std::vector<int> generateTestData(int size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, size * 10);

    for (int& val : data) {
        val = dis(gen);
    }
    return data;
}

int main() {
    const std::vector<int> sizes = { 1'000, 100'000 };
    constexpr int repetitions = 1'000;

    std::cout << std::fixed << std::setprecision(3);

    for (int size : sizes) {
        std::vector<int> data = generateTestData(size);
        std::vector<int> sortedData = data;
        std::sort(sortedData.begin(), sortedData.end());

        std::unordered_map<int, int> hashMap;
        SearchOptimization::buildHashMap(data, hashMap);

        std::vector<int> targets;
        targets.reserve(repetitions);
        for (int i = 0; i < repetitions; ++i) {
            targets.push_back(data[i % data.size()]);
        }

        volatile int result = 0;

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            result = SearchOptimization::linearSearch(data, targets[i]);
        }
        const auto linearTime = std::chrono::steady_clock::now() - start;

        start = std::chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            result = SearchOptimization::binarySearch(sortedData, targets[i]);
        }
        const auto binaryTime = std::chrono::steady_clock::now() - start;

        start = std::chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            result = SearchOptimization::hashSearch(hashMap, targets[i]);
        }
        const auto hashTime = std::chrono::steady_clock::now() - start;

        const double linearMicroseconds =
            std::chrono::duration<double, std::micro>(linearTime).count();
        const double binaryMicroseconds =
            std::chrono::duration<double, std::micro>(binaryTime).count();
        const double hashMicroseconds =
            std::chrono::duration<double, std::micro>(hashTime).count();

        std::cout << "\nDataset: " << size << " elements\n"
                  << "Linear search: " << linearMicroseconds << " us\n"
                  << "Binary search: " << binaryMicroseconds << " us"
                  << " (speedup: " << linearMicroseconds / binaryMicroseconds << "x)\n"
                  << "Hash search:   " << hashMicroseconds << " us"
                  << " (speedup: " << linearMicroseconds / hashMicroseconds << "x)\n";
    }

    return 0;
}
