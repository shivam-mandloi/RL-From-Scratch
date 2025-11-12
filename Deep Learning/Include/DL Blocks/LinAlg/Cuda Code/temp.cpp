#include <iostream>
#include "VexCuda.hpp"
#include "vex.hpp"


int main() {
    vex<float> A(5, 1, 6);
    vex<float> B(5, 1, 6);
    float C[5];

    vecAdd(A.arr, B.arr, C, A.len);

    for(int i = 0; i < A.len; i++)
        std::cout << C[i] << " ";
    std::cout << std::endl;
}
