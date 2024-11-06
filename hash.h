//
// Created by adoma on 11/5/2024.
//

// hash.h
#ifndef BLOCKCHAIN_HASH_H
#define BLOCKCHAIN_HASH_H

#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

const int HASH_SIZE = 32;  // Hash dydis baitais 32 baitai

void computeHashFunction(unsigned int x, std::array<uint8_t, HASH_SIZE>& hashArray, unsigned int& previousY);
std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hashArray);
std::string generateCustomHash(const std::string& input);

#endif // BLOCKCHAIN_HASH_H

