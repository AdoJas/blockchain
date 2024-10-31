#ifndef BLOCKCHAIN_TRANSACTIONUTXO_H
#define BLOCKCHAIN_TRANSACTIONUTXO_H

#include <string>
#include <vector>
#include "user.h"
#include <iostream>
#include <unordered_map>

class Transaction {
public:
    Transaction(const std::string& txID, const std::string& sender, const std::string& receiver, double amount);
    std::string getTxID() const;
    std::string getSender() const;
    std::string getReceiver() const;
    double getAmount() const;
    const std::vector<std::string>& getInputs() const;
    const std::vector<std::string>& getOutputs() const;
    void addInput(const std::string& input);
    void addOutput(const std::string& output);

private:
    std::string txID;
    std::string sender;
    std::string receiver;
    double amount;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

void displayTransaction(const Transaction& tx);

class UTXOPool {
public:
    bool validateTransaction(const Transaction& tx) const;
    bool hasSufficientBalance(const std::string& owner, double amount) const;
    static bool verifyTransactionHash(const Transaction& tx);
    void applyTransaction(const Transaction& tx);
    static void initializePool(const std::vector<User>& users, UTXOPool& utxoPool);
    std::vector<std::pair<std::string, double>> getUTXOsForOwner(const std::string& owner) const;

private:
    std::unordered_map<std::string, std::pair<std::string, double>> utxos;
};

Transaction generateRandomTransaction(const std::vector<User>& users, UTXOPool& utxoPool);
void generateTransactions(int tranCount, const std::vector<User>& users, UTXOPool& utxoPool, std::vector<Transaction>& transactions);

#endif // BLOCKCHAIN_TRANSACTIONUTXO_H
