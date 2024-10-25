#ifndef BLOCKCHAIN_BLOCKCHAIN_H
#define BLOCKCHAIN_BLOCKCHAIN_H

#include <mutex>
#include <thread>
#include <vector>
#include "block.h"
#include "transactionUTXO.h"

extern std::mutex mtx;

class Blockchain {
private:
    std::vector<Block> chain;
    UTXOPool utxoPool;

public:
    void addBlock(Block block);
    Block createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash);
    std::string getLastBlockHash() const;

    void mineBlock(Block& block);
    void parallelMineBlocks(std::vector<Block>& candidateBlocks);
};

#endif // BLOCKCHAIN_BLOCKCHAIN_H
