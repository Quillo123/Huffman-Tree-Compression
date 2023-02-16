/*
 Filename: EncReader.cpp
 Description: Contains the definitions for the EncReader class
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include "EncReader.h"
#include "HuffTree.h"

using std::ifstream;
using std::ofstream;
using std::min;

/**Helper Function**
 * converts an array of 4 char into a uint32_t
 * @param num - the uint to be returned
 * @param b - the char array to be converted
 */
void BytesToUint(uint32_t& num, u_char* b){
    num = 0;
    for(int i = 0; i < 4; i++){
        num |= b[i];
        if(i != 3) num <<= 8;
    }
}

/**Helper function**
 * Clears Non-Essential bits to 0
 * @param b - the variable to clear bits on
 * @param start - amount of bits to keep
 */
void ClearBits(u_char& b, uint8_t start){
    unsigned int mask = (0x1 << start) - 1;
    b &= mask;
}

/**
 * Loads _byteStream with the data from the file
 * @param src - the file to recieve data from
 * @return succeed/fail
 */
bool EncReader::LoadByteStream(ifstream &src) {
    src.seekg(0, std::ios::end);
    uint filesize = src.tellg();
    src.seekg(0, std::ios_base::beg);

    for(uint i = 0; i < filesize; i++){
        u_char a;
        src.read((char*)&a, 1);
        _byteStream.push_back(a);
    }

    if(_byteStream.size() < 4){
        std::cerr << "Invalid File Size.";
        return false;
    }
    return true;
}

/**
 * Gets the next bit out of _byteStream
 * @param byte - used to return the next bit
 * @return succeed/fail
 */
bool EncReader::GetNextBit(u_char &byte) {
    if(_totalBits == 0 || _byteStream.empty())
        return false;

    byte = _byteStream.at(0) >> 7;
    ClearBits(byte, 1);

    _totalBits--;
    _bitsLeft--;
    _byteStream[0] <<= 1;

    if(_bitsLeft == 0){
        _byteStream.erase(_byteStream.begin());
        _bitsLeft = min(_totalBits, static_cast<uint32_t>(8));
    }
    return true;
}

/**
 * Pulls the KeyTable out of the file
 * @return the KeyTable
 */
KeyTable EncReader::GetKeyTable() {

    uint32_t numBytes;
    BytesToUint(numBytes, _byteStream.data());
    _byteStream.erase(_byteStream.begin(), _byteStream.begin() + 4);
    KeyTable keyTable;

    while (numBytes > 0){
        u_char val = _byteStream.at(0);
        _byteStream.erase(_byteStream.begin());
        uint8_t numBits = _byteStream.at(0);
        _byteStream.erase(_byteStream.begin());
        uint32_t code;
        BytesToUint(code, _byteStream.data());
        _byteStream.erase(_byteStream.begin(), _byteStream.begin() + 4);

        keyTable.Insert(val, code, numBits);

        numBytes -= 6;
    }

    BytesToUint(_totalBits, _byteStream.data());
    _byteStream.erase(_byteStream.begin(), _byteStream.begin() + 4);
    _bitsLeft = min(_totalBits, static_cast<uint32_t>(8));

    return keyTable;
}

/**
 * determines if the _byteStream has reached its end
 * @return true/false based on whether _byteStream is empty
 */
bool EncReader::eof() {
    return !_totalBits;
}

EncReader::EncReader() = default;
