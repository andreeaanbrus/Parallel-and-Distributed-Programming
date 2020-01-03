#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include "Matrix.h"
#include <thread>


void mulSync(const Matrix&m1, const Matrix &m2, Matrix &res ) {
    for(int i = 0; i < m1.getN(); i++) {
        for (int j = 0; j < m2.getM(); j++) {
            int acc = 0;
            for (int k = 0; k < m2.getN(); k++)
                acc += m1.getData(i, k) * m2.getData(k, j);
            res.setData(i, j, acc);
        }
    }
}
std::queue<std::tuple<int, int, long>> queue;
std::mutex mutex;
std::condition_variable cv;
bool finished;

Matrix m1("matrix1.txt");
Matrix m2("matrix2.txt");
Matrix m3("matrix3.txt");
Matrix res1(m1.getN(), m2.getM());
Matrix res2(m1.getN(), m3.getM());
Matrix res3(m1.getN(), m2.getN());
inline void producer(int, int);
inline void consumer();
bool compare (const Matrix& m1, const Matrix &m2) {
    for (int i = 0; i < m1.getN(); i++)
        for (int j = 0; j < m1.getM(); j++)
            if (m1.getData(i, j) != m2.getData(i, j))
                return false;
    return true;
}
int main() {
    int threadNumber = 20;

    std::vector<std::thread> producers, consumers;

    for (int i = 0; i <  threadNumber; ++i) {
        producers.emplace_back(producer, i, threadNumber);
    }
    for (int i = 0; i < threadNumber; ++i) {
        consumers.emplace_back(consumer);
    }
    auto start = std::chrono::high_resolution_clock::now();

    for (auto &producer : producers) {
        producer.join();
    }
    {
        std::lock_guard<std::mutex> lk(mutex);
        finished = true;
    }
    cv.notify_all();
    for (auto &consumer : consumers) {
        consumer.join();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = finish - start;
    std::cout << "With " << threadNumber << " threads " << totalTime.count() << '\n';

//    res2.printMatrix();
    mulSync(m1, m2, res1);
    mulSync(res1, m3, res3);
//    res3.printMatrix();
    std::cout << compare(res2, res3);
}


inline void producer (int line, int T) {
    std::cout << "in producer " << line << ' ' << T << '\n';
    for (int i = line; i < m1.getN(); i += T) {
        for (int j = 0; j < m2.getM(); j++) {
            int acc = 0;
            for (int k = 0; k < m1.getM(); k++)
                acc += m1.getData(i, k) * m2.getData(k, j);
            std::unique_lock<std::mutex> uniqueLock(mutex);
            queue.push(std::make_tuple(i, j, acc));
            cv.notify_one();
        }
    }
}


inline void consumer() {
    std::cout << "in consumer\n";
    while(true) {
        std::unique_lock<std::mutex> uniqueLock(mutex);
        cv.wait(uniqueLock, [] {
            return finished || !queue.empty();
        });
        if (finished)
            break;
        std::tuple<int, int, long> front = queue.front();
        queue.pop();
        int i = std::get<0> (front);
        int j = std::get<1> (front);
        long tempAcc = std::get<2> (front);
        for (int k = 0; k < m3.getM(); k++) {
            res2.addValue(i, k, tempAcc * m3.getData(j, k));
        }


    }
}
