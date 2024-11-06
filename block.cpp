#include "block.h"
#include "hash.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <omp.h>

Block::Block(const std::string& prevHash, int diff, int ver) // Bloko konstruktorius
        : version(ver), prevBlockHash(prevHash), difficulty(diff), nonce(0) {
    auto now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y/%m/%d %H:%M:%S");
    timestamp = oss.str();
    calculateMerkleRoot();
}

std::string Block::calculateHash() const { // Bloko hash skaiciavimas, naudojant visus bloko header duomenis
    return generateCustomHash(std::to_string(version) + prevBlockHash + merkleRootHash +
                              std::to_string(nonce) + timestamp + std::to_string(difficulty));
}

//bool Block::mineBlock() { // Bloko kasimas, ieškant tinkamo nonce, kad bloko hash prasidėtų reikiamu kiekiu nuliu
//    std::string target(difficulty, '0');
//    while (calculateHash().substr(0, difficulty) != target) {
//        nonce++;
//    }
//    return true;
//}
bool Block::mineBlock() {
    std::string target(difficulty, '0');
    bool found = false;                   // Ar blokas iskastas
    unsigned int successfulNonce = 0;     // Nonce reiksme
    std::string successfulHash;           // Storinam gera Hasha

#pragma omp parallel
    {
        unsigned int localNonce;          // nonce reiksme kiekvienam threadui
        std::string localHash;            // lokali hasho reiksme kiekvienam threadui

#pragma omp for nowait
        for (localNonce = 0; localNonce < UINT_MAX; ++localNonce) {
            if (found) continue;  // Stabdo darba jei jau rado tinkama nonce

            localHash = generateCustomHash(prevBlockHash + merkleRootHash + std::to_string(localNonce) + timestamp);

            if (localHash.substr(0, difficulty) == target) {
#pragma omp critical
                {
                    if (!found) {
                        found = true;
                        successfulNonce = localNonce;
                        successfulHash = localHash;
                        std::cout << "Block mined by thread " << omp_get_thread_num()
                                  << " with nonce: " << successfulNonce << "\n";
                    }
                }
            }
        }
    }

    if (found) {
        nonce = successfulNonce;
        blockHash = successfulHash;  // storina gera Hasha
        std::cout << "Block hash after mining: " << blockHash << "\n";  // Parodo gera Hasha del aiskumo
        return true;
    }

    std::cout << "Mining failed: No valid nonce found.\n";
    return false;
}

// grazina bloko hash
std::string Block::getHash() const {
    return blockHash;
}

// Grazina bloko, kuris buvo pries tai, hash
const std::string& Block::getPrevBlockHash() const {
    return prevBlockHash;
}

// Grazina bloko kurimo laika
const std::string& Block::getTimestamp() const {
    return timestamp;
}

// Grazina nonce, su kuriuo buvo rastas bloko hashas
unsigned int Block::getNonce() const {
    return nonce;
}

// Funkcija, kuri i bloka prideda transakcija
void Block::addTransaction(const Transaction& tx) {
    transactions.push_back(tx);
    calculateMerkleRoot();
}

// Grazina bloko transakcijas
const std::vector<Transaction>& Block::getTransactions() const {
    return transactions;
}

// Funkcija, kuri skaiciuoja bloko merkle root
void Block::calculateMerkleRoot() {
    std::vector<std::string> transactionHashes;
    for (const auto& tx : transactions) {
        transactionHashes.push_back(tx.getTransactionID());
    }

    while (transactionHashes.size() > 1) {
        if (transactionHashes.size() % 2 != 0) {
            transactionHashes.push_back(transactionHashes.back());
        }

        std::vector<std::string> newLevel;
        for (size_t i = 0; i < transactionHashes.size(); i += 2) {
            std::string combinedHash = generateCustomHash(transactionHashes[i] + transactionHashes[i + 1]);
            newLevel.push_back(combinedHash);
        }
        transactionHashes = newLevel;
    }

    merkleRootHash = transactionHashes.empty() ? "" : transactionHashes[0];
}

// Parodo bloko header informacija, jei showCurrentHashAfterMining = true, parodomas ir bloko hash
void Block::displayHeader(bool showCurrentHashAfterMining) const {
    std::cout << "==================== Block Header ====================\n";
    if (showCurrentHashAfterMining) {
        std::cout << "Current Block Hash : " << blockHash << "\n";  // Use stored blockHash
    }
    std::cout << "Previous Block Hash: " << prevBlockHash << "\n";
    std::cout << "Merkle Root Hash   : " << merkleRootHash << "\n";
    std::cout << "Timestamp          : " << timestamp << "\n";
    std::cout << "Nonce              : " << nonce << "\n";
    std::cout << "======================================================\n";
}

// Parodo visa bloka, su visais transakcijos duomenimis
void Block::display() const {
    displayHeader(true);  // Show the full header, including current hash
    std::cout << "Transactions       : " << transactions.size() << "\n";
    for (const auto& tx : transactions) {
        tx.display();
        std::cout << "---------------------------------------------\n";
    }
}
