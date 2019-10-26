#include <iostream>
#include <future>
#include "Matrix.h"

void multiplyMatrixNoThreads(Matrix *m1, Matrix *m2, Matrix *res) {
    for (int i = 0; i < m1->getN(); i++)
        for(int j = 0; j < m2 ->getM(); j++) {
            int acc = 0;
            for(int k = 0; k < m1->getM(); k++)
                acc += m1->getData(i, k)* m2->getData(k, j);
            res->setData(i, j, acc);
        }
};

void addMatrixByElement(Matrix* m1, Matrix* m2, Matrix *res, int i, int j) {
    res->setData(i, j, m1->getData(i, j) + m2->getData(i, j));
}

void addMatrixNoThreads(Matrix* m1, Matrix *m2, Matrix *res) {
    for (int i = 0; i < m1->getN(); i++)
        for (int j = 0; j < m1->getM(); j++)
            res -> setData(i, j, m1->getData(i, j) + m2 ->getData (i, j));
}

void addMatrixByLineThreads (Matrix *m1, Matrix *m2, Matrix * res, int i) {
    for(int j = 0; j < m1->getM(); j++)
        res -> setData(i, j, m1->getData(i, j) + m2 -> getData(i, j));
}

void multiplicationMatrixByLines(Matrix *m1, Matrix *m2, Matrix *res, int i) {
    for (int j = 0; j < m2->getM(); j++) {
        int acc = 0;
        for (int k = 0; k < m2->getN(); k++)
            acc += m1->getData(i, k) * m2 -> getData(k, j);
        res->setData(i, j, acc);
    }
}

void multiplicationByElements(Matrix *m1, Matrix *m2, Matrix *res, int i, int j) {
    int acc = 0;
    for(int k = 0; k < m2->getM(); k++) {
        acc += m1->getData(i, k) * m2 -> getData(k, j);
    }
    res ->setData(i, j, acc);
}

int main() {
    Matrix m1("matrix1.txt");
    Matrix m2("matrix2.txt");

    if (!(m1.getN() == m2.getN() && m1.getM() == m2.getM())) {
        std::cout << "Invalid matrix for addition";
        return 1;
    }

    if (m1.getM() != m2.getN()) {
        std::cout << "Invalid matrix for multiplication";
    }

    Matrix res(m1.getN(), m2.getM());
    Matrix res2(m1.getN(), m2.getM());
    Matrix res3(m1.getN(), m2.getM());

    std::cout << "--------------ADDITION-------------\n";

//    --------------- 1 THREAD -----------------
    auto startAddNoThreads = std::chrono::system_clock::now();
    addMatrixNoThreads(&m1, &m2, &res);
    auto endAddNoThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsAddNoThreads = endAddNoThreads - startAddNoThreads;
    std::cout << "Time for addition of matrix 1 thread " << elapsed_secondsAddNoThreads.count() << '\n';
    res.printMatrix();

//    --------------- NO OF LINES THREADS -----------------
    auto startAddByLinesThreads = std::chrono::system_clock::now();
    std::vector<std::thread*> threads_lines;
    for (int i = 0; i < m1.getN(); i++)
        threads_lines.push_back(new std::thread(&addMatrixByLineThreads, &m1, &m2, &res3, i));
    for (int i = 0; i < m1.getN(); i++)
        threads_lines[i]->join();
    auto endAddByLinesThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsAddByLinesThreads = endAddByLinesThreads - startAddByLinesThreads;
    std::cout << "Time for addition of matrix n threads " << elapsed_secondsAddByLinesThreads.count() << '\n';
    res3.printMatrix();
//    --------------- NO OF ELEMENTS THREADS -----------------
    auto startAddByElemThreads = std::chrono::system_clock::now();
    std::vector<std::thread*> threads;
    for(int i = 0; i < m1.getN(); i++)
        for (int j = 0; j < m1.getM(); j++)
            threads.push_back(new std::thread(&addMatrixByElement, &m1, &m2, &res2, i, j));
    for (int i = 0; i < m1.getN() * m1.getN(); i++)
        threads[i]->join();
    auto endAddByElemThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsAddByElemThreads = endAddByElemThreads - startAddByElemThreads;
    std::cout << "Time for addition of matrix n*n threads " << elapsed_secondsAddByElemThreads.count() << '\n';
    res2.printMatrix();

    std::cout << "--------------MULTIPLICATION--------------\n";

    Matrix res4(m1.getN(), m2.getM());
    Matrix res5(m1.getN(), m2.getM());
    Matrix res6(m1.getN(), m2.getM());
//    ---------- 1 THREAD -----------
    auto startMulNoThreads = std::chrono::system_clock::now();
    multiplyMatrixNoThreads(&m1, &m2, &res4);
    auto endMulNoThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsMulNoThreads = endMulNoThreads - startMulNoThreads;
    std::cout << "Time for multiplication of matrix 1 thread " << elapsed_secondsMulNoThreads.count() << '\n';
    res4.printMatrix();
//    ----------- NO OF LINES THREADS -----------

    auto startMulLinesThreads = std::chrono::system_clock::now();
    std::vector<std::thread*> threads_mul_lines;
    for (int i = 0; i < m1.getN(); i++)
        threads_mul_lines.push_back(new std::thread(&multiplicationMatrixByLines, &m1, &m2, &res5, i));
    for(int i = 0; i < m1.getN(); i++)
        threads_mul_lines[i]->join();
    auto endMulLinesThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsMulLinesThreads = endMulLinesThreads - startMulLinesThreads;
    std::cout << "Time for multiplication of matrix n threads " << elapsed_secondsMulLinesThreads.count() << '\n';
    res5.printMatrix();


    auto startMulElementsThreads = std::chrono::system_clock::now();
    std::vector<std::thread*> threads_mul_elements;
    for (int i = 0; i < m1.getN(); i++)
        for(int j = 0; j < m2.getM(); j++)
            threads_mul_elements.push_back(new std::thread(&multiplicationByElements, &m1, &m2, &res6, i, j));
    for(int i = 0; i < m1.getN(); i++)
        threads_mul_elements[i]->join();
    auto endMulElementsThreads = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secondsMulElementsThreads = endMulElementsThreads - startMulElementsThreads;
    std::cout << "Time for multiplication of matrix n threads " << elapsed_secondsMulElementsThreads.count() << '\n';
    res6.printMatrix();

    return 0;

}


