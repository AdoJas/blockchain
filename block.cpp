//
// Created by adoma on 10/20/2024.
//

#include "block.h"

void Block::displayTransactions() const {
    std::cout << "Block Transactions:" << std::endl;
    for (const auto& tx : transactions) {
        displayTransaction(tx);
    }
    std::cout << "======================" << std::endl;
}
std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << prevBlockHash << merkleRoot << timestamp << nonce << difficultyTarget;
    return generateCustomHash(ss.str());
}