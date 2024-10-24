//
// Created by adoma on 10/24/2024.
//

#include "blockchain.h"
std::mutex mtx;

void Blockchain::addBlock(Block block) {
    chain.push_back(block);
    for (const auto& tx : block.transactions) {
        utxoPool.applyTransaction(tx);
    }
}
Block Blockchain::createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash) {
    Block newBlock;
    newBlock.prevBlockHash = prevHash;
    newBlock.timestamp = std::time(nullptr);
    newBlock.transactions = transactions;
    newBlock.merkleRoot = "merkle_placeholder";  //TODO: implementuoti markle tree
    newBlock.nonce = 0;  // Pradine nonce reiksme
    return newBlock;
}

std::string Blockchain::getLastBlockHash() const {
    if (!chain.empty()) {
        return chain.back().calculateHash();
    }
    return "genesis";  // Grazinam genesis hasha jei nera daugiau bloku
}

void Blockchain::mineBlock(Block& block) {
    int iterationCount = 0;
    std::string targetString(block.difficultyTarget, '0'); //Kuriam target stringa, priklausomai nuo sudetingumo

    while (true) {
        std::string hash = block.calculateHash(); // Calculate current hash
        if (hash.substr(0, block.difficultyTarget) == targetString) {
            std::lock_guard<std::mutex> guard(mtx);
            std::cout << "Block mined: " << hash << " with nonce: " << block.nonce << std::endl;
            break; // Jei jokie hashai nerasti, iseinam is loopo
        }
        block.nonce++;
        iterationCount++;

        // Printinam log kas 100000 iteraciju
        if (iterationCount % 100000 == 0) {
            std::cout << "Mining iteration: " << iterationCount << " Nonce: " << block.nonce << std::endl;
        }
    }
}

void Blockchain::parallelMineBlocks(std::vector<Block>& candidateBlocks) {
    std::vector<std::thread> miners;
    for (auto& block : candidateBlocks) {
        miners.emplace_back([this, &block]() {
            mineBlock(block);
        });
    }
    for (auto& miner : miners) {
        miner.join();
    }
}