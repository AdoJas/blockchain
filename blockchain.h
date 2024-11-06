#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <map>
#include <vector>
#include <string>
#include "block.h"
#include "utxo.h"
#include "transaction.h"

class Blockchain {
private:
    static const int DIFFICULTY = 5; // Kasimo sunkumas siam pavyzdziui
    std::vector<Block> chain; // Blockchainas
    std::map<std::string, std::vector<UTXO>> utxoPool;  // UTXO poolas pagal public key
    std::vector<Transaction> pendingTransactions;
    std::vector<std::string> users;  // Useriu public keys

    std::vector<Block> candidateBlocks; // Kandidatai blokai

public:
    Blockchain(); // Konstruktorius

    // User valdymas
    void generateUsers(int numUsers); // Sukuria nurodyta kieki vartotoju
    unsigned int getUserBalance(const std::string& publicKey) const; // Grazina vartotojo balansa
    const std::vector<std::string>& getUsers() const; // Grazina vartotoju public keys

    // Transakciju ir bloku valdymas
    void createTransaction(const Transaction& tx); // Sukuria nauja transakcija
    bool validateTransaction(const Transaction& tx, std::vector<Transaction>& pendingTransactions) const; // Patikrina transakcija
    void addBlock(); // Prideda nauja bloka i blockchaina (cia realiai ir vyksta kasimas)
    const std::vector<UTXO>& getUserUTXOs(const std::string& publicKey) const; // Grazina vartotojo UTXO

    // Kandidatu bloku kasimas
    void generateCandidateBlocks(); // Sugeneruoja kandidatus blokus
    bool mineCandidateBlockWithConstraints(Block& candidate, int maxAttempts, int maxTime); // Kasa kandidata bloka su apribojimais
    void mineWithCandidateBlocks(); // Kasa kandidatus blokus

    // Blockchain inspection
    const std::vector<Block>& getChain() const; // Grazina blockchaina
    const std::vector<Transaction>& getPendingTransactions() const; // Grazina laukiancias transakcijas
    const Block& getLatestBlock() const; // Grazina paskutini bloka
    const Block& getBlock(int index) const; // Grazina bloka pagal indeksa

private:
    void processTransaction(const Transaction& tx);  // Transakcijos apdorojimas ir utxo pool atnaujinimas
};

#endif // BLOCKCHAIN_H
