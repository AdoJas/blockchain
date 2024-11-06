// blockchain_console.cpp
#include "blockchain_console.h"
#include <cstdlib>
#include <iomanip>

// Konstruktorius
BlockchainConsole::BlockchainConsole(Blockchain& chain) : blockchain(chain) {}

//Parodo visa blockchaina
void BlockchainConsole::displayBlockchain() const {
    const auto& chain = blockchain.getChain();
    for (size_t i = 0; i < chain.size(); ++i) {
        std::cout << "\nBlock #" << i << ":\n";
        chain[i].display();
    }
}

//Parodo visus vartotojus
void BlockchainConsole::displayAllUsers() const {
    const auto& users = blockchain.getUsers();
    if(users.size() == 0)
    {
        std::cout << "\n==================== Users ====================\n";
        std::cout << "No users in the blockchain.\n";
        std::cout << "===============================================\n";
        return;
    }else{
        std::cout << "\n==================== All of the users ====================\n";
        for (size_t i = 0; i < users.size(); ++i){
            std::cout << "\n================= User Details =================\n";
            std::cout << "Index         : " << i << "\n";
            std::cout << "Public Key    : " << users[i] << "\n";
            std::cout << "Balance       : " << blockchain.getUserBalance(users[i]) << " units\n";
            std::cout << "UTXOs         : " << "\n";

            const auto& utxoList = blockchain.getUserUTXOs(users[i]);
            if (utxoList.empty()) {
                std::cout << "  No UTXOs available.\n";
            } else {
                for (const auto& utxo : utxoList) {
                    std::cout << "  - Amount: " << utxo.getAmount() << " units, Transaction ID: " << utxo.getUTXOId() << "\n";
                }
            }
            std::cout << "=================================================\n";
        }
        std::cout << "=========================================================\n";
    }
}

// Parodo vartotoja pagal indeksa
void BlockchainConsole::displayUserByIndex(int index) const {
    const auto& users = blockchain.getUsers();

    if (index < 0 || index >= users.size()) {
        std::cout << "Invalid user index.\n";
        return;
    }

    std::string userKey = users[index];

    std::cout << "\n================= User Details =================\n";
    std::cout << "Index         : " << index + 1 << "\n";
    std::cout << "Public Key    : " << userKey << "\n";
    std::cout << "Balance       : " << blockchain.getUserBalance(userKey) << " units\n";
    std::cout << "UTXOs         : " << "\n";

    const auto& utxoList = blockchain.getUserUTXOs(userKey);
    if (utxoList.empty()) {
        std::cout << "  No UTXOs available.\n";
    } else {
        for (const auto& utxo : utxoList) {
            std::cout << "  - Amount: " << utxo.getAmount() << " units, Transaction ID: " << utxo.getUTXOId() << "\n";
        }
    }

    std::cout << "=================================================\n";
}

//Parodo sugeneruotas transakcijas, kurios dar nepridetos i bloka
void BlockchainConsole::displayPendingTransactions() const {
    const auto& transactions = blockchain.getPendingTransactions();
    std::cout << "\n--- Pending Transactions ---\n";
    if (transactions.empty()) {
        std::cout << "No pending transactions.\n";
        std::cout << "-----------------\n";
        return;
    }
    for (const auto& tx : transactions) {
        tx.display();
        std::cout << "-----------------\n";
    }
}

//Parodo bloka pagal indeksa
void BlockchainConsole::displayBlockByIndex(int index) const {
    const auto& chain = blockchain.getChain();
    if (index < 0 || index >= chain.size()) {
        std::cout << "Invalid block index.\n";
        return;
    }
    std::cout << "\n--- Block #" << index << " ---\n";
    chain[index].display();
}

// Parodo transakcija is bloko pagal indeksa ( reikia bloko ir transakcijos indekso)
void BlockchainConsole::displayTransactionFromBlock(int blockIndex, int txIndex) const {
    const auto& chain = blockchain.getChain();
    if (blockIndex < 0 || blockIndex >= chain.size()) {
        std::cout << "Invalid block index.\n";
        return;
    }

    const auto& transactions = chain[blockIndex].getTransactions();
    std::cout << transactions.size() << "\n";
    if (txIndex < 0 || txIndex >= transactions.size()) {
        std::cout << "Invalid transaction index.\n";
        return;
    }

    std::cout << "\n--- Transaction #" << txIndex + 1 << " in Block #" << blockIndex + 1 << " ---\n";
    transactions[txIndex].display();
}

