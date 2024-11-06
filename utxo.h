// utxo.h
#ifndef UTXO_H
#define UTXO_H

#include <string>

class UTXO {
private:
    std::string utxoID;        // Unikalus UTXO id
    std::string owner;         // UTXO savininko public key
    unsigned int amount;       // UTXO tokeno verte

public:
    UTXO(const std::string& txID, int index, const std::string& ownerKey, unsigned int amt); // Konstruktorius

    std::string getUTXOId() const; // Grazina UTXO id
    std::string getOwner() const; // Grazina UTXO savininko public key
    unsigned int getAmount() const; // Grazina UTXO tokeno verte
};

#endif // UTXO_H
