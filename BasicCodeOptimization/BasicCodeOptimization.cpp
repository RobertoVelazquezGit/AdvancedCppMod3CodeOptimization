// ============================================================================
// Basic Code Optimization Lab - Solution Files
// Complete implementation of fundamental optimization techniques
// ============================================================================

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <unordered_map>

using namespace std;

// Constants for testing
const int SMALL_SIZE = 1000;
const int LARGE_SIZE = 10000;
const int NUM_ITERATIONS = 5;

// Simple timer class for performance measurement
class SimpleTimer {
private:
    chrono::high_resolution_clock::time_point start_time;

public:
    void start() {
        start_time = chrono::high_resolution_clock::now();
    }

    double getElapsedMs() {
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;  // Convert to milliseconds
    }
};

// ============================================================================
// PART 1: Memory Optimization Techniques
// ============================================================================

// Unoptimized data structure (Array of Structures)
struct UnoptimizedParticle {
    double x, y, z;      // Position
    double vx, vy, vz;   // Velocity  
    double mass;
    int id;
    char padding[4];     // Unintentional padding
};

// SOLUTION: Optimized data structure using Structure of Arrays (SoA)
class OptimizedParticleSystem {
private:
    vector<double> x, y, z;      // Position arrays
    vector<double> vx, vy, vz;   // Velocity arrays
    vector<double> mass;         // Mass array
    vector<int> id;              // ID array

public:
    size_t size() const {
        return x.size();
    }

    void resize(size_t n) {
        x.resize(n);
        y.resize(n);
        z.resize(n);
        vx.resize(n);
        vy.resize(n);
        vz.resize(n);
        mass.resize(n);
        id.resize(n);
    }

    void setParticle(size_t i, double x_val, double y_val, double z_val,
        double vx_val, double vy_val, double vz_val, double mass_val, int id_val) {
        x[i] = x_val;
        y[i] = y_val;
        z[i] = z_val;
        vx[i] = vx_val;
        vy[i] = vy_val;
        vz[i] = vz_val;
        mass[i] = mass_val;
        id[i] = id_val;
    }

    void updatePositions(double dt) {
        // Cache-friendly sequential access
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += vx[i] * dt;
            y[i] += vy[i] * dt;
            z[i] += vz[i] * dt;
        }
    }

    double getTotalKineticEnergy() const {
        double totalEnergy = 0.0;
        // Sequential access to arrays - cache-friendly
        for (size_t i = 0; i < x.size(); ++i) {
            double vel2 = vx[i] * vx[i] + vy[i] * vy[i] + vz[i] * vz[i];
            totalEnergy += 0.5 * mass[i] * vel2;
        }
        return totalEnergy;
    }
};

// SOLUTION: Simple memory pool for frequent allocations
template<typename T>
class SimpleMemoryPool {
private:
    vector<T> pool;          // Pre-allocated storage
    vector<bool> available;  // Track which slots are free
    size_t poolSize;

public:
    SimpleMemoryPool(size_t poolSize) : poolSize(poolSize) {
        pool.resize(poolSize);
        available.resize(poolSize, true);  // All slots initially available
    }

    T* allocate() {
        // Find first available slot
        for (size_t i = 0; i < poolSize; ++i) {
            if (available[i]) {
                available[i] = false;  // Mark as used
                return &pool[i];
            }
        }
        return nullptr;  // Pool exhausted
    }

    void deallocate(T* ptr) {
        if (ptr >= &pool[0] && ptr < &pool[poolSize]) {
            size_t index = ptr - &pool[0];
            available[index] = true;  // Mark as available
        }
    }

    size_t getPoolSize() const {
        return poolSize;
    }
};

// ============================================================================
// PART 2: Algorithm Analysis and Optimization
// ============================================================================

// SOLUTION: Search algorithm implementations and comparisons
class SearchAlgorithms {
public:
    // Linear search - O(n)
    static int linearSearch(const vector<int>& data, int target) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == target) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    // Binary search - O(log n) - requires sorted data
    static int binarySearch(const vector<int>& data, int target) {
        int left = 0;
        int right = static_cast<int>(data.size()) - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;  // Avoid overflow

            if (data[mid] == target) {
                return mid;
            }
            else if (data[mid] < target) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return -1;
    }

    // Hash-based search using unordered_map - O(1) average
    static void buildHashMap(const vector<int>& data, unordered_map<int, int>& hashMap) {
        hashMap.clear();
        for (size_t i = 0; i < data.size(); ++i) {
            hashMap[data[i]] = static_cast<int>(i);  // value -> index mapping
        }
    }

    static int hashSearch(const unordered_map<int, int>& hashMap, int target) {
        auto it = hashMap.find(target);
        return (it != hashMap.end()) ? it->second : -1;
    }
};

