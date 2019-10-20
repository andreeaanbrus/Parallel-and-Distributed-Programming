#include <iostream>
#include <vector>
#include "Matrix.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Matrix m1("matrix1.txt");
    Matrix m2("matrix2.txt");
    Matrix result = m1.sum(m2);
    m1.printMatrix();
    m2.printMatrix();
//    result.printMatrix();
    Matrix prod = m2.prod(m1);
    prod.printMatrix();
    return 0;
}