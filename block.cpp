#include "block.h"
#include "hash.h"
#include <iostream>
#include <ctime>

const int DEFAULT_BLOCK_VERSION = 1;

Block::Block(std::string previousHash, const std::vector<Transaction>& transactions, int difficulty)
        : previousHash(std::move(previousHash)), transactions(transactions), version(DEFAULT_BLOCK_VERSION), nonce(0), difficulty(difficulty) {
    timestamp = std::time(nullptr);
    calculateHash();
}

int Block::getVersion() const {
    return version;
}

int Block::getNonce() const {
    return nonce;
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

    std::string data = std::to_string(timestamp) + previousHash + std::to_string(nonce) + std::to_string(difficulty);
    for (const auto& tx : transactions) {
        data += tx.txID;
    }
    hash = generateCustomHash(data);
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0'); // Sukuriame targeta, kurio priekyje bus nuliu tiek, kiek buvo nurodyta sudetingumo reikalavime
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        calculateHash();
    }
}
