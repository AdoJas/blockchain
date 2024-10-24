//
// Created by adoma on 10/24/2024.
//

#ifndef BLOCKCHAIN_TRANSACTIONUTXO_H
#define BLOCKCHAIN_TRANSACTIONUTXO_H

#include <string>
#include <vector>
#include "user.h"
#include <iostream>
#include "unordered_map"
struct Transaction {
    std::string txID;
    std::string sender;
    std::string receiver;
    double amount;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

Transaction generateRandomTransaction(const std::vector<User>& users);

void displayTransaction(const Transaction& tx);

class UTXOPool {
public:
    // UTXO -> (owner, amount)
    std::unordered_map<std::string, std::pair<std::string, double>> utxos;

    bool validateTransaction(const Transaction& tx);

    void applyTransaction(const Transaction& tx);
};

void transactionGeneration(int tranCount, std::vector<User> users, std::vector<Transaction> transactions);
#endif //BLOCKCHAIN_TRANSACTIONUTXO_H
