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
    Blockchain(int difficultyLevel);

    Block& getLastBlock();
    Block createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash);

    void addBlock(Block block);
    void getBlock(int index);
    void displaySpecificTransaction(int blockIndex, int transactionIndex);
    void displayBlockchain() const;
    void mineBlock(Block& block);

    void parallelMineBlocks(std::vector<Block>& candidateBlocks);
    void processTransactions(std::vector<Transaction>& transactions);
    std::string getLastBlockHash() const;
    unsigned long long getTransactionCount(int index) const;
    unsigned long long size() const { return chain.size(); };
    bool isEmpty() const;
};

#endif // BLOCKCHAIN_BLOCKCHAIN_H
