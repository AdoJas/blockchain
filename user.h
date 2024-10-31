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

extern std::unordered_set<std::string> existingPublicKeys;

std::string generateRandomPublicKey();

class User {
public: User();
    explicit User(const std::string& name);

    std::string getName() const;
    std::string getPublicKey() const;
    double getBalance() const;
    void updateBalance(double amount);
    void display() const;

private:
    std::string name;
    std::string publicKey;
    double balance;
    static std::unordered_set<std::string> existingPublicKeys;

    static std::string generateRandomPublicKey();
};

User generateRandomUser();
User printData(User user);
void randomUserGeneration(int userNumber, std::vector<User>& users);
#endif //BLOCKCHAIN_USER_H
