// blockchain_console.h
#ifndef BLOCKCHAIN_CONSOLE_H
#define BLOCKCHAIN_CONSOLE_H

#include "blockchain.h"
#include <iostream>
#include <string>

class BlockchainConsole {
private:
    Blockchain& blockchain; // Blockchain objektas

public:
    BlockchainConsole(Blockchain& chain); // Konstruktorius

    // Display functions
    void displayBlockchain() const; // Parodo visa blockchaina
    void displayAllUsers() const;   // Parodo visus vartotojus
    void displayUserByIndex(int index) const; // Parodo vartotoja pagal indeksa
    void displayPendingTransactions() const; // Parodo laukiancias transakcijas
    void displayBlockByIndex(int index) const; // Parodo bloka pagal indeksa
    void displayTransactionFromBlock(int blockIndex, int txIndex) const; //Parodo specifine transakcija is specifinio bloko
    void displayLatestBlock() const; // Parodo paskutini bloka ir jo informacija

    // User valdymo funkcijos
    void generateUsers(int numUsers) const; // Funkcija kuri sukuria vartotojus
    void minePendingTransactions(); // Funkcija kuri kasa i joki bloka neitrauktas transakcijas
    void generateRandomTransactions(int numTransactions);  // New function for random transactions

    void mineWithCandidateBlocks();
    void run();  // Funkcija kuri paleidzia blockchain_console programa
};

#endif // BLOCKCHAIN_CONSOLE_H
