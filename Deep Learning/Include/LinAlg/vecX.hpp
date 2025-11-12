#pragma once

#include <iostream>
#include <cstddef>

template <typename T>
class vecX
{
public:
    T *arr;
    int len, row, col;
    bool isTransposed = false;
    vecX() : row(0), col(0), len(0), arr(nullptr) {}
    vecX(size_t _row, size_t _col, T defaultData)
    {
        row = _row;
        col = _col;
        arr = new T[row * col];
        len = row * col;
        std::fill(arr, arr + len, defaultData);
    }
    // vecX(size_t _row, size_t _col) : vecX(_row, _col, T{}) {}
    vecX(size_t _col, T defaultData) : vecX(1, _col, defaultData) {}
    vecX(size_t _col) : vecX(1, _col, T{}) {}

    vecX(const vecX<T> &newArr) : row(newArr.row), col(newArr.col), len(newArr.len)
    {
        arr = new T[len];
        std::copy(newArr.arr, newArr.arr + newArr.len, arr);
    }

    vecX(std::initializer_list<T> list) : row(1), col(list.size()), len(list.size())
    {
        arr = new T[len];
        std::copy(list.begin(), list.end(), arr);
    }

    vecX<T> &operator=(const vecX<T> &other)
    {        
        if (this != &other)
        {
            delete[] arr;
            len = other.len;
            row = other.row;
            col = other.col;
            isTransposed = other.isTransposed;
            arr = new T[len];
            std::copy(other.arr, other.arr + len, arr);
        }
        return *this;
    }

    ~vecX()
    {
        delete[] arr;
    }

    T Get(int x)
    {
        return arr[x];
    }

    T Get(int i, int j)
    {
        return isTransposed ? Get(row * j + i) : Get(col * i + j);
    }

    void push(int i, T data)
    {
        arr[i] = data;
    }

    void push(int i, int j, T data)
    {
        if (isTransposed)
            push(j * row + i, data);
        else
            push(i * col + j, data);
    }

    vecX<int> size()
    {
        vecX<int> size(2, 0);
        size.push(0, row);
        size.push(1, col);
        return size;
    }

    void TR()
    {
        isTransposed = !isTransposed;
        int temp = row;
        row = col;
        col = temp;
    }

    void print()
    {
        try
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    std::cout << Get(i, j) << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
};
