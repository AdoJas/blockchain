#include <unordered_map>
#include "transactionUTXO.h"
#include "user.h"
#include "hash.h"

Transaction::Transaction(const std::string& txID, const std::string& sender, const std::string& receiver, double amount)
        : txID(txID), sender(sender), receiver(receiver), amount(amount) {}

std::string Transaction::getTxID() const { return txID; }
std::string Transaction::getSender() const { return sender; }
std::string Transaction::getReceiver() const { return receiver; }

double Transaction::getAmount() const { return amount; }

const std::vector<std::string>& Transaction::getInputs() const { return inputs; }
const std::vector<std::string>& Transaction::getOutputs() const { return outputs; }

void Transaction::addInput(const std::string& input) { inputs.push_back(input); }
void Transaction::addOutput(const std::string& output) { outputs.push_back(output); }

void displayTransaction(const Transaction& tx) {
    std::cout << "==================== Transaction ====================\n";
    std::cout << "Transaction ID    : " << tx.getTxID() << "\n";
    std::cout << "SenderPublicKey   : " << tx.getSender() << "\n";
    std::cout << "ReceiverPublicKey : " << tx.getReceiver() << "\n";
    std::cout << "Amount            : " << tx.getAmount() << "\n";
    std::cout << "Inputs            : ";
    for (const auto& input : tx.getInputs()) std::cout << input << " ";
    std::cout << "\nOutputs           : ";
    for (const auto& output : tx.getOutputs()) std::cout << output << " ";
    std::cout << "\n====================================================\n";
}

double UTXOPool::calculateBalance(const std::string& publicKey) const {
    double balance = 0.0;
    for (const auto& [utxoID, utxo] : utxos) {
        if (utxo.first == publicKey) {
            balance += utxo.second;
        }
    }
    return balance;
}
std::vector<std::pair<std::string, double>> UTXOPool::getUTXOsForOwner(const std::string& owner) const {
    std::vector<std::pair<std::string, double>> userUTXOs;
    for (const auto &[utxoID, utxo]: utxos) {
        if (utxo.first == owner) {
            userUTXOs.emplace_back(utxoID, utxo.second);
        }
    }
    return userUTXOs;
}

void UTXOPool::addInitialUTXO(const User& user, double initialBalance) {
    std::string utxoID = generateCustomHash(user.getPublicKey() + "_initial" + std::to_string(std::time(nullptr)) + std::to_string(std::rand()));
    utxos[utxoID] = {user.getPublicKey(), initialBalance};
}
void generateTransactions(int tranCount, const std::vector<User>& users, UTXOPool& utxoPool, std::vector<Transaction>& transactions) {
    for (int i = 0; i < tranCount; ++i) {
        Transaction tx = generateRandomTransaction(users, utxoPool);
        if (utxoPool.validateTransaction(tx)) {
            transactions.push_back(tx);
            utxoPool.applyTransaction(tx);
        } else {
            std::cout << "Generated an invalid transaction (ID: " << tx.getTxID() << "), skipping.\n";
        }
    }
}
void UTXOPool::displayUTXOs() const {
    std::cout << "==================== UTXOs ====================\n";
    for (const auto& [utxoID, utxoData] : utxos) {
        std::cout << "UTXO ID: " << utxoID << ", Owner: " << utxoData.first << ", Amount: " << utxoData.second << "\n";
    }
    std::cout << "==============================================\n";
}
bool UTXOPool::hasUTXO(const std::string& utxoID) const {
    return utxos.find(utxoID) != utxos.end();
}
Transaction generateRandomTransaction(const std::vector<User>& users, UTXOPool& utxoPool) {
    if (users.size() < 2) {
        std::cerr << "Not enough users to generate a valid transaction." << std::endl;
        return Transaction("", "", "", 0.0);
    }
    int senderIndex = std::rand() % users.size();
    int receiverIndex;
    do { receiverIndex = std::rand() % users.size(); } while (receiverIndex == senderIndex);

    User sender = users[senderIndex];
    User receiver = users[receiverIndex];

    auto senderUTXOs = utxoPool.getUTXOsForOwner(sender.getPublicKey());
    if (senderUTXOs.empty()) {
        std::cerr << "No UTXOs available for sender: " << sender.getPublicKey() << std::endl;
        return Transaction("", "", "", 0.0);
    }

    double totalBalance = 0.0;
    for (const auto& [utxoID, utxoValue] : senderUTXOs) {
        totalBalance += utxoValue;
    }

    double amount = totalBalance * 0.25;

    std::string txID = generateCustomHash(sender.getPublicKey() + receiver.getPublicKey() + std::to_string(amount) + std::to_string(std::time(nullptr)));
    Transaction tx(txID, sender.getPublicKey(), receiver.getPublicKey(), amount);

    double inputTotal = 0.0;
    for (const auto& [utxoID, utxoValue] : senderUTXOs) {
        tx.addInput(utxoID);
        inputTotal += utxoValue;
        if (inputTotal >= amount) break;
    }

    if (inputTotal < amount) {
        std::cerr << "Insufficient funds for sender: " << sender.getPublicKey() << std::endl;
        return Transaction("", "", "", 0.0);
    }

    tx.addOutput(receiver.getPublicKey() + ":" + std::to_string(amount));
    if (inputTotal > amount) {
        tx.addOutput(sender.getPublicKey() + ":" + std::to_string(inputTotal - amount));
    }

    return tx;
}

bool UTXOPool::validateTransaction(const Transaction& tx) const {
    double inputTotal = 0.0;
    double outputTotal = 0.0;

    for (const auto& input : tx.getInputs()) {
        auto it = utxos.find(input);
        if (it == utxos.end() || it->second.first != tx.getSender()) {
            std::cout << "Invalid transaction: Input UTXO not found or ownership mismatch.\n";
            return false;
        }
        inputTotal += it->second.second;
    }

    for (const auto& output : tx.getOutputs()) {
        size_t delimiterPos = output.find(':');
        if (delimiterPos != std::string::npos) {
            outputTotal += std::stod(output.substr(delimiterPos + 1));
        }
    }

    return inputTotal >= outputTotal;
}

void UTXOPool::applyTransaction(const Transaction& tx) {
    for (const auto& input : tx.getInputs()) {
        if (hasUTXO(input)) {
            std::cout << "Removing UTXO ID: " << input << "\n";
            utxos.erase(input);
        }
    }

    int outputIndex = 0;
    for (const auto& output : tx.getOutputs()) {
        size_t delimiterPos = output.find(':');
        if (delimiterPos != std::string::npos) {
            std::string recipient = output.substr(0, delimiterPos);
            double amount = std::stod(output.substr(delimiterPos + 1));
            std::string rawUTXOID = tx.getTxID() + "_" + std::to_string(outputIndex++);
            utxos[generateCustomHash(rawUTXOID)] = {recipient, amount};
            std::cout << "Added UTXO ID: " << rawUTXOID << ", Recipient: " << recipient << ", Amount: " << amount << "\n";
        }
    }
}


