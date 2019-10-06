//
// Created by andreea on 04.10.2019.
//

#ifndef LAB1_BANK_H
#define LAB1_BANK_H

#include <vector>
#include "Account.h"
#include "Transaction.h"
#include <thread>
class Bank {
private:
    std::vector<Account> accounts;
    std::vector<std::thread> threads;
    int maxAmount;
public:
    Bank();
    std::vector<Account> getAll();
    void makeTransaction(Transaction t);
    void run();
    void Executor();
    bool checkConsistency();

    int noOfAccounts;
};


#endif //LAB1_BANK_H