// SOLUTION: Sorting algorithm optimizations
// Ordering from small to large for better cache performance    
class SortingOptimization {
public:
    // Bubble sort - O(n²) - inefficient but simple
    static void bubbleSort(vector<int>& data) {
        size_t n = data.size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
					swap(data[j], data[j + 1]);  // the bigger element bubbles to the end   
                }
            }
        }
    }

    // Use STL sort - O(n log n) - highly optimized
    static void stlSort(vector<int>& data) {
        sort(data.begin(), data.end());
    }

    // Optimized for small arrays - hybrid approach
    static void hybridSort(vector<int>& data) {
        if (data.size() <= 20) {
            insertionSort(data);  // Better for small arrays
        }
        else {
            stlSort(data);        // Better for large arrays
        }
    }

private:
    static void insertionSort(vector<int>& data) {
        for (size_t i = 1; i < data.size(); ++i) {
            int key = data[i];
            int j = static_cast<int>(i) - 1;

            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }
};

// ============================================================================
// PART 3: Loop and Computational Optimizations
// ============================================================================

// SOLUTION: Loop optimization techniques
class LoopOptimizations {
public:
    // Unoptimized matrix multiplication - poor cache locality
    static void matrixMultiplyBasic(const vector<vector<double>>& A,
        const vector<vector<double>>& B,
        vector<vector<double>>& C) {
        int n = static_cast<int>(A.size());
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                C[i][j] = 0.0;
                for (int k = 0; k < n; ++k) {
                    C[i][j] += A[i][k] * B[k][j];  // Poor cache locality for B
                }
            }
        }
    }

    // Cache-optimized matrix multiplication with loop interchange
    static void matrixMultiplyOptimized(const vector<vector<double>>& A,
        const vector<vector<double>>& B,
        vector<vector<double>>& C) {
        int n = static_cast<int>(A.size());

        // Initialize result matrix
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                C[i][j] = 0.0;
            }
        }

        // i-k-j order for better cache locality
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < n; ++k) {
                double a_ik = A[i][k];  // Load once, reuse
                for (int j = 0; j < n; ++j) {
                    C[i][j] += a_ik * B[k][j];  // Better cache locality
                }
            }
        }
    }

    // Basic dot product
    static double dotProductBasic(const vector<double>& a, const vector<double>& b) {
        double result = 0.0;
        for (size_t i = 0; i < a.size(); ++i) {
            result += a[i] * b[i];
        }
        return result;
    }

    // Loop unrolling for better performance
    static double dotProductUnrolled(const vector<double>& a, const vector<double>& b) {
        double result = 0.0;
        size_t n = a.size();
        size_t i = 0;

        // Process 4 elements at a time (loop unrolling)
        for (; i + 3 < n; i += 4) {
            result += a[i] * b[i];
            result += a[i + 1] * b[i + 1];
            result += a[i + 2] * b[i + 2];
            result += a[i + 3] * b[i + 3];
        }

        // Handle remaining elements
        for (; i < n; ++i) {
            result += a[i] * b[i];
        }

        return result;
    }
};

// ============================================================================
// PART 4: Performance Testing Framework
// ============================================================================

class PerformanceTester {
private:
    SimpleTimer timer;

public:
    template<typename Func>
    double measurePerformance(Func function, const string& description) {
        cout << "Testing: " << description << "..." << endl;

        timer.start();
        for (int i = 0; i < NUM_ITERATIONS; ++i) {
            function();
        }
        double totalTime = timer.getElapsedMs();
        double avgTime = totalTime / NUM_ITERATIONS;

        cout << "  Average time: " << fixed << setprecision(3) << avgTime << " ms" << endl;
        return avgTime;
    }

    void comparePerformance(double baselineTime, double optimizedTime, const string& optimization) {
        double speedup = baselineTime / optimizedTime;
        double improvement = ((baselineTime - optimizedTime) / baselineTime) * 100.0;

        cout << "  " << optimization << ":" << endl;
        cout << "    Speedup: " << fixed << setprecision(2) << speedup << "x" << endl;
        cout << "    Improvement: " << fixed << setprecision(1) << improvement << "%" << endl;
        cout << endl;
    }
};

// Test data generators
vector<int> generateRandomData(int size) {
    vector<int> data(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, size * 10);

    for (int& val : data) {
        val = dis(gen);
    }
    return data;
}

vector<UnoptimizedParticle> generateParticles(int count) {
    vector<UnoptimizedParticle> particles(count);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> pos(-100.0, 100.0);
    uniform_real_distribution<double> vel(-10.0, 10.0);
    uniform_real_distribution<double> mass(0.1, 10.0);

    for (int i = 0; i < count; ++i) {
        particles[i] = { pos(gen), pos(gen), pos(gen),
                       vel(gen), vel(gen), vel(gen),
                       mass(gen), i, {0} };
    }
    return particles;
}

