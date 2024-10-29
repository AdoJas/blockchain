#include <unordered_map>
#include "transactionUTXO.h"
#include "user.h"
#include "hash.h"

void displayTransaction(const Transaction& tx) {
    std::cout << "==================== Transaction ====================" << std::endl;
    std::cout << "Transaction ID : " << tx.txID << std::endl;
    std::cout << "Sender         : " << tx.sender << std::endl;
    std::cout << "Receiver       : " << tx.receiver << std::endl;
    std::cout << "Amount         : " << tx.amount << std::endl;

    std::cout << "Inputs         : ";
    for (const auto& input : tx.inputs) std::cout << input << " ";
    std::cout << "\nOutputs        : ";
    for (const auto& output : tx.outputs) std::cout << output << " ";
    std::cout << "\n=====================================================" << std::endl;
}

void transactionGeneration(int tranCount, const std::vector<User>& users, UTXOPool& utxoPool, std::vector<Transaction>& transactions) {
    if (users.size() < 2) {
        std::cerr << "Error: Not enough users to generate transactions." << std::endl;
        return;
    }

    for (int i = 0; i < tranCount; ++i) {
        Transaction tx;
        int attempts = 0;
        do {
            tx = generateRandomTransaction(users, utxoPool);
            attempts++;
        } while ((tx.inputs.empty() || tx.outputs.empty()) && attempts < 5);

        if (!tx.inputs.empty() && !tx.outputs.empty()) {
            transactions.push_back(tx);
        }
    }
}

Transaction generateRandomTransaction(const std::vector<User>& users, UTXOPool& utxoPool) {
    int senderIndex = std::rand() % users.size();
    int receiverIndex;
    do {
        receiverIndex = std::rand() % users.size();
    } while (receiverIndex == senderIndex);

    double amount = (std::rand() % 99999) / 100.0 + 1.0;

    Transaction tx;
    tx.sender = users[senderIndex].publicKey;
    tx.receiver = users[receiverIndex].publicKey;
    tx.amount = amount;

    std::string txData = tx.sender + tx.receiver + std::to_string(tx.amount);
    tx.txID = generateCustomHash(txData);

    double inputTotal = 0.0;
    for (const auto& [utxoID, utxo] : utxoPool.utxos) {
        if (utxo.first == tx.sender && inputTotal < amount) {
            tx.inputs.push_back(utxoID);
            inputTotal += utxo.second;
        }
    }

    if (inputTotal < amount) {
        tx.inputs.clear();
        tx.outputs.clear();
    } else {
        tx.outputs.push_back(tx.receiver + ":" + std::to_string(amount));
        if (inputTotal > amount) {
            double change = inputTotal - amount;
            tx.outputs.push_back(tx.sender + ":" + std::to_string(change));
        }
    }

    return tx;
}

void initializeUTXOPool(const std::vector<User>& users, UTXOPool& utxoPool) {
    for (const auto& user : users) {
        // Naudojame pradini user balansa
        double userBalance = user.balance;

        // Sukuriam UTXO ID hashinant user public key ir "initial" zodi
        std::string utxoID = generateCustomHash(user.publicKey + "_initial");

        // Pridedam sukurta UTXO i pool'a
        utxoPool.utxos[utxoID] = {user.publicKey, userBalance};
    }
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
        std::cout << "Invalid transaction: Insufficient input balance." << std::endl;
        return false;
    }
    //displayTransaction(tx);
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
