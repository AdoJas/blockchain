#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "transaction.h"

class Block {
private:
    int version = 1;                     // Bloko versija
    std::string prevBlockHash;       // Buvusio bloko hashas
    std::string merkleRootHash;      // Merkle root hashas
    unsigned int nonce;              // Nonce, su kuriuo bloko hashas buvo iskastas
    std::string timestamp;           // Bloko kurimo laikas
    int difficulty;                  // Bloko kasybos sunkumas
    std::vector<Transaction> transactions;  // Transactions within this block
    std::string blockHash;           // Bloko hashas

public:
    Block(const std::string& prevHash, int diff, int ver = 1);

    std::string calculateHash() const;    // Calculates the block's hash
    bool mineBlock();                     // Mines the block by finding a valid nonce
    std::string getHash() const;          // Returns the block's calculated hash
    const std::string& getPrevBlockHash() const;
    const std::string& getTimestamp() const;
    unsigned int getNonce() const;

    void addTransaction(const Transaction& tx);  // Adds a transaction to the block
    void calculateMerkleRoot();                  // Calculates the Merkle root
    void displayHeader(bool showCurrentHashAfterMining = false) const;  // Display header info
    void display() const;                        // Display full block info
    const std::vector<Transaction>& getTransactions() const;
};

#endif // BLOCK_H
