#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H

#include <mutex>
#include <thread>
#include <vector>
#include "block.h"
#include "transactionUTXO.h"
#include <algorithm>

extern std::mutex mtx;

class Blockchain {
private:
    std::vector<Block> chain;
    UTXOPool utxoPool;
    int difficulty;

public:
    Blockchain(int difficultyLevel) : difficulty(difficultyLevel) {};
    void addBlock(Block block);
    Block createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash);
    std::string getLastBlockHash() const;
    unsigned long long getTransactionCount(int index) const;
    Block& getLastBlock();
    void displayBlockchain() const;
    bool isEmpty() const;
    void getBlock(int index);
    void displaySpecificTransaction(int blockIndex, int transactionIndex);
    unsigned long long size() const { return chain.size(); };
    void mineBlock(Block& block);
    void parallelMineBlocks(std::vector<Block>& candidateBlocks);
    void processTransactions(std::vector<Transaction>& transactions);
};

#endif // BLOCKCHAIN_BLOCKCHAIN_H
