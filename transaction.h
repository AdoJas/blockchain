// transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string transactionID; // Transakcijos ID
    std::string senderPublicKey; // Siuntejo public key
    std::string receiverPublicKey; // Gavejo public key
    unsigned int amount; // Transakcijos suma
    std::string timestamp;  // Transakcijos laikas

public:
    Transaction(const std::string& senderPublicKey, const std::string& receiverPublicKey, unsigned int amt); // Transakcijos konstruktorius

    std::string getTransactionID() const; //Grazina transakcijos ID
    std::string getSender() const; // Grazina siuntejo public key
    std::string getReceiver() const; // Grazina gavėjo public key
    unsigned int getAmount() const; // Grazina transakcijos suma
    std::string getTimestamp() const; // Grazina transakcijos laika
    std::string getCurrentTimestamp() const; // Grazina dabartini laika
    std::string generateTransactionID() const; // Generuoja transakcijos ID
    void display() const; //Parodo transakcijos informacija

    bool operator==(const Transaction& other) const { // pagalbine palyginimo funkcija
        return transactionID == other.transactionID;
    }
};

#endif // TRANSACTION_H
