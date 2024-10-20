#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <random>
#include <functional>
#include <thread>
#include <sstream>
#include <iomanip>
#include <array>
#include <mutex>
#include <unordered_set>

std::mutex mtx;

const int HASH_SIZE = 32; // hashArray dydis baitais
std::unordered_set<std::string> existingPublicKeys;
// 1 uzduoties hash funkcija
void computeHashFunction(unsigned int x, std::array<uint8_t, HASH_SIZE>& hashArray, unsigned int& previousY) {
    unsigned int p1 = 2654435761; // Knuth's multiplication constant
    unsigned int p2 = 1597334677; // Random prime
    unsigned int p3 = 2246822519; // Random prime
    unsigned int p4 = 3266489917; // Random prime

    for (int i = 0; i < HASH_SIZE; i++) {
        unsigned int uniqueInput = (x ^ previousY) + (i * p1);

        uniqueInput = (uniqueInput << (i % 16)) | (uniqueInput >> (16 - (i % 16)));
        unsigned int y = (13 * uniqueInput + 17 * (uniqueInput * uniqueInput)
                          + 5 * (previousY * previousY)
                          + ((previousY ^ uniqueInput) * p3)
                          + ((uniqueInput * uniqueInput * uniqueInput) % 31) * p4);

        y = (y ^ (y << 13)) ^ ((y >> 11) | (previousY << (i % 8)));

        previousY = ((previousY * p1) ^ (y + i * p2) + (previousY << 5)) % 1048576;

        // Update the hash array
        hashArray[i] ^= (y & 0xFF);
        hashArray[i] ^= ((y >> 8) & 0xFF);
        hashArray[i] ^= ((y >> 16) & 0xFF);
        hashArray[i] ^= ((y >> 24) & 0xFF);
    }
}

// Bitu konvertavimas i hex
std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hashArray) {
    std::ostringstream oss;
    for (const auto& byte : hashArray) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
    }
    return oss.str();
}

// Hasho skaiciavimo pritaikymas
std::string generateCustomHash(const std::string& input) {
    std::array<uint8_t, HASH_SIZE> hashArray = {0};
    unsigned int previousY = 0;

    // Process each character in the string
    for (char c : input) {
        unsigned int decimalValue = static_cast<unsigned int>(c);
        computeHashFunction(decimalValue, hashArray, previousY);
    }

    return toHexString(hashArray);
}

// User struktura
struct User {
    std::string name;
    std::string publicKey;
    double balance;
};
User generateRandomUser() {
    User user;
    user.name = "User_" + std::to_string(rand() % 1000);
    do {
        user.publicKey = "PublicKey_" + std::to_string(rand() % 10000); // generuojam atsitiktini public key
    } while (existingPublicKeys.find(user.publicKey) != existingPublicKeys.end());
    user.balance = static_cast<double>(rand() % 999901) + 100; // balanso reiksme nuo 100 iki 10^6 valiutos vienetu
    return user;
}

// UTXO modelio transakciju struktura
struct Transaction {
    std::string txID;
    std::string sender;
    std::string receiver;
    double amount;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};
// Funkcija transakcijos isvedimui i konsole
void displayTransaction(const Transaction& tx) {
    std::cout << "Transaction ID: " << tx.txID << std::endl;
    std::cout << "Sender: " << tx.sender << std::endl;
    std::cout << "Receiver: " << tx.receiver << std::endl;
    std::cout << "Amount: " << tx.amount << std::endl;
    std::cout << "Inputs: ";
    for (const auto& input : tx.inputs) {
        std::cout << input << " ";
    }
    std::cout << "\nOutputs: ";
    for (const auto& output : tx.outputs) {
        std::cout << output << " ";
    }
    std::cout << "\n-----------------------" << std::endl;
}
// Function to generate a random transaction
Transaction generateRandomTransaction(const std::vector<User>& users) {
    Transaction tx;
    tx.txID = "TX_" + std::to_string(rand() % 10000);
    const auto& sender = users[rand() % users.size()];
    const auto& receiver = users[rand() % users.size()];
    tx.sender = sender.publicKey;
    tx.receiver = receiver.publicKey;
    tx.amount = static_cast<double>(rand() % 100) + 0.01; // Random amount
    tx.inputs.push_back("UTXO_" + std::to_string(rand() % 10000)); // Add random UTXO
    tx.outputs.push_back("UTXO_" + std::to_string(rand() % 10000)); // Add new UTXO
    return tx;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random skaiciu generatoriui

    // Step 1: Generuojam atsitiktinius userius
    const int numUsers = 10;
    std::vector<User> users;
    for (int i = 0; i < numUsers; i++) {
        users.push_back(generateRandomUser());
    }

    // Step 2: generuojam atsitiktines transakcijas
    const int numTransactions = 10;
    std::vector<Transaction> transactions;
    for (int i = 0; i < numTransactions; i++) {
        transactions.push_back(generateRandomTransaction(users));
    }

    return 0;
}
