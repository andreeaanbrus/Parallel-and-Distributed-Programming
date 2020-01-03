#include <iostream>
#include <vector>
#include <future>

std::vector<int> array = {6, 4, 16, 10, 16, 14, 2, 8};
std::vector<int> result (array.size(), 0);
std::vector<int> result2  = array;
std::vector<int> result3  = array;

void dummySumsOfKFirst() {
//    the dynamic programming version
    result[0] = array[0];
    for (int i = 1; i < array.size(); i++)
        result[i] = array[i] + result[i - 1];
    std::cout << '\n';
}

void sumsOfKParallelized() {
//    sums of k parallelized
    int k;
    std::vector<std::future<void> > threads;
    for (k = 1; k < array.size(); k = k * 2) {
        threads.clear();
        for (int i = 2 * k - 1; i < array.size(); i += 2 * k )
            threads.push_back(std::async([](int i, int k) { result2[i] += result2[i - k]; }, i, k));
    }
    for(int i = 0; i < threads.size(); i++)
        threads[i].wait();
    for ( k = k / 4 ; k > 0; k = k / 2) {
        threads.clear();
        for (int i = 3 * k - 1; i < array.size(); i += 2 * k)
            threads.push_back(std::async([](int i, int k) { result2[i] += result2[i - k]; }, i, k));
    }
}

void sumsOfK() {
    int k;
    for (k = 1; k < array.size(); k = k * 2)
        for (int i = 2 * k - 1; i < array.size(); i+= 2 * k)
            result3[i] += result3[i - k];
    for (k = k / 4; k > 0; k = k / 2) {
        for (int i = 3 * k - 1; i < array. size(); i += 2 * k)
            result3[i] += result3[i - k];
    }

}

bool check(std::vector<int> v1, std::vector<int> v2) {
    for (int i = 0; i < v1.size(); i++)
        if (v1[i] != v2[i])
            return false;
    return true;
}

int main() {
    dummySumsOfKFirst();
    sumsOfK();
    sumsOfKParallelized();
    std::cout << check(result, result3);
    return 0;
}


