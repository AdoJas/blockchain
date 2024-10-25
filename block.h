#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>
#include "transactionUTXO.h"

class Block {
public:
    Block(std::string previousHash, const std::vector<Transaction>& transactions);

    std::string getHash() const;
    std::string getPreviousHash() const;
    time_t getTimestamp() const;
    int getVersion() const;

    void displayBlockTransactions() const;
    void calculateHash();
    std::string hash;
    std::vector<Transaction> transactions;

private:

    std::string previousHash;
    time_t timestamp;
    int version;

};

#endif // BLOCK_H
