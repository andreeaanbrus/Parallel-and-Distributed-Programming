//
// Created by andreea on 20.10.2019.
//

#include <fstream>
#include <iostream>
#include "Matrix.h"

Matrix::Matrix(const std::string &fileName) {
    std::string filePath = fileName;

    std::ifstream matrixFile(filePath);
    matrixFile >> N >> M;
    data = std::vector<std::vector<long long>>(static_cast<unsigned long>(N), std::vector<long long>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            matrixFile >> this->data[i][j];
    }
    matrixFile.close();
}

Matrix::Matrix(int N, int M) {
    this->N = N;
    this->M = M;
    data = std::vector<std::vector<long long>>(static_cast<unsigned long>(N), std::vector<long long>(M, 0));
}

void Matrix::printMatrix() {
    for (const auto &i : this->data) {
        for (const auto &j : i)
            std::cout << j << ' ';
        std::cout << '\n';
    }
}

void Matrix::addValue(int i, int j, long value) {
    this->data[i][j] += value;
}
