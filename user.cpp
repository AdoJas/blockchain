//
// Created by adoma on 11/5/2024.
//

// user.cpp
#include "user.h"
#include <iostream>

User::User(const std::string& publicKey) : publicKey(publicKey) {} // Konstruktorius

std::string User::getPublicKey() const { return publicKey; } // Grazina vartotojo public key

void User::display() const { // Parodo vartotojo viesaji rakta
    std::cout << "User Public Key: " << publicKey << "\n";
}
