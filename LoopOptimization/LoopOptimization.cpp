/*
You're optimizing matrix operations for a graphics rendering pipeline where performance is critical for maintaining frame rates.
Practice
In the code below, optimize computational loops :
Implement loop interchange in matrix multiplication(change i - j - k to i - k - j order)
Add loop unrolling to dot product computation(process 4 elements per iteration)
Test with matrices of size 200x200 and vectors of length 50, 000
Measure cache performance improvements from loop reordering
Verify numerical correctness of optimized versions
*/

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>


class MatrixOptimization {
public:
    // Basic matrix multiplication - poor cache locality
    static void matrixMultiplyBasic(const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B,
        std::vector<std::vector<double>>& C) {
        int n = static_cast<int>(A.size());
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                C[i][j] = 0.0;
                for (int k = 0; k < n; ++k) {
                    C[i][j] += A[i][k] * B[k][j];  // Poor cache locality
                }
            }
        }
    }

    static void matrixMultiplyOptimized(const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B,
        std::vector<std::vector<double>>& C) {
        int n = static_cast<int>(A.size());
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) C[i][j] = 0.0;
        }

        // Better cache locality: the inner loop scans a fixed row sequentially,
        // accessing consecutive elements instead of jumping between rows in a fixed column.
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < n; ++k) {
                double a_ik = A[i][k];
                for (int j = 0; j < n; ++j) {
                    C[i][j] += a_ik * B[k][j];
                }
            }
        }
    }

    // Basic vector dot product
    static double dotProductBasic(const std::vector<double>& a, const std::vector<double>& b) {
        double result = 0.0;
        for (size_t i = 0; i < a.size(); ++i) {
            result += a[i] * b[i];
        }
        return result;
    }

    static double dotProductUnrolled(const std::vector<double>& a, const std::vector<double>& b) {
        // Process multiple elements per iteration to reduce loop overhead
        double result = 0.0;
        size_t i = 0;

        for (; i + 3 < a.size(); i += 4) {
            result += a[i] * b[i];
            result += a[i + 1] * b[i + 1];
            result += a[i + 2] * b[i + 2];
            result += a[i + 3] * b[i + 3];
        }

        for (; i < a.size(); ++i) {
            result += a[i] * b[i];
        }

        return result;
    }

    static double dotProduct(const std::vector<double>& a, const std::vector<double>& b)
    {
        double result = 0.0;

#pragma loop(hint_unroll)
        for (size_t i = 0; i < a.size(); ++i)
        {
            result += a[i] * b[i];
        }

        return result;
    }
};


int main()
{
    using namespace std::chrono;

    constexpr int matrixSize = 200;
    constexpr int vectorSize = 50000;

	std::mt19937 rng(42);  // Fixed seed for reproducibility    
    std::uniform_real_distribution<double> dist(0.0, 100.0);

    // ==========================================================
    // MATRIX TEST
    // ==========================================================

    std::vector<std::vector<double>> A(matrixSize, std::vector<double>(matrixSize));

    std::vector<std::vector<double>> B(matrixSize, std::vector<double>(matrixSize));

    std::vector<std::vector<double>> Cbasic(matrixSize, std::vector<double>(matrixSize));

    std::vector<std::vector<double>> Copt(matrixSize, std::vector<double>(matrixSize));

    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
			A[i][j] = dist(rng);  // row fixed advancing on columns good for cache locality 
            B[i][j] = dist(rng);
        }
    }

    auto start = high_resolution_clock::now();

    MatrixOptimization::matrixMultiplyBasic(A, B, Cbasic);

    auto end = high_resolution_clock::now();

    auto basicTime = duration_cast<milliseconds>(end - start);

    start = high_resolution_clock::now();

    MatrixOptimization::matrixMultiplyOptimized(A, B, Copt);

    end = high_resolution_clock::now();

    auto optimizedTime = duration_cast<milliseconds>(end - start);

    double maxMatrixError = 0.0;

    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            maxMatrixError = std::max(maxMatrixError, std::abs(Cbasic[i][j] - Copt[i][j]));
        }
    }

    std::cout << "\n=== MATRIX MULTIPLICATION ===\n";
    std::cout << "Basic      : " << basicTime.count() << " ms\n";

    std::cout << "Optimized  : " << optimizedTime.count() << " ms\n";

    std::cout << "Max error  : " << maxMatrixError << "\n";

    // ==========================================================
    // DOT PRODUCT TEST
    // ==========================================================

    std::vector<double> v1(vectorSize);
    std::vector<double> v2(vectorSize);

    for (int i = 0; i < vectorSize; ++i)
    {
        v1[i] = dist(rng);
        v2[i] = dist(rng);
    }

    start = high_resolution_clock::now();

    double resultBasic = MatrixOptimization::dotProductBasic(v1, v2);

    end = high_resolution_clock::now();

    auto dotBasicTime = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();

    double resultPragma = MatrixOptimization::dotProduct(v1, v2);

    end = high_resolution_clock::now();

    auto dotPragmaTime = duration_cast<microseconds>(end - start);

    start = high_resolution_clock::now();

    double resultUnrolled = MatrixOptimization::dotProductUnrolled(v1, v2);

    end = high_resolution_clock::now();

    auto dotUnrolledTime = duration_cast<microseconds>(end - start);

    std::cout << "\n=== DOT PRODUCT ===\n";

    std::cout << std::fixed << std::setprecision(10);

    std::cout << "Basic result    : " << resultBasic << "\n";

    std::cout << "Pragma result   : " << resultPragma << "\n";

    std::cout << "Unrolled result : " << resultUnrolled << "\n\n";

    std::cout << "Basic time      : " << dotBasicTime.count() << " us\n";

    std::cout << "Pragma time     : " << dotPragmaTime.count() << " us\n";

    std::cout << "Unrolled time   : " << dotUnrolledTime.count() << " us\n";

    std::cout << "\nDifference Basic-Pragma   : " << std::abs(resultBasic - resultPragma) << "\n";

    std::cout << "Difference Basic-Unrolled : " << std::abs(resultBasic - resultUnrolled) << "\n";

    return 0;
}
