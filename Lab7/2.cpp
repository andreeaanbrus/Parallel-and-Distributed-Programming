
#include <vector>
#include <iostream>
#include <queue>
#include <cmath>
#include <chrono>
#include <thread>
#include <stdlib.h>

void print_queue(std::queue<int> q)
{
    while (!q.empty())
    {
        std::cout << q.front() << " ";
        q.pop();
    }
    std::cout << std::endl;
}

std::string generateBigNumber() {
    int digitNumber = 200;
    std::string number;
    for (int i = 0; i < digitNumber; i++) {
        int digit = rand() % 10;
        number += (std::to_string(digit));
    }
    return number;
}

std::vector<std::string> generateNumbers() {
    int  n = std::pow(2, 10);
    std::vector<std::string> numbers;
    for (int i = 0; i < n; i++)
        numbers.push_back(generateBigNumber());
    return numbers;
}

//std::vector<std::string> bignumbers = {
//        "1000000000000000",
//        "1000000000000000",
//        "1999999999999999",
//        "1",
//        "1000000000000000",
//        "1000000000000000",
//        "1999999999999999",
//        "1",
//};

auto bignumbers = generateNumbers();
std::vector<std::queue<int>> queuesParallel {bignumbers.size()};
std::queue<int> digits(std::string s) {
// transform string into queue
    std::queue<int> result;
    for(int i = s.size() - 1; i >= 0; i --) {
        char c = s[i];
        int digit = c - '0';
        result.push(digit);
    }
    return result;
}

int getNextDigit(std::queue<int>& number) {
//    get the next digit from the queue
    if (number.empty())
        return 0;
    int front = number.front();
    number.pop();
    return front;
}

std::queue<int> addTwo(std::queue<int>& left, std::queue<int>& right) {
//    add 2 big numbers represented as a queue
//    return also a queue
    std::queue<int> result;
    int carry = 0;
    while (!left.empty() || !right.empty()) {
        int sum = getNextDigit(left) + getNextDigit(right) + carry;
        result.push(sum % 10);
        carry = sum / 10;
    }
    if (carry != 0) {
        result.push(carry);
    }
    return result;
}

std::string getResult(std::queue<int> resultQueue) {
//    transform the result queue in a string number
    std::string result;
    while(!resultQueue.empty()) {
        result.insert(0, std::to_string(resultQueue.front()));
        resultQueue.pop();
    }
    return result;
}

std::string addSequential() {
    std::vector<std::queue<int>> queues {bignumbers.size()};
    for (int i = 0; i < bignumbers.size(); i++) {
        queues[i] = digits(bignumbers[i]);
    }
    for (int d = 0; d < int(log(queues.size())) + 1; d++) {
        for (int i = 0; i < queues.size(); i += int(pow(2, d + 1))) {

            queues[(int) (i + pow(2, d + 1) - 1)] =
                    addTwo(queues[(int) (i + pow(2, d) - 1)],
                           queues[(int) (i + pow(2, d + 1) - 1)]);
        }
    }
    return getResult(queues[queues.size() - 1]);
}

void worker (int i, int d ) {
    queuesParallel[(int) (i + pow(2, d + 1) - 1)] =
            addTwo(queuesParallel[(int) (i + pow(2, d) - 1)],
                   queuesParallel[(int) (i + pow(2, d + 1) - 1)]);
}

std::string addParallel() {

    std::vector<std::thread> threads;

    for (int i = 0; i < bignumbers.size(); i++) {
        queuesParallel[i] = digits(bignumbers[i]);
    }
    for (int d = 0; d < int(log(queuesParallel.size())) + 1; d++) {
        for (int i = 0; i < queuesParallel.size(); i += int(pow(2, d + 1))) {

            threads.push_back(std::thread(worker, i, d));
        }
    }

    for(int i = 0; i < threads.size(); i++)
        threads[i].join();
    return getResult(queuesParallel[queuesParallel.size() - 1]);
}

int main() {
    auto startSequential = std::chrono::high_resolution_clock::now();
    std::string sequentialResult = addSequential();
    auto stopSequential = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeSequential = stopSequential - startSequential;
    std::cout << "Time for sequential algorithm for adding n big numbers " << timeSequential.count() << '\n';
    std::cout << "The result is: " << sequentialResult << '\n';
    auto startParallel = std::chrono::high_resolution_clock::now();
    std::string parallelResult = addParallel();
    auto stopParallel = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeParallel = stopParallel - startParallel;
    std::cout << "Time for parallel algorithm for adding n big numbers " << timeParallel.count() << '\n';
    std::cout << "The result is: " <<  parallelResult << '\n';
}