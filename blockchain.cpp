#include "blockchain.h"
#include "hash.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unordered_set>
#include <random>
#include <algorithm>

const int MAX_ATTEMPTS = 100000;  // Maksimalus bandymu skaicius kasant kandidatus blokus
const int MAX_TIME = 5;           // Maksimalus laikas sekundemis kasant kandidatus blokus

//Genesis bloko konstruktorius
Blockchain::Blockchain() {
    Block genesis("0", DIFFICULTY);
    genesis.mineBlock();
    chain.push_back(genesis);
}
//Funkcija, kuri grazina vartotojo UTXO tokenus
const std::vector<UTXO>& Blockchain::getUserUTXOs(const std::string& publicKey) const {
    static const std::vector<UTXO> emptyUTXOs;
    auto it = utxoPool.find(publicKey);
    return (it != utxoPool.end()) ? it->second : emptyUTXOs;
}

//Funkcija, kuri sugeneruoja vartotojus su random publicKey ir balansu
void Blockchain::generateUsers(int numUsers) {
    if (numUsers <= 0) {
        std::cout << "\n==================== User Generation ====================\n";
        std::cout << "Invalid number of users.\n";
        std::cout << "==========================================================\n";
        return;
    }
    int startingIndex = users.size();
    std::unordered_set<std::string> existingKeys(users.begin(), users.end());
    static std::mt19937 mt(static_cast<unsigned>(std::time(nullptr))); // Random generatoriaus seedas
    std::uniform_int_distribution<int> dist(0, 255);  // For public key bytes
    std::uniform_int_distribution<unsigned int> balanceDist(100, 1000000); // Pradinis balansas

    std::cout << "\n==================== User Generation ====================\n";

    for (int i = 0; i < numUsers; ++i) {
        // Unikalus 32 baitu public key generavimas(hex formatas)
        std::string publicKey;
        do {
            std::ostringstream oss;
            for (int j = 0; j < 32; ++j) {
                int byte = dist(mt);
                oss << std::setw(2) << std::setfill('0') << std::hex << byte;
            }
            publicKey = oss.str();
        } while (existingKeys.find(publicKey) != existingKeys.end());

        existingKeys.insert(publicKey);
        users.push_back(publicKey);

        // Random pradinis balansas UTXO velesniam generavimui
        unsigned int initialBalance = balanceDist(mt);

        // Sukuriam Genesis UTXO
        std::string transactionID = generateCustomHash(publicKey + std::to_string(initialBalance) + std::to_string(std::time(0)));
        UTXO genesisUTXO(publicKey, initialBalance, transactionID, initialBalance);
        utxoPool[publicKey].push_back(genesisUTXO);

        // Spausdinam User informacija
        std::cout << "\n------------------- User #" << (startingIndex + i + 1) << " -------------------\n";
        std::cout << std::left << std::setw(15) << "Public Key:" << publicKey << "\n";
        std::cout << std::left << std::setw(15) << "Initial Balance:" << initialBalance << " units\n";
        std::cout << "----------------------------------------------------------\n";
    }

    std::cout << "==========================================================\n\n";
}
//Funkcija, kuri grazina vartotojo balansa apskaiciuota pagal UTXO
unsigned int Blockchain::getUserBalance(const std::string& publicKey) const {
    unsigned int balance = 0;
    auto it = utxoPool.find(publicKey);
    if (it != utxoPool.end()) {
        for (const auto& utxo : it->second) {
            balance += utxo.getAmount();
        }
    }
    return balance;
}

//Funkcija, kuri grazina vartotojus
const std::vector<std::string>& Blockchain::getUsers() const {
    return users;
}

//Funkcija, kuri sukuria transakcija ir ja patikrina
void Blockchain::createTransaction(const Transaction& tx) {
    if (validateTransaction(tx, pendingTransactions)) {
        pendingTransactions.push_back(tx);
    }
}

//Funkcija, kuri validuoja transakcija, jei transakcija bloga, ji istrina is pendingTransactions
bool Blockchain::validateTransaction(const Transaction& tx, std::vector<Transaction>& pendingTransactions) const {
    std::string recalculatedHash = generateCustomHash(
            tx.getSender() + tx.getReceiver() + std::to_string(tx.getAmount()) + tx.getTimestamp());

    if (recalculatedHash != tx.getTransactionID()) {
        std::cout << "Transaction rejected: Hash mismatch for transaction ID " << tx.getTransactionID() << ".\n";
        pendingTransactions.erase(
                std::remove(pendingTransactions.begin(), pendingTransactions.end(), tx),
                pendingTransactions.end()
        );
        return false;
    }

    unsigned int senderBalance = getUserBalance(tx.getSender());
    if (tx.getAmount() > senderBalance) {
        std::cout << "Transaction rejected: Insufficient balance for sender " << tx.getSender()
                  << ". Has: " << senderBalance
                  << ", Tried to send: " << tx.getAmount() << " units.\n";
        pendingTransactions.erase(
                std::remove(pendingTransactions.begin(), pendingTransactions.end(), tx),
                pendingTransactions.end()
        );
        return false;
    }

    return true;
}

//Funkcija, kuri generuoja candidate blokus
void Blockchain::generateCandidateBlocks() {
    if (pendingTransactions.size() < 100) {
        std::cout << "Insufficient pending transactions to create candidate blocks.\n";
        return;
    }

    candidateBlocks.clear();
    for (int i = 0; i < 5; ++i) {
        Block candidate(chain.back().getHash(), DIFFICULTY);
        int txCount = 0;

        for (const auto& tx : pendingTransactions) {
            if (validateTransaction(tx, pendingTransactions)) {
                candidate.addTransaction(tx);
                txCount++;
                if (txCount >= 100) break;
            }
        }
        candidateBlocks.push_back(candidate);
    }

    std::cout << "5 candidate blocks created.\n";
}

