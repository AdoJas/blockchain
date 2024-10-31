#include "blockchain.h"
#include <ctime>
#include <iostream>
#include <chrono>


std::mutex mtx;

void Blockchain::getBlock(int index){
    if(index < chain.size()){
        std::cout << "=================== Block " << index << " ===================\n" << std::endl;
        std::cout << "Block Hash: " << chain[index].getHash() << "\n";
        std::cout << "Previous Hash: " << chain[index].getPreviousHash() << "\n";
        std::cout << "Merkle Root: " << chain[index].getMerkleRoot() << "\n";
        std::cout << "Transactions:\n";
        chain[index].displayBlockTransactions();
        //std::cout << "===================================================\n" << std::endl;
    }
    else std::cout << "Block with index " << index << " does not exist!" << std::endl;
}

unsigned long long Blockchain::getTransactionCount(int index) const {
    if(index < chain.size()){
        return chain[index].transactionCount();
    }
    else return 0;
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
    std::random_shuffle(tempPool.begin(), tempPool.end());
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
    while (!transactions.empty()) {
        std::vector<Transaction> selectedTransactions;
        selectRandomTransactions(transactions, selectedTransactions);

        Block block = createBlock(selectedTransactions, getLastBlockHash());
        mineBlock(block);
        addBlock(block);

        //block.displayBlockTransactions();

        transactions.erase(transactions.begin(), transactions.begin() + selectedTransactions.size());
        std::cout << transactions.size() << " transactions remaining in the pool." << std::endl;
    }
    std::cout << "All transactions have been processed and added to the blockchain." << std::endl;
}