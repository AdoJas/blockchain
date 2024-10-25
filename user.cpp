//
// Created by adoma on 10/24/2024.
//
#include "user.h"
std::unordered_set<std::string> existingPublicKeys;

//Random public key generavimo funkcija
std::string generateRandomPublicKey() {
    static std::mt19937 mt(static_cast<unsigned>(std::time(nullptr)));  // Seed with time to vary each run
    std::uniform_int_distribution<int> dist(0, 255);

    std::ostringstream oss;
    for (int i = 0; i < 64; ++i) {
        int byte = dist(mt);
        oss << std::setw(2) << std::setfill('0') << std::hex << byte;
    }
    return oss.str();
}

void randomUserGeneration(int userNumber, std::vector<User>& users){
    for (int i = 0; i < userNumber; ++i) {
        User user = generateRandomUser();
        users.push_back(user);  // Ensure this actually adds users
    }
}
User generateRandomUser() {
    static int userNumber = 0;

    if (userNumber == 0 && !existingPublicKeys.empty()) {
        for (const auto& key : existingPublicKeys) {
            if (key.find("User_") == 0) {
                int existingNumber = std::stoi(key.substr(5));
                if (existingNumber >= userNumber) {
                    userNumber = existingNumber + 1;
                }
            }
        }
    }
    User user;
    user.name = "User_" + std::to_string(userNumber++);

    do {
        user.publicKey = generateRandomPublicKey();
    } while (existingPublicKeys.find(user.publicKey) != existingPublicKeys.end());

    user.balance = static_cast<double>(rand() % 999901) + 100;
    printData(user);
    existingPublicKeys.insert(user.publicKey);
    return user;
}
User printData(User user) {
    std::cout << "Name: " << user.name << std::endl;
    std::cout << "Public key: " << user.publicKey << std::endl;
    std::cout << "Balance: " << user.balance << std::endl;
    std::cout << "-----------------------" << std::endl;
    return user;
}
