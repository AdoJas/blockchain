// transaction.cpp
#include "transaction.h"
#include "hash.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

Transaction::Transaction(const std::string& s, const std::string& r, unsigned int amt) // Transakcijos konstruktorius
        : senderPublicKey(s), receiverPublicKey(r), amount(amt), timestamp(getCurrentTimestamp()) {
    transactionID = generateTransactionID();
}

std::string Transaction::getSender() const { return senderPublicKey; } // Grazina siuntejo public key
std::string Transaction::getReceiver() const { return receiverPublicKey; } // Grazina gavėjo public key
unsigned int Transaction::getAmount() const { return amount; } // Grazina transakcijos suma
std::string Transaction::getTransactionID() const { return transactionID; } // Grazina transakcijos ID
std::string Transaction::getTimestamp() const { return timestamp; } // Grazina transakcijos laika

// Generuoja transakcijos ID
std::string Transaction::generateTransactionID() const {
    return generateCustomHash(senderPublicKey + receiverPublicKey + std::to_string(amount) + timestamp);
}

// Grazina dabartini laika
std::string Transaction::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Parodo transakcijos informacija
void Transaction::display() const {
    std::cout << "-------------------- Transaction --------------------\n";
    std::cout << std::left << std::setw(20) << "Transaction ID" << ": " << transactionID << "\n";
    std::cout << std::left << std::setw(20) << "Timestamp"      << ": " << timestamp << "\n";
    std::cout << std::left << std::setw(20) << "Sender"         << ": " << senderPublicKey << "\n";
    std::cout << std::left << std::setw(20) << "Receiver"       << ": " << receiverPublicKey << "\n";
    std::cout << std::left << std::setw(20) << "Amount"         << ": " << amount << " units\n";
    std::cout << "------------------------------------------------------\n";
}
