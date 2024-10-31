//
// Created by adoma on 10/24/2024.
//
#include "user.h"
std::unordered_set<std::string> User::existingPublicKeys;

User::User() : name("User_" + std::to_string(existingPublicKeys.size() + 1)),
               publicKey(generateRandomPublicKey()), balance(static_cast<double>(rand() % 999901) + 100) {
    existingPublicKeys.insert(publicKey);
}

User::User(const std::string& name) : name(name), publicKey(generateRandomPublicKey()), balance(static_cast<double>(rand() % 999901) + 100) {
    existingPublicKeys.insert(publicKey);
}

std::string User::getName() const {
    return name;
}

std::string User::getPublicKey() const {
    return publicKey;
}

void User::updateBalance(double amount) {
    balance += amount;
}

double User::getBalance() const {
    return balance;
}
//Random public key generavimo funkcija
std::string User::generateRandomPublicKey() {
    static std::mt19937 mt(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 255);
    std::ostringstream oss;
    for (int i = 0; i < 32; ++i) {
        int byte = dist(mt);
        oss << std::setw(2) << std::setfill('0') << std::hex << byte;
    }
    return oss.str();
}

void randomUserGeneration(int userNumber, std::vector<User>& users){
    for (int i = 0; i < userNumber; ++i) {
        users.emplace_back();
    }
}

void User::display() const {
    std::cout << "=================================== User ===================================\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Public key: " << publicKey << "\n";
    std::cout << "Balance: " << balance << "\n";
    std::cout << "============================================================================\n";
}
