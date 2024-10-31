#include "blockchain.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <random>


std::mutex mtx;

Blockchain::Blockchain(int difficulty) : difficulty(difficulty) {
    Block genesisBlock = createBlock({}, "0");
    mineBlock(genesisBlock);
    addBlock(genesisBlock);
    std::cout << "Genesis block created with hash: " << genesisBlock.getHash() << std::endl;
}

void Blockchain::getBlock(int index){
    if(index < chain.size()){
        std::cout << "=================== Block " << index << " ===================\n" << std::endl;
        std::cout << "Block Hash: " << chain[index].getHash() << "\n";
        std::cout << "Previous Hash: " << chain[index].getPreviousHash() << "\n";
        std::cout << "Merkle Root: " << chain[index].getMerkleRoot() << "\n";
        std::cout << "Transactions:\n";
        chain[index].displayBlockTransactions();
    }
    else std::cout << "Block with index " << index << " does not exist!" << std::endl;
}

unsigned long long Blockchain::getTransactionCount(int index) const {
    return index < chain.size() ? chain[index].transactionCount() : 0;
}

Block& Blockchain::getLastBlock() {
    return chain.back();
}
bool Blockchain::isEmpty() const {
    if(chain.empty()) return true;
    else return false;
}
void Blockchain::displayBlockchain() const {
    std::cout << "==================== Blockchain ====================\n";
    for (const auto& block : chain) {

        block.displayBlockTransactions();
        std::cout << "Block Hash: " << block.getHash() << "\n";
        std::cout << "Previous Hash: " << block.getPreviousHash() << "\n";
        std::cout << "Merkle Root: " << block.getMerkleRoot() << "\n";
        std::cout << "Transactions: above ^\n";
        std::cout << "---------------------------------------------------\n";
    }
    std::cout << "===================================================\n";
}

void Blockchain::displaySpecificTransaction(int blockIndex, int transactionIndex){
    if(blockIndex < chain.size()){
        if(transactionIndex < chain[blockIndex].transactionCount()){
            std::cout << "=================== Transaction " << transactionIndex + 1 << " ===================\n" << std::endl;
            displayTransaction(chain[blockIndex].transactions[transactionIndex]);
            std::cout << "===================================================\n" << std::endl;
        }
        else std::cout << "Transaction with index " << transactionIndex << " does not exist!" << std::endl;
    }
    else std::cout << "Block with index " << blockIndex << " does not exist!" << std::endl;

}
std::string Block::getPreviousHash() const {
    return previousHash;
}

void selectRandomTransactions(const std::vector<Transaction>& transactionPool, std::vector<Transaction>& selectedTransactions) {
    selectedTransactions.clear();
    std::vector<Transaction> tempPool = transactionPool;
    std::shuffle(tempPool.begin(), tempPool.end(), std::mt19937(std::random_device()()));
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
    std::vector<Transaction> selectedTransactions;
    selectRandomTransactions(transactions, selectedTransactions);

    std::vector<Transaction> validTransactions;

    for (auto& tx : selectedTransactions) {
        if (utxoPool.validateTransaction(tx)) {
            validTransactions.push_back(tx); // Add valid transactions to pool for future block creation
            std::cout << "Transaction " << tx.getTxID() << " validated successfully.\n";
        } else {
            std::cout << "Invalid transaction with ID: " << tx.getTxID() << " was ignored.\n";
        }
    }

    if (!validTransactions.empty()) {
        Block block = createBlock(validTransactions, getLastBlockHash());
        mineBlock(block);
        addBlock(block);

        for (const auto& tx : validTransactions) {
            utxoPool.applyTransaction(tx);
        }
        std::cout << "Block with " << validTransactions.size() << " transactions mined and added to blockchain.\n";
    } else {
        std::cout << "No valid transactions for this block, skipping block creation.\n";
    }
}