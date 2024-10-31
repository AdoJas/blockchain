#include <vector>
#include <string>
#include <ctime>
#include "user.h"
#include "transactionUTXO.h"
#include "block.h"
#include "blockchain.h"


//TODO: implementuoti gerasias oop praktikas, RAII ir tt. pakeisti User struktura i klase, sutvarkyti readME.md faila, kintamuju vardus
void displayAllTransactions(const std::vector<Transaction>& transactions) {
    std::cout << "==================== Transactions ====================\n";
    for (const auto& tx : transactions) {
        displayTransaction(tx);
    }
    std::cout << "======================================================\n";
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    std::vector<User> users;
    UTXOPool utxoPool;
    std::vector<Transaction> transactions;
    Blockchain blockchain(3);
    while (true) {
        std::cout << "List of commands: \n"
                  << "1. user-generate\n"
                  << "2. show-user\n"
                  << "3. transaction-generate\n"
                  << "4. show-transaction-in-pool\n"
                  << "5. mine-block\n"
                  << "6. show-specific-block\n"
                  << "7. show-transaction-specific-block\n"
                  << "8. display-current-block\n"
                  << "9. display-blockchain\n"
                  << "10. exit\n";

        std::cout << "Enter a command (number 1-10): ";
        std::string command;
        std::getline(std::cin, command);

        while (stoi(command) < 1 || stoi(command) > 10) {
            std::cout << "------------------------------------------------\n";
            std::cout << " Invalid command. Please enter a valid command: ";

            std::getline(std::cin, command);
            std::cout << "------------------------------------------------\n";
        }

        if (command == "10") {
            break;
        }

        switch (std::stoi(command)) {
            case 1: {
                std::cout << "Enter the number of users to generate: ";
                int userNumber;
                std::cin >> userNumber;
                std::cin.ignore();
                randomUserGeneration(userNumber, users);
                UTXOPool::initializePool(users, utxoPool);
                std::cout << userNumber << " users have been generated.\n";
                std::cout << "Total number of users: " << users.size() << std::endl;
                break;
            }
            case 2:{
                int userNumber;
                if(users.empty()){
                    std::cout << "Error: No users available to display.\n";
                    break;
                }
                std::cout << "Enter the number of the user to display (if you want to display all the current users, type 0): ";
                std::cout << "Total number of users: " << users.size() << std::endl;
                std::cin >> userNumber;
                std::cin.ignore();
                if(userNumber == 0){
                    for(const auto & user : users){
                        user.display();
                    }
                    break;
                }
                users[userNumber-1].display();
                break;
            }
            case 3: {
                if(users.empty()){
                    std::cout << "Error: No users available to generate transactions.\n";
                    break;
                }
                std::cout << "Enter the number of transactions to generate: ";
                int tranCount;
                std::cin >> tranCount;
                std::cin.ignore();

                auto start = std::chrono::high_resolution_clock::now();
                generateTransactions(tranCount, users, utxoPool, transactions);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                std::cout << tranCount << " transactions have been generated.\n";
                std::cout << "Time taken to generate " << tranCount << " transactions: " << duration.count() << " sec\n";
                std::cout << "Total number of transactions: " << transactions.size() << std::endl;
                break;
            }

            case 4: {  // Display transactions in pool
                if (transactions.empty()) {
                    std::cout << "Error: No  transactions available to display.\n";
                    break;
                }
                displayAllTransactions(transactions);
                break;
            }

            case 5: {
                if(transactions.empty()){
                    std::cout << "Error: No transactions available to mine.\n";
                    break;
                }
                std::cout << "===================== Block =======================\n";
                std::cout << "Starting the mining process...\n";
                blockchain.processTransactions(transactions);
                std::cout << "Block mined and added to the blockchain.\n";
                std::cout << "===================================================\n";
                //blockchain.displayBlockchain();
                break;
            }
            case 6: {
                if(blockchain.isEmpty()){
                    std::cout << "Error: No block available to display.\n";
                    break;
                }
                std::cout << "Enter the number of the block to display: ";
                int blockNumber;
                std::cin >> blockNumber;
                std::cin.ignore();
                if(blockNumber < 0 || blockNumber > blockchain.size()){
                    std::cout << "Error: Invalid block number.\n";
                    break;
                }
                blockchain.getBlock(blockNumber-1);
                break;
            }
            case 7: {
                int transactionNumber;
                if(blockchain.isEmpty()){
                    std::cout << "Error: No block available to display.\n";
                    break;
                }
                std::cout << "Enter the number of the block to display: ";
                int blockNumber;
                std::cin >> blockNumber;
                std::cin.ignore();
                if(blockNumber < 0 || blockNumber > blockchain.size()){
                    std::cout << "Error: Invalid block number.\n";
                    break;
                }
                std::cout << "There are a total of " << blockchain.getTransactionCount(blockNumber - 1) << " transactions in the block.\n";
                std::cout << "Enter the number of the transaction to display in the block: ";
                std::cin >> transactionNumber;
                std::cin.ignore();
                if(transactionNumber < 0 || transactionNumber > blockchain.getTransactionCount(blockNumber - 1)){
                    std::cout << "Error: Invalid transaction number.\n";
                    break;
                }else{
                    blockchain.displaySpecificTransaction(blockNumber-1, transactionNumber-1);
                }
                break;
            }
            case 8: {
                if(blockchain.isEmpty()){
                    std::cout << "Error: No block available to display.\n";
                    break;
                }
                if (!blockchain.getLastBlock().transactions.empty()) {
                    std::cout << "Displaying transactions in the most recent block:\n";
                    blockchain.getBlock(blockchain.size()-1);
                } else {
                    std::cout << "No block transactions to display.\n";
                }
                break;
            }
            case 9: {  // Display entire blockchain
                if(blockchain.isEmpty()){
                    std::cout << "Error: No blockchain available to display.\n";
                    break;
                }
                std::cout << "Displaying blockchain:\n";
                blockchain.displayBlockchain();
                break;
            }
        }
    }

    return 0;
}
