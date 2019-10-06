//
// Created by andreea on 04.10.2019.
//

#ifndef LAB1_ACCOUNT_H
#define LAB1_ACCOUNT_H


#include "Transaction.h"
#include <vector>
#include <mutex>

class Account {
private:
    int initialBalance;
    int id;
    int balance;
    std::mutex* mutex;
    std::vector<Transaction> sendTransactions;
    std::vector<Transaction> receiveTransactions;

public:
    Account(int, int);
    int getBalance();
    void send(Transaction t);
    void receive(Transaction t);
    bool validTransaction(Transaction t);
    bool checkConsistency();
    void lock();
    void unlock();
};


#endif //LAB1_ACCOUNT_H
