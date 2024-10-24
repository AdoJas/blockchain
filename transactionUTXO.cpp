//
// Created by adoma on 10/24/2024.
//

#include <unordered_map>
#include "transactionUTXO.h"
#include "user.h"

void displayTransaction(const Transaction& tx) {
    std::cout << "Transaction ID: " << tx.txID << std::endl;
    std::cout << "Sender: " << tx.sender << std::endl;
    std::cout << "Receiver: " << tx.receiver << std::endl;
    std::cout << "Amount: " << tx.amount << std::endl;
    std::cout << "Inputs: ";
    for (const auto& input : tx.inputs) {
        std::cout << input << " ";
    }
    std::cout << "\nOutputs: ";
    for (const auto& output : tx.outputs) {
        std::cout << output << " ";
    }
    std::cout << "\n-----------------------" << std::endl;
}

void transactionGeneration(int tranCount, std::vector<User> users, std::vector<Transaction> transactions){
    const int numTransactions = 100;
    for (int i = 0; i < numTransactions; i++) {
        transactions.push_back(generateRandomTransaction(users));
    }
}

bool UTXOPool::validateTransaction(const Transaction& tx) {
    double totalInput = 0, totalOutput = 0;
    for (const auto& input : tx.inputs) {
        if (utxos.find(input) == utxos.end()) return false; // UTXO neegzistuoja
        totalInput += utxos[input].second;
    }
    for (const auto& output : tx.outputs) {
        totalOutput += /* value from output */ 0; // Placeholder, calculate from actual transaction
    }
    return totalInput >= totalOutput; // Inputu turi but daugiau nei outputu
}

void UTXOPool::applyTransaction(const Transaction& tx) {
    for (const auto& input : tx.inputs) {
        utxos.erase(input); // Pazymim input UTXOs kaip isleistus
    }
    for (const auto& output : tx.outputs) {
        // Add new UTXOs
        utxos[output] = {tx.receiver, /* value from output */ 0}; // Placeholder, update with actual values
    }
}

Transaction generateRandomTransaction(const std::vector<User>& users) {
    Transaction tx;
    tx.txID = "TX_" + std::to_string(rand() % 10000);
    const auto& sender = users[rand() % users.size()];
    const auto& receiver = users[rand() % users.size()];
    tx.sender = sender.publicKey;
    tx.receiver = receiver.publicKey;
    tx.amount = static_cast<double>(rand() % 100) + 0.01; // Random amount
    tx.inputs.push_back("UTXO_" + std::to_string(rand() % 10000)); // Add random UTXO
    tx.outputs.push_back("UTXO_" + std::to_string(rand() % 10000)); // Add new UTXO
    return tx;
}