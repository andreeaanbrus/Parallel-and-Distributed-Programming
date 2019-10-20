//
// Created by andreea on 04.10.2019.
//

#include "Bank.h"
#include <iostream>
#include <random>

int generateRange(int x, int y) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uid(x, y);
    return uid(rng);
}

Bank::Bank() {
    noOfAccounts = generateRange(20, 10000); //between 20 and 10000
    maxAmount = 10000;
    for (int i = 0; i < noOfAccounts; i++) {
        const int amount = generateRange(1, maxAmount); //between 1 and 10000
        this->accounts.emplace_back(amount, amount);
    }
}

std::vector<Account> Bank::getAll() {
    return accounts;
}

void Bank::makeTransaction(Transaction t) {
        accounts[t.getFromId()].lock();
        if (accounts[t.getFromId()].validTransaction(t)) {
            accounts[t.getFromId()].send(t);
            accounts[t.getFromId()].unlock();
            accounts[t.getToId()].lock();
            accounts[t.getToId()].receive(t);
            accounts[t.getToId()].unlock();
            return;
        }
        accounts[t.getFromId()].unlock();
}

void Bank::Executor() {
    const int noOfTransactions = 1000;
    for (int i = 0; i < noOfTransactions; ++i) {
        int fromId = generateRange(0, noOfAccounts - 1);
        int toId = generateRange(0, noOfAccounts - 1);
        int amount = generateRange(0, maxAmount);
        Transaction t (fromId, toId, amount);
//        printf("%d gives %d this much: %d \n", fromId, toId, amount);
        makeTransaction(t);
    }
}

bool Bank::checkConsistency() {
    for (auto& account : accounts) {
        if (!account.checkConsistency())
            return false;
    }
    return true;
}
void Bank::run() {
    const int noOfThreads = 8;
    for (int bucket = 0; bucket < 10; ++bucket) {
        for(int i = 0; i < noOfThreads; i++) {
            threads.emplace_back(&Bank::Executor, this);
        }
        for (auto& i : threads)
            i.join();
        threads.clear();
        std::cout << (checkConsistency() ? "TRUE" : "FALSE") << " " << bucket << '\n';
//            for ( int i = 0; i < noOfAccounts; i++) {
//        std::cout << getAll()[i].getBalance() << ' ';
//    }
    }
}