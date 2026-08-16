#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>

class PerformanceBenchmark
{
private:
    std::chrono::high_resolution_clock::time_point startTime;

public:
    void startTiming();
    double getElapsedMilliseconds();

    // NOTE:
    // This function is a template and must be fully defined in the header file.
    // Templates are instantiated by the compiler when used, so their implementation
    // must be visible at the point of instantiation. Placing the definition in a
    // .cpp file would typically result in linker errors because the compiler would
    // not be able to generate the required specializations.
    template<typename Func>
    double measureAveragePerformance(Func function, const std::string& testName, int iterations = 5)
    {
        std::cout << "Testing: " << testName << std::endl;

        double totalTime = 0.0;
        for (int i = 0; i < iterations; ++i) {
            startTiming();
            function();
            totalTime += getElapsedMilliseconds();
        }

        double avgTime = totalTime / iterations;
        std::cout << "  Average time: " << std::fixed << std::setprecision(3) << avgTime << " ms" << std::endl;

        return avgTime;
    }

    void comparePerformance(double baselineTime, double optimizedTime, const std::string& description);
};