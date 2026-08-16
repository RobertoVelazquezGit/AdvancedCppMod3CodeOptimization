#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "PerformanceBenchmark.h"
#include "MatrixOptimization.h"

int main()
{
    constexpr int matrixSize = 200;
    constexpr int vectorSize = 50000;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    // Matrices
    std::vector<std::vector<double>> A(matrixSize, std::vector<double>(matrixSize));
    std::vector<std::vector<double>> B(matrixSize, std::vector<double>(matrixSize));
    std::vector<std::vector<double>> Cbasic(matrixSize, std::vector<double>(matrixSize));
    std::vector<std::vector<double>> Copt(matrixSize, std::vector<double>(matrixSize));

    // Vectores
    std::vector<double> v1(vectorSize);
    std::vector<double> v2(vectorSize);

    // Inicialización aleatoria
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            A[i][j] = dist(rng);
            B[i][j] = dist(rng);
        }
    }

    for (int i = 0; i < vectorSize; ++i) {
        v1[i] = dist(rng);
        v2[i] = dist(rng);
    }

    PerformanceBenchmark benchmark;

    std::cout << "\n==============================\n"
        << " MATRIX MULTIPLICATION TESTS\n"
        << "==============================\n";

    double basicMatrixTime = benchmark.measureAveragePerformance(
        [&]() { MatrixOptimization::matrixMultiplyBasic(A, B, Cbasic); },
        "Basic Matrix Multiplication", 3);

    double optimizedMatrixTime = benchmark.measureAveragePerformance(
        [&]() { MatrixOptimization::matrixMultiplyOptimized(A, B, Copt); },
        "Optimized Matrix Multiplication", 3);

    benchmark.comparePerformance(basicMatrixTime, optimizedMatrixTime, "Matrix Multiplication");

    // Verificación de resultados
    double maxError = 0.0;
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            maxError = std::max(maxError, std::abs(Cbasic[i][j] - Copt[i][j]));
        }
    }

    std::cout << "\nMaximum error: " << maxError << "\n";

    std::cout << "\n==============================\n"
        << " DOT PRODUCT TESTS\n"
        << "==============================\n";

    double resultBasic = 0.0;
    double resultPragma = 0.0;
    double resultUnrolled = 0.0;

    double basicDotTime = benchmark.measureAveragePerformance(
        [&]() { resultBasic = MatrixOptimization::dotProductBasic(v1, v2); },
        "Basic Dot Product", 20);

    double pragmaDotTime = benchmark.measureAveragePerformance(
        [&]() { resultPragma = MatrixOptimization::dotProduct(v1, v2); },
        "Pragma Unrolled Dot Product", 20);

    double unrolledDotTime = benchmark.measureAveragePerformance(
        [&]() { resultUnrolled = MatrixOptimization::dotProductUnrolled(v1, v2); },
        "Manual Unrolled Dot Product", 20);

    std::cout << "\nResults:\n"
        << "Basic    : " << resultBasic << '\n'
        << "Pragma   : " << resultPragma << '\n'
        << "Unrolled : " << resultUnrolled << '\n';

    benchmark.comparePerformance(basicDotTime, pragmaDotTime, "Basic vs Pragma");
    benchmark.comparePerformance(basicDotTime, unrolledDotTime, "Basic vs Manual Unrolling");

    return 0;
}