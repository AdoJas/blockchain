#include "utxo.h"
#include "hash.h"

UTXO::UTXO(const std::string& txID, int index, const std::string& ownerKey, unsigned int amt) // Konstruktorius
        : owner(ownerKey), amount(amt) {
    // Sukombinuojam duomenis, kuriuos naudosim hashavimui / transakcijosID-index-ownerKey-amount
    std::string dataToHash = txID + "-" + std::to_string(index) + "-" + ownerKey + "-" + std::to_string(amt);
    utxoID = generateCustomHash(dataToHash);  // Generate a hashed UTXO ID
}

std::string UTXO::getUTXOId() const { // Grazina UTXO id
    return utxoID;
}

std::string UTXO::getOwner() const { // Grazina UTXO savininko public key
    return owner;
}

unsigned int UTXO::getAmount() const { // Grazina UTXO tokeno verte
    return amount;
}