//Parodo paskutinio bloko informacija (header info, transakcijas)
void BlockchainConsole::displayLatestBlock() const {
    std::cout << "\n--- Latest Block ---\n";
    blockchain.getLatestBlock().display();
}
// Funkcija kuri kuria vartotojus
void BlockchainConsole::generateUsers(int numUsers) const {
    blockchain.generateUsers(numUsers);
}
// Funkcija kuri kasa laukiancias transakcijas (Ne kandidatu bloku metodas)
void BlockchainConsole::minePendingTransactions() {
    blockchain.addBlock();
}
// Funkcija kuri kasa laukiancias transakcijas su kandidatais blokais
void BlockchainConsole::mineWithCandidateBlocks(){
    blockchain.mineWithCandidateBlocks();
}
// Funkcija kuri generuoja atsitiktines transakcijas
void BlockchainConsole::generateRandomTransactions(int numTransactions) {
    const auto& users = blockchain.getUsers();
    if (users.size() < 2) {
        std::cout << "At least two users are needed to create transactions.\n";
        return;
    }

    for (int i = 0; i < numTransactions; ++i) {
        // Parenkam siunteja ir gaveja, taip, kad jie nebutu vienodi
        std::string sender, receiver;
        do {
            sender = users[rand() % users.size()];
            receiver = users[rand() % users.size()];
        } while (sender == receiver);

        // Gaunam siuntejo balansa
        unsigned int senderBalance = blockchain.getUserBalance(sender);
        if (senderBalance == 0) {
            continue;  // Skip if sender has no balance
        }

        unsigned int maxAmount = static_cast<unsigned int>(1.25 * senderBalance);  // 125% siuntejo balanso
        unsigned int amount = (rand() % maxAmount) + 1; //Suma nuo 1 iki 125% siuntejo balanso



        // Sukuriam transakcija ir ja pridedam i laukianciu transakciju sarasa
        Transaction tx(sender, receiver, amount); //Time stampas ir transaction id sugeneruojami konstruktoriuje
        blockchain.createTransaction(tx);

        std::cout << "\n================= Generated Transaction =================\n";
        std::cout << std::left << std::setw(15) << "Sender:"      << sender << "\n";
        std::cout << std::left << std::setw(15) << "Receiver:"    << receiver << "\n";
        std::cout << std::left << std::setw(15) << "Amount:"      << amount << " units\n";
        std::cout << "=========================================================\n";
    }
}
//Funkcija kuri paleidzia user interface (konsole)
void BlockchainConsole::run() {
    while (true) {
        std::cout << "\n--- Blockchain Console ---\n";
        std::cout << "1. Display Blockchain\n";
        std::cout << "2. Display All Users\n";
        std::cout << "3. Display User by Index\n";
        std::cout << "4. Display Pending Transactions\n";
        std::cout << "5. Mine Pending Transactions\n";
        std::cout << "6. Display Block by Index\n";
        std::cout << "7. Display Transaction from Block\n";
        std::cout << "8. Display Latest Block\n";
        std::cout << "9. Generate Users\n";
        std::cout << "10. Generate Random Transactions\n";
        std::cout << "11. Mine with candidate blocks\n";
        std::cout << "12. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            displayBlockchain();
        } else if (choice == 2) {
            displayAllUsers();
        } else if (choice == 3) {
            int index;
            std::cout << "Enter user index: ";
            std::cin >> index;
            displayUserByIndex(index - 1);
        } else if (choice == 4) {
            displayPendingTransactions();
        } else if (choice == 5) {
            minePendingTransactions();
        } else if (choice == 6) {
            int index;
            std::cout << "Enter block index: ";
            std::cin >> index;
            displayBlockByIndex(index - 1);
        } else if (choice == 7) {
            int blockIndex, txIndex;
            std::cout << "Enter block index: ";
            std::cin >> blockIndex;
            std::cout << "Enter transaction index: ";
            std::cin >> txIndex;
            displayTransactionFromBlock(blockIndex - 1, txIndex - 1);
        } else if (choice == 8) {
            displayLatestBlock();
        } else if (choice == 9) {
            int numUsers;
            std::cout << "Enter number of users to generate: ";
            std::cin >> numUsers;
            generateUsers(numUsers);
        } else if (choice == 10) {
            int numTransactions;
            std::cout << "Enter number of random transactions to generate: ";
            std::cin >> numTransactions;
            generateRandomTransactions(numTransactions);
        } else if(choice == 11){
            mineWithCandidateBlocks();
        } else if (choice == 12) {
            std::cout << "Exiting Blockchain Console.\n";
            break;
        } else {
            std::cout << "Invalid choice, please try again.\n";
        }
    }
}
