// main.cpp

#include <iostream>
#include "blockchain.h"
#include "blockchain_console.h"

int main() {
    Blockchain blockchain; // Sukuriamas blockchain objektas
    BlockchainConsole console(blockchain); // Sukuriamas blockchain console objektas

    console.run(); // Paleidziama blockchain console programa
    return 0;
}
