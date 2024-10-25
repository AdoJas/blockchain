#include <unordered_map>
#include "transactionUTXO.h"
#include "user.h"
#include "hash.h"

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

void transactionGeneration(int tranCount, const std::vector<User>& users, std::vector<Transaction>& transactions) {
    for (int i = 0; i < tranCount; ++i) {
        Transaction tx = generateRandomTransaction(users);
        transactions.push_back(tx);
    }
}

Transaction generateRandomTransaction(const std::vector<User>& users) {
    //Parenkam random siunteja ir gaveja
    int senderIndex = std::rand() % users.size();
    int receiverIndex;
    do {
        receiverIndex = std::rand() % users.size();
    } while (receiverIndex == senderIndex); // Patikrinam, ar siuntejas ir gavejas skirtingi

    double amount = (std::rand() % 99999) / 100.0 + 1.0;  // Random kiekis nuo 100 iki milijono

    Transaction tx;
    tx.sender = users[senderIndex].publicKey;
    tx.receiver = users[receiverIndex].publicKey;
    tx.amount = amount;

    // Unikalus transakciju id
    std::string txData = tx.sender + tx.receiver + std::to_string(tx.amount);
    tx.txID = generateCustomHash(txData);

    return tx;
}

bool UTXOPool::validateTransaction(const Transaction& tx) {
    double inputTotal = 0.0;
    double outputTotal = 0.0;

    // 1. tikrina ar utxo yra pool'e
    for (const auto& input : tx.inputs) {
        auto it = utxos.find(input);
        if (it == utxos.end()) {
            std::cout << "Invalid transaction: Input UTXO not found in pool." << std::endl;
            return false;
        }

        // 2. Nuosavybes patvirtinimas, ar outputas priklauso senderiui
        if (it->second.first != tx.sender) {
            std::cout << "Invalid transaction: UTXO ownership does not match sender." << std::endl;
            return false;
        }

        inputTotal += it->second.second;
    }
    // 3. Visas output kiekis
    for (const auto& output : tx.outputs) {
        size_t delimiterPos = output.find(':');
        if (delimiterPos != std::string::npos) {
            double outputAmount = std::stod(output.substr(delimiterPos + 1));
            outputTotal += outputAmount;
        }
    }

    // 4. inputas didesnis nei outputas
    if (inputTotal < outputTotal) {
        //std::cout << "Invalid transaction: Insufficient input balance." << std::endl;
        return false;
    }

    return true;
}

void UTXOPool::applyTransaction(const Transaction& tx) {
    for (const auto& input : tx.inputs) {
        utxos.erase(input);
    }

    // Pridedam naujus UTXO
    for (const auto& output : tx.outputs) {
        size_t delimiterPos = output.find(':');
        if (delimiterPos != std::string::npos) {
            std::string recipient = output.substr(0, delimiterPos);
            double amount = std::stod(output.substr(delimiterPos + 1));

            // Nauji unukalus UTXO id hashinant txID ir recipient.
            std::string rawUTXOID = tx.txID + "_" + recipient;
            std::string newUTXOID = generateCustomHash(rawUTXOID);

            utxos[newUTXOID] = {recipient, amount};
        }
    }
}
