#include <vector>
#include <string>
#include <ctime>
#include "user.h"
#include "transactionUTXO.h"
#include "block.h"
#include "blockchain.h"

int main() {
    srand(static_cast<unsigned>(time(0)));

    std::vector<User> users;
    randomUserGeneration(1000, users);

    std::vector<Transaction> transactions;
    transactionGeneration(10000, users, transactions);

    Blockchain blockchain;
    Block block = blockchain.createBlock(transactions, blockchain.getLastBlockHash());

    blockchain.mineBlock(block);
    blockchain.addBlock(block);

    block.displayBlockTransactions();

    return 0;
}
