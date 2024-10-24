//
// Created by adoma on 10/20/2024.
//

#ifndef BLOCKCHAIN_BLOCK_H
#define BLOCKCHAIN_BLOCK_H
#include "transactionUTXO.h"
#include "hash.h"

struct Block {
    std::string prevBlockHash;
    std::string merkleRoot;
    time_t timestamp;
    int nonce;
    int difficultyTarget;
    std::vector<Transaction> transactions;

    std::string calculateHash() const;
    // Metodas isvedantis sio bloko transakcijas i konsole
    void displayTransactions() const;
};


#endif //BLOCKCHAIN_BLOCK_H
