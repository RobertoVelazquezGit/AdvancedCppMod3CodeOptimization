#pragma once
#pragma once

#include <vector>

class MatrixOptimization
{
public:
    // Matrix multiplication
    static void matrixMultiplyBasic(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B,
        std::vector<std::vector<double>>& C);

    static void matrixMultiplyOptimized(
        const std::vector<std::vector<double>>& A,
        const std::vector<std::vector<double>>& B,
        std::vector<std::vector<double>>& C);

    // Dot product
    static double dotProductBasic(
        const std::vector<double>& a,
        const std::vector<double>& b);

    static double dotProductUnrolled(
        const std::vector<double>& a,
        const std::vector<double>& b);

    static double dotProduct(
        const std::vector<double>& a,
        const std::vector<double>& b);
};
