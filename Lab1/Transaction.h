//
// Created by andreea on 04.10.2019.
//

#ifndef LAB1_TRANSACTION_H
#define LAB1_TRANSACTION_H


class Transaction {
private:
    static int serial_no;
private:
    int id;
    int fromID;
    int toID;
    int amount;
public:
    Transaction(int, int, int);
    int getFromId();
    int getToId();
    int getAmount();
    int getId();

};


#endif //LAB1_TRANSACTION_H
