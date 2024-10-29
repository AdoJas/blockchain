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
    randomUserGeneration(2, users);

    UTXOPool utxoPool;
    initializeUTXOPool(users, utxoPool);

    std::vector<Transaction> transactions;
    transactionGeneration(1, users, utxoPool, transactions);

    int difficultyLevel = 2;
    Blockchain blockchain(difficultyLevel);

    std::cout << "Starting the mining process..." << std::endl;
    blockchain.processTransactions(transactions);

    return 0;
}
