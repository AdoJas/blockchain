//
// Created by adoma on 10/24/2024.
//

#ifndef BLOCKCHAIN_USER_H
#define BLOCKCHAIN_USER_H
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <iostream>
#include "transactionUTXO.h"

class UTXOPool;

class User {
public: User();
    explicit User(const std::string& name);

    std::string getName() const;
    std::string getPublicKey() const;
    void display(const UTXOPool& utxoPool) const;

private:
    std::string name;
    std::string publicKey;
    static std::unordered_set<std::string> existingPublicKeys;

    static std::string generateRandomPublicKey();
};
#endif //BLOCKCHAIN_USER_H
