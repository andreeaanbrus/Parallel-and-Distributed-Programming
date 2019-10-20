//
// Created by andreea on 20.10.2019.
//

#ifndef LAB2_MATRIX_H
#define LAB2_MATRIX_H
#include <vector>
#include <string>

class Matrix {
private:
    std::vector<std::vector<long long> > data;
    int N{}, M{};
public:
    explicit Matrix(const std::string &);
    Matrix (int N, int M);
    void printMatrix();
    int getN() const {return N;}
    int getM() const {return M;}
    int getData(int i, int j) const {return data[i][j];}
    void setData(int i, int j, long long value) {this->data[i][j] = value;}
    Matrix sum(Matrix & other);
    Matrix prod(Matrix & other);
};


#endif //LAB2_MATRIX_H
