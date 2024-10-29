#include "blockchain.h"
#include <ctime>
#include <iostream>
#include <chrono>


std::mutex mtx;

void selectRandomTransactions(const std::vector<Transaction>& transactionPool, std::vector<Transaction>& selectedTransactions) {
    selectedTransactions.clear();
    std::vector<Transaction> tempPool = transactionPool;
    std::random_shuffle(tempPool.begin(), tempPool.end());
    int numTransactions = std::min(100, static_cast<int>(tempPool.size()));
    selectedTransactions.insert(selectedTransactions.end(), tempPool.begin(), tempPool.begin() + numTransactions);
}

void Blockchain::addBlock(Block block) {
    chain.push_back(block);
    for (const auto& tx : block.transactions) {
        utxoPool.applyTransaction(tx);
    }
}

Block Blockchain::createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash) {
    return Block(prevHash, transactions, difficulty); // Perduodam sudetinguma
}

std::string Blockchain::getLastBlockHash() const {
    if (chain.empty()) return {};
    return chain.back().getHash();
}

void Blockchain::mineBlock(Block& block) {
    block.mineBlock(difficulty); // Kvieciam Block klases metoda mineBlock, perduodami sudetinguma
}

void Blockchain::parallelMineBlocks(std::vector<Block>& candidateBlocks) {
    for (auto& block : candidateBlocks) {
        std::thread miner(&Blockchain::mineBlock, this, std::ref(block));
        miner.detach();
    }
}
void Blockchain::processTransactions(std::vector<Transaction>& transactions) {
    while (!transactions.empty()) {
        std::vector<Transaction> selectedTransactions;
        selectRandomTransactions(transactions, selectedTransactions);

        Block block = createBlock(selectedTransactions, getLastBlockHash());
        mineBlock(block);
        addBlock(block);

        block.displayBlockTransactions();

        transactions.erase(transactions.begin(), transactions.begin() + selectedTransactions.size());
        std::cout << transactions.size() << " transactions remaining in the pool." << std::endl;
    }
    std::cout << "All transactions have been processed and added to the blockchain." << std::endl;
}