// Main testing functions
void testMemoryOptimizations() {
    cout << "\n=== Memory Optimization Tests ===" << endl;

    PerformanceTester tester;
    auto particles = generateParticles(LARGE_SIZE);

    // Test Array of Structures vs Structure of Arrays
    auto aosTest = [&particles]() {
        double totalEnergy = 0.0;
        for (const auto& p : particles) {
            double vel2 = p.vx * p.vx + p.vy * p.vy + p.vz * p.vz;
            totalEnergy += 0.5 * p.mass * vel2;
        }
        };

    OptimizedParticleSystem soaParticles;
    soaParticles.resize(LARGE_SIZE);
    for (size_t i = 0; i < particles.size(); ++i) {
        soaParticles.setParticle(i, particles[i].x, particles[i].y, particles[i].z,
            particles[i].vx, particles[i].vy, particles[i].vz,
            particles[i].mass, particles[i].id);
    }

    auto soaTest = [&soaParticles]() {
        double totalEnergy = soaParticles.getTotalKineticEnergy();
        };

    double aosTime = tester.measurePerformance(aosTest, "Array of Structures (AoS)");
    double soaTime = tester.measurePerformance(soaTest, "Structure of Arrays (SoA)");

    tester.comparePerformance(aosTime, soaTime, "SoA vs AoS");

    // Test memory pool
    cout << "Testing memory pool allocation..." << endl;
    SimpleMemoryPool<int> pool(1000);

    auto poolTest = [&pool]() {
        vector<int*> ptrs;
        for (int i = 0; i < 100; ++i) {
            ptrs.push_back(pool.allocate());
        }
        for (int* ptr : ptrs) {
            if (ptr) pool.deallocate(ptr);
        }
        };

    auto heapTest = []() {
        vector<int*> ptrs;
        for (int i = 0; i < 100; ++i) {
            ptrs.push_back(new int(i));
        }
        for (int* ptr : ptrs) {
            delete ptr;
        }
        };

    double poolTime = tester.measurePerformance(poolTest, "Memory Pool Allocation");
    double heapTime = tester.measurePerformance(heapTest, "Heap Allocation");

    tester.comparePerformance(heapTime, poolTime, "Memory Pool vs Heap");
}

void testAlgorithmOptimizations() {
    cout << "\n=== Algorithm Optimization Tests ===" << endl;

    PerformanceTester tester;
    auto data = generateRandomData(LARGE_SIZE);
    auto sortedData = data;
    sort(sortedData.begin(), sortedData.end());

    int target = data[data.size() / 2];  // Pick middle element

    // Search algorithm comparison
    auto linearTest = [&data, target]() {
        SearchAlgorithms::linearSearch(data, target);
        };

    auto binaryTest = [&sortedData, target]() {
        SearchAlgorithms::binarySearch(sortedData, target);
        };

    unordered_map<int, int> hashMap;
    SearchAlgorithms::buildHashMap(data, hashMap);
    auto hashTest = [&hashMap, target]() {
        SearchAlgorithms::hashSearch(hashMap, target);
        };

    double linearTime = tester.measurePerformance(linearTest, "Linear Search O(n)");
    double binaryTime = tester.measurePerformance(binaryTest, "Binary Search O(log n)");
    double hashTime = tester.measurePerformance(hashTest, "Hash Search O(1)");

    tester.comparePerformance(linearTime, binaryTime, "Binary vs Linear");
    tester.comparePerformance(linearTime, hashTime, "Hash vs Linear");

    // Sorting algorithm comparison
    auto testData = generateRandomData(SMALL_SIZE);

    auto bubbleTest = [&testData]() {
        auto data = testData;
        SortingOptimization::bubbleSort(data);
        };

    auto stlTest = [&testData]() {
        auto data = testData;
        SortingOptimization::stlSort(data);
        };

    auto hybridTest = [&testData]() {
        auto data = testData;
        SortingOptimization::hybridSort(data);
        };

    double bubbleTime = tester.measurePerformance(bubbleTest, "Bubble Sort O(n^2)");
    double stlTime = tester.measurePerformance(stlTest, "STL Sort O(n log n)");
    double hybridTime = tester.measurePerformance(hybridTest, "Hybrid Sort");

    tester.comparePerformance(bubbleTime, stlTime, "STL vs Bubble");
    tester.comparePerformance(stlTime, hybridTime, "Hybrid vs STL");
}

