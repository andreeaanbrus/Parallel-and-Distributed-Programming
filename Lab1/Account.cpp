//
// Created by andreea on 04.10.2019.
//

#include "Account.h"

Account::Account(int balance, int id) {
    this->balance = balance;
    this->mutex = new std::mutex;
    this->initialBalance = balance;
    this->id = id;
}

int Account::getBalance() {
    return this->balance;
}

void Account::send(Transaction t) {
    this -> sendTransactions.push_back(t);
    this -> balance -= t.getAmount();
}

void Account::receive(Transaction t) {
    this -> receiveTransactions.push_back(t);
    this -> balance += t.getAmount();
}

bool Account::validTransaction(Transaction t) {
    return this -> balance - t.getAmount() > 0;

}

void Account::lock() {
    this->mutex->lock();
}

void Account::unlock() {
    this->mutex->unlock();
}

bool Account::checkConsistency() {
    int currentBalance = this->initialBalance;
    for (auto& i : this->sendTransactions) {
        currentBalance -= i.getAmount();
    }
    for (auto& i : this->receiveTransactions) {
        currentBalance += i.getAmount();
    }
    return currentBalance == this->balance;
}

