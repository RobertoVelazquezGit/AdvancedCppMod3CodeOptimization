#include "MatrixOptimization.h"

void MatrixOptimization::matrixMultiplyBasic(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B,
    std::vector<std::vector<double>>& C)
{
    int n = static_cast<int>(A.size());

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            C[i][j] = 0.0;

            for (int k = 0; k < n; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void MatrixOptimization::matrixMultiplyOptimized(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B,
    std::vector<std::vector<double>>& C)
{
    int n = static_cast<int>(A.size());

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            C[i][j] = 0.0;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            double a_ik = A[i][k];

            for (int j = 0; j < n; ++j)
            {
                C[i][j] += a_ik * B[k][j];
            }
        }
    }
}

double MatrixOptimization::dotProductBasic(
    const std::vector<double>& a,
    const std::vector<double>& b)
{
    double result = 0.0;

    for (size_t i = 0; i < a.size(); ++i)
    {
        result += a[i] * b[i];
    }

    return result;
}

double MatrixOptimization::dotProductUnrolled(
    const std::vector<double>& a,
    const std::vector<double>& b)
{
    double result = 0.0;
    size_t i = 0;

    for (; i + 3 < a.size(); i += 4)
    {
        result += a[i] * b[i];
        result += a[i + 1] * b[i + 1];
        result += a[i + 2] * b[i + 2];
        result += a[i + 3] * b[i + 3];
    }

    for (; i < a.size(); ++i)
    {
        result += a[i] * b[i];
    }

    return result;
}

double MatrixOptimization::dotProduct(
    const std::vector<double>& a,
    const std::vector<double>& b)
{
    double result = 0.0;

#pragma loop(hint_unroll)
    for (size_t i = 0; i < a.size(); ++i)
    {
        result += a[i] * b[i];
    }

    return result;
}