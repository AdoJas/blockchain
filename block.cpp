#include "block.h"
#include "hash.h"
#include <iostream>
#include <ctime>

const int DEFAULT_BLOCK_VERSION = 1;

Block::Block(std::string previousHash, const std::vector<Transaction>& transactions, int difficulty)
        : previousHash(std::move(previousHash)), transactions(transactions), version(DEFAULT_BLOCK_VERSION), nonce(0), difficulty(difficulty) {
    timestamp = std::time(nullptr);

    std::vector<std::string> transactionIDs;
    for (const auto& tx : transactions) {
        transactionIDs.push_back(tx.txID);
    }

    merkleRoot = calculateMerkleRoot(transactionIDs);

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
    std::cout << "\n===============================================================" << std::endl;
}

std::string Block::getHash() const {
    return hash;
}

void Block::calculateHash() {
    std::string data = std::to_string(timestamp) + previousHash + std::to_string(nonce) + std::to_string(difficulty) + merkleRoot;
    hash = generateCustomHash(data);
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0'); // Sukuriame targeta, kurio priekyje bus nuliu tiek, kiek buvo nurodyta sudetingumo reikalavime
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        calculateHash();
    }
}
std::string Block::calculateMerkleRoot(const std::vector<std::string>& transactionIDs) {
    if (transactionIDs.empty()) return ""; // Jei nera transakciju, graziname tuscia stringa

    // Pradedame nuo transakciju id sudejimo i merkle medzio "lapus"
    std::vector<std::string> merkleTree = transactionIDs;

    // Standartine merkleTree medzio implementacija
    while (merkleTree.size() > 1) {
        if (merkleTree.size() % 2 != 0) {
            merkleTree.push_back(merkleTree.back());
        }

        std::vector<std::string> newLevel;
        for (size_t i = 0; i < merkleTree.size(); i += 2) {
            std::string combinedHash = generateCustomHash(merkleTree[i] + merkleTree[i + 1]);
            newLevel.push_back(combinedHash);
        }
        merkleTree = newLevel; // pereinam i kita lygi
    }

    return merkleTree[0]; // Merkle tree root/pradinis hashas
}