// Funkcija, kuri kasa candidate blokus su apribojimais
bool Blockchain::mineCandidateBlockWithConstraints(Block& candidate, int maxAttempts, int maxTime) {
    int attempts = 0;
    time_t startTime = std::time(nullptr);

    while (attempts < maxAttempts && (std::time(nullptr) - startTime) < maxTime) {
        if (candidate.mineBlock()) {
            std::cout << "Candidate block mined successfully!\n";

            for (const auto& tx : candidate.getTransactions()) {
                processTransaction(tx);
            }

            auto &txs = candidate.getTransactions();
            pendingTransactions.erase(
                    std::remove_if(pendingTransactions.begin(), pendingTransactions.end(), // Pasalinam transakcijas is pendingTransactions
                                   [&txs](const Transaction& tx) {
                                       return std::find(txs.begin(), txs.end(), tx) != txs.end();
                                   }),
                    pendingTransactions.end()
            );

            chain.push_back(candidate);
            candidate.displayHeader(true);
            return true;
        }
        attempts++;
    }

    // Kasimo bandymas nepavyko, parodom info:
    std::cout << "Mining attempt for candidate block failed. Attempts: " << attempts
              << ", Time elapsed: " << (std::time(nullptr) - startTime) << " seconds.\n";
    return false;
}

// Funkcija, kuri kasa su candidate blokais ir laiko bei bandymu kiekio apribojimais
void Blockchain::mineWithCandidateBlocks() {
    bool blockMined = false;

    int attemptCount = MAX_ATTEMPTS;
    int timeLimit = MAX_TIME;

    while (!blockMined) {
        generateCandidateBlocks();
        std::cout << "Starting mining attempts with new candidate blocks.\n";
        std::cout << "Current constraints - Attempts: " << attemptCount << ", Time: " << timeLimit << " seconds.\n";

        for (int i = 0; i < candidateBlocks.size(); ++i) {
            if (mineCandidateBlockWithConstraints(candidateBlocks[i], attemptCount, timeLimit)) {
                blockMined = true;
                break;
            }
        }

        if (!blockMined) {
            std::cout << "All candidate blocks failed to mine. Doubling constraints and regenerating candidates...\n";
            attemptCount *= 2;
            timeLimit *= 2;
        }
    }
}

//Funkcija, kuri apdoroja transakcija ir atnaujina UTXO poola
void Blockchain::processTransaction(const Transaction& tx) {
    unsigned int remainingAmount = tx.getAmount();
    unsigned int totalSelectedAmount = 0;
    auto& senderUTXOs = utxoPool[tx.getSender()];

    std::vector<UTXO> spentUTXOs;
    for (auto it = senderUTXOs.begin(); it != senderUTXOs.end() && totalSelectedAmount < remainingAmount;) { // Surenka senderio utxos, neperlipant paskutinio utxo kuri senderis turi
        totalSelectedAmount += it->getAmount();
        spentUTXOs.push_back(*it);
        it = senderUTXOs.erase(it);
    }

    if (totalSelectedAmount < remainingAmount) {
        std::cerr << "Insufficient balance for transaction\n";
        return;
    }

    UTXO receiverUTXO(tx.getTransactionID(), 0, tx.getReceiver(), remainingAmount); // Sukuriam gavejo utxo
    utxoPool[tx.getReceiver()].push_back(receiverUTXO);

    unsigned int change = totalSelectedAmount - remainingAmount; // Sukuriam grazos utxo jei reikia
    if (change > 0) {
        UTXO changeUTXO(tx.getTransactionID(), 1, tx.getSender(), change);
        utxoPool[tx.getSender()].push_back(changeUTXO);
    }

}
//Funkcija, kuri prideda bloka i blockchaina
void Blockchain::addBlock() {
    if (pendingTransactions.empty()) {
        std::cout << "No pending transactions to mine.\n";
        return;
    }

    std::string prevHash = chain.back().getHash();

    Block newBlock(prevHash, DIFFICULTY, 1);

    std::cout << "Preparing to mine a new block...\n";
    newBlock.displayHeader(false);

    int transactionCount = std::min(100, static_cast<int>(pendingTransactions.size()));
    int successfulTransactions = 0;
    for (int i = 0; i < transactionCount; ++i) {
        const Transaction& tx = pendingTransactions[i];
        if (validateTransaction(tx, pendingTransactions)) {
            newBlock.addTransaction(tx);
            processTransaction(tx);
            successfulTransactions++;
        }
    }

    pendingTransactions.erase(pendingTransactions.begin(), pendingTransactions.begin() + transactionCount);

    newBlock.mineBlock();
    chain.push_back(newBlock);

    std::cout << "Successfully mined a new block with " << successfulTransactions << " transactions.\n";

    newBlock.displayHeader(true);
}
//Funkcija, kuri grazina blockchaina
const std::vector<Block>& Blockchain::getChain() const {
    return chain;
}

//Funkcija, kuri grazina pending transakcijas
const std::vector<Transaction>& Blockchain::getPendingTransactions() const {
    return pendingTransactions;
}

//Funkcija, kuri grazina paskutini bloka ir jo informacija
const Block& Blockchain::getLatestBlock() const {
    return chain.back();
}

//Funkcija, kuri grazina bloka pagal indexa ir jo visa informacija
const Block& Blockchain::getBlock(int index) const {
    return chain.at(index);
}
