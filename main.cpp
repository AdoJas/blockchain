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

//Random public key generavimo funkcija
std::string generateRandomPublicKey(int length = 64) {
    // Use random_device and mt19937 for better randomness
    std::random_device rd;
    std::mt19937 mt(rd());   // Mersenne Twister generatorius, random skaiciu generavimui
    std::uniform_int_distribution<int> dist(0, 255); // Random skaiciu generavimas nuo 0 iki 255

    std::ostringstream oss;
    for (int i = 0; i < length; ++i) {
        int byte = dist(mt); //Generuoja random baita ir pavercia i hex reiksme
        oss << std::setw(2) << std::setfill('0') << std::hex << byte;
    }
    return oss.str();
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
        user.publicKey = generateRandomPublicKey();
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
// Funkcija generuoti random transakcija
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
struct Block {
    std::string prevBlockHash;
    std::string merkleRoot;
    time_t timestamp;
    int nonce;
    int difficultyTarget;
    std::vector<Transaction> transactions;

    std::string calculateHash() const {
        std::stringstream ss;
        ss << prevBlockHash << merkleRoot << timestamp << nonce << difficultyTarget;
        return generateCustomHash(ss.str());
    }
    // Metodas isvedantis sio bloko transakcijas i konsole
    void displayTransactions() const {
        std::cout << "Block Transactions:" << std::endl;
        for (const auto& tx : transactions) {
            displayTransaction(tx);
        }
        std::cout << "======================" << std::endl;
    }
};
class UTXOPool {
public:
    // UTXO -> (owner, amount)
    std::unordered_map<std::string, std::pair<std::string, double>> utxos;

    bool validateTransaction(const Transaction& tx) {
        double totalInput = 0, totalOutput = 0;
        for (const auto& input : tx.inputs) {
            if (utxos.find(input) == utxos.end()) return false; // UTXO neegzistuoja
            totalInput += utxos[input].second;
        }
        for (const auto& output : tx.outputs) {
            totalOutput += /* value from output */ 0; // Placeholder, calculate from actual transaction
        }
        return totalInput >= totalOutput; // Inputu turi but daugiau nei outputu
    }

    void applyTransaction(const Transaction& tx) {
        for (const auto& input : tx.inputs) {
            utxos.erase(input); // Pazymim input UTXOs kaip isleistus
        }
        for (const auto& output : tx.outputs) {
            // Add new UTXOs
            utxos[output] = {tx.receiver, /* value from output */ 0}; // Placeholder, update with actual values
        }
    }
};
class Blockchain {
private:
    std::vector<Block> chain;
    UTXOPool utxoPool;

public:
    void addBlock(Block block) {
        chain.push_back(block);
        for (const auto& tx : block.transactions) {
            utxoPool.applyTransaction(tx);
        }
    }

    Block createBlock(const std::vector<Transaction>& transactions, const std::string& prevHash) {
        Block newBlock;
        newBlock.prevBlockHash = prevHash;
        newBlock.timestamp = std::time(nullptr);
        newBlock.transactions = transactions;
        newBlock.merkleRoot = "merkle_placeholder";  //TODO: implementuoti markle tree
        newBlock.nonce = 0;  // Pradine nonce reiksme
        return newBlock;
    }

    std::string getLastBlockHash() const {
        if (!chain.empty()) {
            return chain.back().calculateHash();
        }
        return "genesis";  // Grazinam genesis hasha jei nera daugiau bloku
    }

    // Mining loop for a block
    void mineBlock(Block& block) {
        int iterationCount = 0;
        std::string targetString(block.difficultyTarget, '0'); //Kuriam target stringa, priklausomai nuo sudetingumo

        while (true) {
            std::string hash = block.calculateHash(); // Calculate current hash
            if (hash.substr(0, block.difficultyTarget) == targetString) {
                std::lock_guard<std::mutex> guard(mtx);
                std::cout << "Block mined: " << hash << " with nonce: " << block.nonce << std::endl;
                break; // Jei jokie hashai nerasti, iseinam is loopo
            }
            block.nonce++;
            iterationCount++;

            // Printinam log kas 100000 iteraciju
            if (iterationCount % 100000 == 0) {
                std::cout << "Mining iteration: " << iterationCount << " Nonce: " << block.nonce << std::endl;
            }
        }
    }
    //TODO: Implementuoti paralelini mininima
    // Paralelinis mininima, gal bus panaudotas vėliau
    void parallelMineBlocks(std::vector<Block>& candidateBlocks) {
        std::vector<std::thread> miners;
        for (auto& block : candidateBlocks) {
            miners.emplace_back([this, &block]() {
                mineBlock(block);
            });
        }
        for (auto& miner : miners) {
            miner.join();
        }
    }
};

// UTXO pool'o klase

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random skaiciu generatoriui

    // Step 1: Generuojam atsitiktinius userius
    const int numUsers = 10;
    std::vector<User> users;
    for (int i = 0; i < numUsers; i++) {
        users.push_back(generateRandomUser());
    }

    // Step 2: generuojam atsitiktines transakcijas
    const int numTransactions = 100;
    std::vector<Transaction> transactions;
    for (int i = 0; i < numTransactions; i++) {
        transactions.push_back(generateRandomTransaction(users));
    }
    // Step 3: Sukuriam blockchain ir candidate blokus
    Blockchain blockchain;
    std::vector<Block> candidateBlocks(5); // Create 5 candidate blocks
    for (int i = 0; i < 5; ++i) {
        candidateBlocks[i] = blockchain.createBlock(transactions, blockchain.getLastBlockHash());
        candidateBlocks[i].difficultyTarget = 1; // mazas sudetingumas testavimui
    }
    return 0;
}
