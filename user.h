//
// Created by adoma on 11/5/2024.
//

// user.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string publicKey; // Vartotojo public key

public:
    User(const std::string& publicKey); // Konstruktorius

    std::string getPublicKey() const; // Grazina vartotojo public key
    void display() const;  // Parodo vartotojo public key
};

#endif // USER_H

