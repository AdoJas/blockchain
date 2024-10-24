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

extern std::unordered_set<std::string> existingPublicKeys;

std::string generateRandomPublicKey();

struct User {
    std::string name;
    std::string publicKey;
    double balance;
};

// Declaration of generateRandomUser
User generateRandomUser();

void randomUserGeneration(int& userNumber, std::vector<User> users);
#endif //BLOCKCHAIN_USER_H
