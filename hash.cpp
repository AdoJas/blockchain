//
// Created by adoma on 10/24/2024.
//

#include "hash.h"

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