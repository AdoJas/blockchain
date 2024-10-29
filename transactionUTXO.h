#ifndef BLOCKCHAIN_TRANSACTIONUTXO_H
#define BLOCKCHAIN_TRANSACTIONUTXO_H

#include <string>
#include <vector>
#include "user.h"
#include <iostream>
#include <unordered_map>

struct Transaction {
    std::string txID;
    std::string sender;
    std::string receiver;
    double amount;
    std::vector<std::string> inputs;   // Buvusiu UTXO ID
    std::vector<std::string> outputs;  // Nauji UTXO ID
};

void displayTransaction(const Transaction& tx);


class UTXOPool {
public:
    std::unordered_map<std::string, std::pair<std::string, double>> utxos; // Priskiriam UTXO ID - (savininkas, suma)

    bool validateTransaction(const Transaction& tx);
    void applyTransaction(const Transaction& tx);

};

void initializeUTXOPool(const std::vector<User>& users, UTXOPool& utxoPool);
// Generuojam random pervedimus
Transaction generateRandomTransaction(const std::vector<User>& users, UTXOPool& utxoPool);

// Bulk set transakcijos
void transactionGeneration(int tranCount, const std::vector<User>& users, UTXOPool& utxoPool, std::vector<Transaction>& transactions);

#endif // BLOCKCHAIN_TRANSACTIONUTXO_H
