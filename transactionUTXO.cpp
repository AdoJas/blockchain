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

void generateTransactions(int tranCount, const std::vector<User>& users, UTXOPool& utxoPool, std::vector<Transaction>& transactions) {
    for (int i = 0; i < tranCount; ++i) {
        Transaction tx = generateRandomTransaction(users, utxoPool);
        if (!tx.getInputs().empty() && !tx.getOutputs().empty()) {
            transactions.push_back(tx);
        }
    }
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

Transaction generateRandomTransaction(const std::vector<User>& users, UTXOPool& utxoPool) {
    int senderIndex = std::rand() % users.size();
    int receiverIndex;
    do { receiverIndex = std::rand() % users.size(); } while (receiverIndex == senderIndex);

    double amount = (std::rand() % 99999) / 100.0 + 1.0;
    std::string txID = generateCustomHash(users[senderIndex].getPublicKey() + users[receiverIndex].getPublicKey() + std::to_string(amount));
    Transaction tx(txID, users[senderIndex].getPublicKey(), users[receiverIndex].getPublicKey(), amount);

    double inputTotal = 0.0;
    auto userUTXOs = utxoPool.getUTXOsForOwner(tx.getSender());

    for (const auto& [utxoID, utxoValue] : userUTXOs) {
        if (inputTotal >= amount) break;
        tx.addInput(utxoID);
        inputTotal += utxoValue;
    }

    if (inputTotal >= amount) {
        tx.addOutput(tx.getReceiver() + ":" + std::to_string(amount));
        if (inputTotal > amount) {
            tx.addOutput(tx.getSender() + ":" + std::to_string(inputTotal - amount));
        }
    }

    return tx;
}
void UTXOPool::initializePool(const std::vector<User>& users, UTXOPool& utxoPool) {
    for (const auto& user : users) {
        double userBalance = user.getBalance();
        std::string utxoID = generateCustomHash(user.getPublicKey() + "_initial");
        utxoPool.utxos[utxoID] = {user.getPublicKey(), userBalance};
    }
}

bool UTXOPool::validateTransaction(const Transaction& tx) const {
    if (!hasSufficientBalance(tx.getSender(), tx.getAmount())) {
        std::cout << "Insufficient balance for transaction.\n";
        return false;
    }
    if (!verifyTransactionHash(tx)) {
        std::cout << "Transaction hash verification failed.\n";
        return false;
    }
    double inputTotal = 0.0;
    double outputTotal = 0.0;

    for (const auto& input : tx.getInputs()) {
        auto it = utxos.find(input);
        if (it == utxos.end() || it->second.first != tx.getSender()) return false;
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
        utxos.erase(input);
    }

    int outputIndex = 0;
    for (const auto& output : tx.getOutputs()) {
        size_t delimiterPos = output.find(':');
        if (delimiterPos != std::string::npos) {
            std::string recipient = output.substr(0, delimiterPos);
            double amount = std::stod(output.substr(delimiterPos + 1));
            std::string rawUTXOID = tx.getTxID() + "_" + std::to_string(outputIndex++);
            utxos[generateCustomHash(rawUTXOID)] = {recipient, amount};
        }
    }
}
bool UTXOPool::hasSufficientBalance(const std::string& owner, double amount) const {
    double totalBalance = 0.0;
    for (const auto& [utxoID, utxo] : utxos) {
        if (utxo.first == owner) {
            totalBalance += utxo.second;
            if (totalBalance >= amount) return true;
        }
    }
    return false;
}
bool UTXOPool::verifyTransactionHash(const Transaction& tx) {
    std::string txData = tx.getSender() + tx.getReceiver() + std::to_string(tx.getAmount());
    return tx.getTxID() == generateCustomHash(txData);
}
