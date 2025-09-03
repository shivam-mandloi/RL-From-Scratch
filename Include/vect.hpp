#pragma once

#include <iostream>
#include <cstddef>

template <typename T>
class vect
{
public:
    T *arr;
    int len, row, col;
    vect(size_t _row, size_t _col, T defaultData)
    {
        row = _row;
        col = _col;
        arr = new T[row * col];
        len = row * col;
        std::fill(arr, arr + len, defaultData);
    }
    vect(size_t _row, size_t _col) : vect(_row, _col, T{}) {}
    vect(size_t _col, T defaultData) : vect(1, _col, defaultData) {}
    vect(size_t _col) : vect(1, _col, T{}) {}
    
    vect(const vect<T> &newArr): row(newArr.row), col(newArr.col), len(newArr.len)
    {
        arr = new T[len];
        std::copy(newArr.arr, newArr.arr + newArr.len, arr);
    }

    vect(std::initializer_list<T> list) : row(1), col(list.size()), len(list.size())
    {
        arr = new T[len];
        std::copy(list.begin(), list.end(), arr);
    }

    vect<T>& operator=(const vect<T> &other)
    {
        if(this != &other)
        {
            delete[] arr;
            len = other.len;
            row = other.row;
            col = other.col;
            arr = new T[len];
            std::copy(other.arr, other.arr + len, arr);            
        }
        return *this;
    }

    ~vect()
    {        
        delete[] arr;
    }

    T Get(int x)
    {
        return arr[x];
    }

    T Get(int i, int j)
    {
        return Get(row * i + j);
    }

    void push(int i, T data)
    {
        arr[i] = data;
    }

    void push(int i, int j, T data)
    {
        push(i * row + j, data);
    }

    vect<int> size()
    {
        vect<int> size(2, 0);
        size.push(0, row);
        size.push(1, col);
        return size;
    }

    void print()
    {
        for(int i = 0; i < row; i++)
        {            
            for(int j = 0; j < col; j++)
            {
                std::cout << Get(i, j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};