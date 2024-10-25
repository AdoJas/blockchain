#ifndef BLOCKCHAIN_HASH_H
#define BLOCKCHAIN_HASH_H

#include <array>
#include <cstdint>
#include <iomanip>

const int HASH_SIZE = 32;
void computeHashFunction(unsigned int x, std::array<uint8_t, HASH_SIZE>& hashArray, unsigned int& previousY);
std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hashArray);
std::string generateCustomHash(const std::string& input);

#endif // BLOCKCHAIN_HASH_H
