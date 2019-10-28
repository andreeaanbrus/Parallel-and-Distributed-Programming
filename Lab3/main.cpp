#include <iostream>
#include <chrono>
#include <future>
#include "Matrix.h"
#include "ThreadPool.h"

void addLineExecutor(int line, const Matrix& m1, const Matrix& m2, Matrix *res) {
    for(int j = 0; j < m1.getN(); j++)
        res->setData(line, j, m1.getData(line, j) + m2.getData(line, j));
}

void mulLineExcutor(int line, const Matrix& m1, const Matrix& m2, Matrix *res) {
    for(int j = 0; j < m2.getM(); j++) {
        int acc = 0;
        for (int k = 0; k < m1.getM(); k++)
            acc += m1.getData(line, k) * m2.getData(k, j);
        res -> setData(line, j, acc);
    }
}

void addAsync (const Matrix& m1, const Matrix& m2, Matrix& res) {
    std::vector<std::future<void>> f;
    f.reserve(m1.getN());
    for(int i = 0; i < m1.getN(); i++) {
        f.push_back(std::async(addLineExecutor, i, m1, m2, &res));
    }
    for(auto & future : f)
        future.wait();
}

void multiplicationAsync(const Matrix& m1, const Matrix& m2, Matrix &res) {
    std::vector<std::future<void>> f;
    f.reserve(m1.getN());
    for(int i = 0; i < m1.getN(); i++)
        f.push_back(std::async(mulLineExcutor, i, m1, m2, &res));
    for(auto & future : f)
        future.wait();
}


void addThreadPool(const Matrix& m1, const Matrix &m2, Matrix &res) {
    ThreadPool tp(12);
    std::vector<std::future<void>> f;
    f.reserve(m1.getN());
    for (int i = 0; i < m1.getN(); i++)
        f.push_back(tp.enqueue(addLineExecutor, i, m1, m2, &res));
    for  (int i = 0; i < m1.getN(); i++)
        f[i].wait();
}

void multiplicationThreadPool(const Matrix&m1, const Matrix &m2, Matrix &res) {
    ThreadPool tp(12);
    std::vector<std::future<void>> f;
    f.reserve(m1.getN());
    for (int i = 0; i < m1.getN(); i++)
        f.push_back(tp.enqueue(mulLineExcutor, i, m1, m2, &res));
    for(auto & future : f)
        future.wait();
}

void syncAdd(const Matrix &m1, const Matrix &m2, Matrix &res) {
    for (int i = 0; i < m1.getN(); i++)
        for (int j = 0; j < m1.getM(); j++)
            res.setData(i, j, m1.getData(i, j) + m2.getData(i, j));
}

bool compare (const Matrix& m1, const Matrix &m2) {
    for (int i = 0; i < m1.getN(); i++)
        for (int j = 0; j < m1.getM(); j++)
            if (m1.getData(i, j) != m2.getData(i, j))
                return false;
    return true;
}

int main() {

    Matrix m1("matrix1.txt");
    Matrix m2("matrix2.txt");
    Matrix syncRes (m1.getN(), m1.getM());
    syncAdd(m1, m2, syncRes);
    std::cout <<  "-------------ASYNC---------" << std::endl;
    Matrix resAddAsync (m1.getN(), m1.getM());
    auto startAddAsync = std::chrono::high_resolution_clock::now();
    addAsync(m1, m2, resAddAsync);
    auto stopAddAsync = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeAddAsync = stopAddAsync - startAddAsync;


    std::cout << "Time for adding 2 matrices using async + future: " << timeAddAsync.count() << '\n';

    Matrix resProdAsync (m1.getN(), m2.getM());
    auto startProdAsync = std::chrono::high_resolution_clock::now();
    multiplicationAsync(m1, m2, resProdAsync);
    auto stopProdAsync = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeProdAsync = stopProdAsync - startProdAsync;
    std::cout << "Time for multiplying 2 matrices using async + future: " << timeProdAsync.count() << '\n';
//    resProdAsync.printMatrix();

    std::cout << "----------THREAD POOL-----------\n";
    Matrix resAddThreadPool (m1.getN(), m1.getM());
    auto startAddThreadPool = std::chrono::high_resolution_clock::now();
     addThreadPool(m1, m2, resAddThreadPool);
    auto stopAddThreadPool = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeAddThreadPool = stopAddThreadPool - startAddThreadPool;
    std::cout << "Time for adding 2 matrices using thread pool " << timeAddThreadPool.count() << '\n';

    Matrix resMultiplyThreadPool (m1.getN(), m2.getM());
    auto startMulThreadPool = std::chrono::high_resolution_clock::now();
    multiplicationThreadPool(m1, m2, resMultiplyThreadPool);
    auto stopMulThreadPool = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeMulThreadPool = stopMulThreadPool - startMulThreadPool;
    std::cout << "Time for multiplying 2 matrices using thread pool " << timeMulThreadPool.count() << '\n';
    std::cout << compare (syncRes, resAddThreadPool);
    return 0;
}

