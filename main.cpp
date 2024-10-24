#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include "user.h"
#include "transactionUTXO.h"
#include "block.h"
#include "blockchain.h"

//TODO: sutvarkyti funkcijas strukturas ir klases i jiems skirtus failus


int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random skaiciu generatoriui

    // Step 1: Generuojam atsitiktinius userius


    // Step 2: generuojam atsitiktines transakcijas

    // Step 3: Sukuriam blockchain ir candidate blokus
    Blockchain blockchain;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    std::vector<Block> candidateBlocks(5); // Create 5 candidate blocks
    for (int i = 0; i < 5; ++i) {
        candidateBlocks[i] = blockchain.createBlock(transactions, blockchain.getLastBlockHash());
        candidateBlocks[i].difficultyTarget = 1; // mazas sudetingumas testavimui
    }

    return 0;
}
