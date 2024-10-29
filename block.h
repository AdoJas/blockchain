#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <ctime>
#include "transactionUTXO.h"

class Block {
public:
    Block(std::string previousHash, const std::vector<Transaction>& transactions, int difficulty);

    std::string getHash() const;
    std::string getPreviousHash() const;
    time_t getTimestamp() const;
    int getVersion() const;
    int getNonce() const;

    void displayBlockTransactions() const;
    void calculateHash();
    void mineBlock(int difficulty);

    std::string hash;
    std::vector<Transaction> transactions;

private:
    std::string previousHash;
    time_t timestamp;
    int version;
    int nonce;
    int difficulty;
};

#endif // BLOCK_H
