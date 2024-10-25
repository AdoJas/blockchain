#include "blockchain.h"
#include <ctime>
#include <iostream>
#include <chrono>

std::mutex mtx;

void Blockchain::addBlock(Block block) {
    chain.push_back(block);
    for (const auto& tx : block.transactions) {
        utxoPool.applyTransaction(tx);
    }
}

Block Blockchain::createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash) {
    return Block(prevHash, transactions);
}

std::string Blockchain::getLastBlockHash() const {
    if (chain.empty()) return {};
    return chain.back().getHash();
}

void Blockchain::mineBlock(Block& block) {
    while (block.getHash().substr(0, 2) != "00") {
        block.calculateHash();  // Pakeiciam tam, kad galetume ieskoti hasho
    }
}

void Blockchain::parallelMineBlocks(std::vector<Block>& candidateBlocks) {
    for (auto& block : candidateBlocks) {
        std::thread miner(&Blockchain::mineBlock, this, std::ref(block));
        miner.detach();
    }
}
