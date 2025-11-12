/*
    Include Code for matrix Operations
    Main file to include
*/

#pragma once

#include "vecX.hpp"
#include <random>
#include <ctime>
#include <cmath>
#include <functional>

int randCount = 0;

vecX<double> MatMul(vecX<double> &mat1, vecX<double> &mat2)
{
    vecX<double> res(mat1.row, mat2.col, 0.0);
    for (int i = 0; i < mat1.row; i++)
    {
        for (int j = 0; j < mat2.col; j++)
        {
            double temp = 0;
            for (int k = 0; k < mat2.row; k++)
            {
                temp += (mat1.Get(i, k) * mat2.Get(k, j));
            }
            res.push(i, j, temp);
        }
    }
    return res;
}

// Add two matrix, even if there size is not same
vecX<double> MatAdd(vecX<double> &mat1, vecX<double> &mat2)
{
    /*
        Here mi = row, and ni = column
        if mat1 = (m1 X n1), mat2 = (m2 X n2)

        Then take,
            m = max(m1, m2)
            n = max(n1, n2)

        and for each  0 <= i < m and 0 <= j < n
        use mat1[i % m1][j % n1] + mat2[i % m1][j % n2]
    */
    int row = std::max(mat1.row, mat2.row), col = std::max(mat1.col, mat2.col);
    vecX<double> res(row, col, 0.0);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // if one matrix row or column are smaller then other it will automatic find this.
            res.push(i, j, mat1.Get(i % mat1.row, j % mat1.col) + mat2.Get(i % mat2.row, j % mat2.col));
        }
    }
    return res;
}

vecX<double> MatScalarProd(vecX<double> &mat, double val)
{
    vecX<double> res(mat.row, mat.col, 0.0);
    for (int i = 0; i < mat.len; i++)
    {
        res.push(i, val * mat.Get(i));
    }
    return res;
}

vecX<double> HadamardProduct(vecX<double> &mat1, vecX<double> &mat2)
{
    vecX<double> res(mat1.row, mat1.col, 0);

    // two loop because matrix can be transposed, and there is not any changes of transposed matrix in array
    for (int i = 0; i < mat1.row; i++)
    {
        for (int j = 0; j < mat1.col; j++)
            res.push(i, j, mat1.Get(i, j) * mat2.Get(i, j));
    }
    return res;
}

vecX<double> RandomVecX(int row, int col, double mean = 0.0, double variance = 1.0)
{
    vecX<double> vec(row, col, 0.0);
    std::random_device rd;
    std::time_t currentTime = std::time(nullptr);
    unsigned int uniqueNumber = static_cast<unsigned int>(currentTime) + randCount;
    randCount++;
    std::mt19937 gen(uniqueNumber);
    std::normal_distribution<> gaussian(mean, std::sqrt(variance));

    for (int i = 0; i < vec.len; i++)
    {
        vec.push(i, gaussian(gen));
    }
    return vec;
}

template <typename F>
vecX<double> ApplyFunction(vecX<double> &mat, F func)
{
    vecX<double> res(mat.row, mat.col, 0);

    // if matrix transposed it doesn't affect on this function
    for (int i = 0; i < mat.len; i++)
    {
        res.push(i, func(mat.Get(i)));
    }
    return res;
}

// make vector to matrix by copy the vector along to column/row
// Take vector and integer: copy vector n times
vecX<double> CopyVector(vecX<double> vector, int n)
{
    /*
        => If given vector is column vector, it will expand towards column otherwise add row vectors
    */
    if (vector.col != 1 && vector.row != 1)
    {
        std::cout << "CopyVector work only on a vector" << std::endl;
        vector.print();
        std::exit(0);
    }

    // convert vector to row vector
    bool isRowVector = true;
    if (vector.col == 1)
    {
        vector.TR();
        isRowVector = false;
    }
    vecX<double> mat(n, vector.col, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < vector.col; j++)
            mat.push(i, j, vector.Get(j));
    }
    if (!isRowVector)
    {
        mat.TR();
    }
    return mat;
}