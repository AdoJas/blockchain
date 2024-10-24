//
// Created by adoma on 10/24/2024.
//
#include "user.h"
std::unordered_set<std::string> existingPublicKeys;
//Random public key generavimo funkcija
std::string generateRandomPublicKey() {
    // Use random_device and mt19937 for better randomness
    std::random_device rd;
    std::mt19937 mt(rd());   // Mersenne Twister generatorius, random skaiciu generavimui
    std::uniform_int_distribution<int> dist(0, 255); // Random skaiciu generavimas nuo 0 iki 255

    std::ostringstream oss;
    for (int i = 0; i < 64; ++i) {
        int byte = dist(mt); //Generuoja random baita ir pavercia i hex reiksme
        oss << std::setw(2) << std::setfill('0') << std::hex << byte;
    }
    return oss.str();
}
void randomUserGeneration(int& userNumber, std::vector<User> users){
    const int numUsers = 10;
    for (int i = 0; i < numUsers; i++) {
        users.push_back(generateRandomUser());
    }
}
User generateRandomUser() {
    User user;
    user.name = "User_" + std::to_string(rand() % 1000);
    do {
        user.publicKey = generateRandomPublicKey();
    } while (existingPublicKeys.find(user.publicKey) != existingPublicKeys.end());
    user.balance = static_cast<double>(rand() % 999901) + 100; // balanso reiksme nuo 100 iki 10^6 valiutos vienetu
    return user;
}
