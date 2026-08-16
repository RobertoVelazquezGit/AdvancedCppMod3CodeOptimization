/*
Create a comprehensive system for measuring and comparing optimization effectiveness.
Practice: Using the code below, build performance analysis capabilities:
- Complete the measurement framework with proper timing and averaging
- Calculate speedup ratios and percentage improvements
- Test all previous optimizations using this framework
- Generate performance reports showing before / after comparisons
- Document which optimizations provide the most significant gains
*/

#include "PerformanceBenchmark.h"

void PerformanceBenchmark::startTiming()
{
    startTime = std::chrono::high_resolution_clock::now();
}

double PerformanceBenchmark::getElapsedMilliseconds()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    return duration.count() / 1000.0;
}

void PerformanceBenchmark::comparePerformance(double baselineTime, double optimizedTime, const std::string& description)
{
    double speedup = baselineTime / optimizedTime;
    double improvement = ((baselineTime - optimizedTime) / baselineTime) * 100.0;

    std::cout << '\n' << description << '\n'
        << "Baseline : " << baselineTime << " ms\n"
        << "Optimized: " << optimizedTime << " ms\n"
        << "Speedup  : " << speedup << "x\n"
        << "Improvement: " << improvement << "%\n";
}