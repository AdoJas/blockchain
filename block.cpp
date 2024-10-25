#include "block.h"
#include <iostream>
#include <ctime>

const int DEFAULT_BLOCK_VERSION = 1;

Block::Block(std::string previousHash, const std::vector<Transaction>& transactions)
        : previousHash(std::move(previousHash)), transactions(transactions), version(DEFAULT_BLOCK_VERSION) {
    timestamp = std::time(nullptr);
    calculateHash();
}

int Block::getVersion() const {
    return version;
}

void Block::displayBlockTransactions() const {
    for (const auto& tx : transactions) {
        displayTransaction(tx);
    }
}

std::string Block::getHash() const {
    return hash;
}

void Block::calculateHash() {
    //TODO: Implementuoti hashavimo algoritma pilnai
    // Laikinas Sprendimas
    hash = std::to_string(timestamp) + previousHash;
}
