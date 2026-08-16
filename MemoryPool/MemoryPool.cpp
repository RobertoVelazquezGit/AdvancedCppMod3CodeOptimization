/*
You're building a real-time system that frequently allocates and deallocates small objects. The default heap allocation is causing performance bottlenecks.
Practice
Using the code below, build and test custom memory allocation :
Implement pool storage using pre - allocated vector
Track available slots with boolean vector or free list
Create performance comparison between pool allocation and heap allocation
Test with 1, 000 allocation / deallocation cycles
Measure allocation speed improvements and memory fragmentation reduction
*/

#include <vector>
#include <memory>
#include <chrono>
#include <iostream>

template<typename T>
class CustomMemoryPool {
private:
    std::vector<T> pool;
    std::vector<bool> available;

public:
    CustomMemoryPool(size_t poolSize) : pool(poolSize), available(poolSize, true) {}

    T* allocate() {
        for (size_t i = 0; i < available.size(); ++i) {
            if (available[i]) {
                available[i] = false;
                return &pool[i];
            }
        }
        return nullptr;
    }

    void deallocate(T* ptr) {
        if (ptr >= &pool[0] && ptr < &pool[0] + pool.size()) {
            size_t index = ptr - &pool[0];
            available[index] = true;
        }
    }

    size_t availableSlots() const {
        size_t count = 0;
        for (bool avail : available) {
            if (avail) ++count;
        }
        return count;
    }
};

// Test object for memory pool
struct TestObject {
    double data[8];  // 64 bytes
    int id;
    TestObject() : id(0) {
        for (int i = 0; i < 8; ++i) data[i] = 0.0;
    }
    TestObject(int id_val) : id(id_val) {
        for (int i = 0; i < 8; ++i) data[i] = id_val * i;
    }
};

int main() {
    const size_t poolSize = 1000;
    const int cycles = 1000;

    std::cout << "=== CustomMemoryPool Benchmark ===" << std::endl;
    std::cout << "Pool size: " << poolSize << " objects" << std::endl;
    std::cout << "Test cycles: " << cycles << std::endl << std::endl;

    // Benchmark: CustomMemoryPool allocation
    CustomMemoryPool<TestObject> memoryPool(poolSize);
    std::vector<TestObject*> poolAllocated;

    auto startPool = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < cycles; ++i) {
        TestObject* obj = memoryPool.allocate();
        if (obj) {
            new (obj) TestObject(i);  // Placement new
            poolAllocated.push_back(obj);
        }
    }
    auto endPool = std::chrono::high_resolution_clock::now();
    auto poolAllocDuration = std::chrono::duration_cast<std::chrono::microseconds>(endPool - startPool);

    std::cout << "Pool allocation time: " << poolAllocDuration.count() << " us" << std::endl;
    std::cout << "Objects allocated: " << poolAllocated.size() << std::endl;
    std::cout << "Available slots remaining: " << memoryPool.availableSlots() << std::endl << std::endl;

    // Benchmark: CustomMemoryPool deallocation
    auto startPoolDealloc = std::chrono::high_resolution_clock::now();
    for (auto obj : poolAllocated) {
		obj->~TestObject();  // is the delete with placement new, so we call the destructor explicitly  
        memoryPool.deallocate(obj);
    }
    auto endPoolDealloc = std::chrono::high_resolution_clock::now();
    auto poolDeallocDuration = std::chrono::duration_cast<std::chrono::microseconds>(endPoolDealloc - startPoolDealloc);

    std::cout << "Pool deallocation time: " << poolDeallocDuration.count() << " us" << std::endl;
    std::cout << "Available slots after dealloc: " << memoryPool.availableSlots() << std::endl << std::endl;

    // Benchmark: Heap allocation for comparison
    std::vector<TestObject*> heapAllocated;

    auto startHeap = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < cycles; ++i) {
        TestObject* obj = new TestObject(i);
        heapAllocated.push_back(obj);
    }
    auto endHeap = std::chrono::high_resolution_clock::now();
    auto heapAllocDuration = std::chrono::duration_cast<std::chrono::microseconds>(endHeap - startHeap);

    std::cout << "Heap allocation time: " << heapAllocDuration.count() << " us" << std::endl << std::endl;

    // Benchmark: Heap deallocation
    auto startHeapDealloc = std::chrono::high_resolution_clock::now();
    for (auto obj : heapAllocated) {
        delete obj;
    }
    auto endHeapDealloc = std::chrono::high_resolution_clock::now();
    auto heapDeallocDuration = std::chrono::duration_cast<std::chrono::microseconds>(endHeapDealloc - startHeapDealloc);

    std::cout << "Heap deallocation time: " << heapDeallocDuration.count() << " us" << std::endl << std::endl;

    // Performance comparison
    std::cout << "=== Performance Comparison ===" << std::endl;
    double poolSpeedup = static_cast<double>(heapAllocDuration.count()) / poolAllocDuration.count();
    double deallocSpeedup = static_cast<double>(heapDeallocDuration.count()) / poolDeallocDuration.count();

    std::cout << "Allocation speedup (Heap / Pool): " << poolSpeedup << "x faster" << std::endl;
    std::cout << "Deallocation speedup (Heap / Pool): " << deallocSpeedup << "x faster" << std::endl;

    return 0;
}