void testLoopOptimizations() {
    cout << "\n=== Loop Optimization Tests ===" << endl;

    PerformanceTester tester;
    const int matrixSize = 200;  // Small enough for reasonable test time

    // Initialize matrices with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    vector<vector<double>> A(matrixSize, vector<double>(matrixSize));
    vector<vector<double>> B(matrixSize, vector<double>(matrixSize));
    vector<vector<double>> C1(matrixSize, vector<double>(matrixSize, 0.0));
    vector<vector<double>> C2(matrixSize, vector<double>(matrixSize, 0.0));

    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    auto basicMatrixTest = [&]() {
        LoopOptimizations::matrixMultiplyBasic(A, B, C1);
        };

    auto optimizedMatrixTest = [&]() {
        LoopOptimizations::matrixMultiplyOptimized(A, B, C2);
        };

    double basicTime = tester.measurePerformance(basicMatrixTest, "Basic Matrix Multiply (i-j-k)");
    double optimizedTime = tester.measurePerformance(optimizedMatrixTest, "Optimized Matrix Multiply (i-k-j)");

    tester.comparePerformance(basicTime, optimizedTime, "Loop Interchange Optimization");

    // Vector operations
    vector<double> vec1(LARGE_SIZE);
    vector<double> vec2(LARGE_SIZE);

    for (int i = 0; i < LARGE_SIZE; ++i) {
        vec1[i] = dist(gen);
        vec2[i] = dist(gen);
    }

    auto basicDotTest = [&]() {
        volatile double result = LoopOptimizations::dotProductBasic(vec1, vec2);
        (void)result;  // Prevent optimization
        };

    auto unrolledDotTest = [&]() {
        volatile double result = LoopOptimizations::dotProductUnrolled(vec1, vec2);
        (void)result;  // Prevent optimization
        };

    double basicDotTime = tester.measurePerformance(basicDotTest, "Basic Dot Product");
    double unrolledDotTime = tester.measurePerformance(unrolledDotTest, "Unrolled Dot Product");

    tester.comparePerformance(basicDotTime, unrolledDotTime, "Loop Unrolling Optimization");

    // Verify correctness
    double result1 = LoopOptimizations::dotProductBasic(vec1, vec2);
    double result2 = LoopOptimizations::dotProductUnrolled(vec1, vec2);
    cout << "Correctness check - Basic: " << fixed << setprecision(6) << result1
        << ", Unrolled: " << result2 << " (difference: " << abs(result1 - result2) << ")" << endl;
}

int main() {
    cout << "=== Basic Code Optimization Lab ===" << endl;
    cout << "Learning memory optimization, algorithm analysis, and compiler optimization" << endl;

    testMemoryOptimizations();
    testAlgorithmOptimizations();
    testLoopOptimizations();

    cout << "\n=== Compiler Optimization Demonstration ===" << endl;
    cout << "Current build optimization level affects these results." << endl;
    cout << "Try compiling with different flags to see the impact:" << endl;
    cout << "  g++ -O0 : No optimization (slowest)" << endl;
    cout << "  g++ -O2 : Standard optimization (good balance)" << endl;
    cout << "  g++ -O3 : Maximum optimization (fastest)" << endl;
    cout << "  g++ -O3 -march=native : Optimize for your specific CPU" << endl;

    cout << "\n=== Lab Complete! ===" << endl;
    cout << "- Structure of Arrays (SoA) for better cache locality" << endl;
    cout << "- Memory pools for efficient allocation patterns" << endl;
    cout << "- Algorithm complexity optimization (O(n^2) -> O(n log n))" << endl;
    cout << "- Search algorithm optimization (O(n) -> O(log n) -> O(1))" << endl;
    cout << "- Loop interchange for better cache utilization" << endl;
    cout << "- Loop unrolling for reduced overhead" << endl;
    cout << "- Performance measurement and comparison framework" << endl;
    cout << "\nYou've successfully implemented fundamental optimization techniques!" << endl;

    return 0;
}

/*
VALIDATION CHECKLIST COMPLETED:
- Structure of Arrays (SoA) implemented with separate arrays for each particle property

- Simple memory pool created with pre-allocated storage and availability tracking

- Linear, binary, and hash search algorithms implemented with correct time complexities

- Sorting algorithms compared showing dramatic performance difference (O(n²) vs O(n logn))

- Matrix multiplication optimized using loop interchange (i-j-k → i-k-j) for better cache locality

- Loop unrolling implemented for dot product with proper handling of remaining elements

- Performance measurement framework provides accurate timing and comparison metrics

- All optimizations demonstrate measurable performance improvements

EXPECTED OPTIMIZATION RESULTS:
- SoA should be 10-30% faster than AoS for large datasets due to better cache locality
- Binary search should be 100-1000x faster than linear search for large arrays
- Hash search should be fastest overall with near-constant time
- STL sort should be 100-10000x faster than bubble sort depending on data size
- Loop interchange can provide 20-50% improvement for matrix operations
- Loop unrolling typically provides 5-15% improvement for simple operations
- Memory pools show significant speedup for frequent small allocations

These results vary based on:
- Compiler optimization level (-O0 vs -O2 vs -O3)
- CPU architecture and cache sizes
- Data sizes and access patterns
- System load and other factors
*/