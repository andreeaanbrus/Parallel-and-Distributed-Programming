//
// Created by andreea on 04.10.2019.
//

#include "Transaction.h"

int Transaction::serial_no = 0;

Transaction::Transaction(int fromID, int toID, int amount) {
    this -> id =  serial_no++;
    this -> fromID = fromID;
    this -> toID = toID;
    this -> amount = amount;
}

int Transaction::getId() {
    return this -> id;
}

int Transaction::getFromId() {
    return this -> fromID;
}

int Transaction::getToId() {
    return this -> toID;
}

int Transaction::getAmount() {
    return this -> amount;
}


