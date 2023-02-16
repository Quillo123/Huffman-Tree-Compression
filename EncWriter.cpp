/*
 Filename: EncWriter.cpp
 Description: Contains the definitions for the EncWriter class
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/

#include <fstream>
#include <string>
#include <iostream>
#include "KeyTable.h"
#include "EncWriter.h"

using std::string;
using std::ifstream;
using std::ofstream;

/**Helper Function**
 * Converts uint32_t to an array of chars
 * @param num - the numer to be converted
 * @param b - the array that will hold the returned value
 */
void UintToBytes(uint32_t num, u_char* b){
    for(int i = 0; i < 4; i++){
        b[i] =  num >> (i * 8);
    }
}

/**Helper function**
 * Clears Non-Essential bits to 0
 * @param b - the variable to clear bits on
 * @param start - amount of bits to keep
 */
void ClearBitsWriter(uint32_t & b, uint8_t start){
    unsigned int mask = (0x1 << start) - 1;
    b &= mask;
}

/**
 * Generates _byteStream based on the KeyTable and text file
 * @param src - the text file to convert
 * @param keyTable - the keytable to convert to
 * @return succeed/fail
 */
bool EncWriter::GenerateByteStream(ifstream &src, KeyTable keyTable) {
    _byteStream.push_back('\0');
    char c;
    uint8_t numBits;
    uint32_t code;
    while(src.read(&c, 1)){
        if(src.gcount() != 1){
            std::cerr << "Invalid Read" << std::endl;
            return false;
        }
        if(!keyTable.GetCode(c, numBits, code))
            return false;
        AddBits(code, numBits);

    }

    return true;
}

/**
 * Adds only the desired bits from the given code
 * @param code - the code to be inserted
 * @param numBits - the number of important bits in code
 */
void EncWriter::AddBits(uint32_t code, uint8_t numBits) {
    while(numBits > 0){
        if(_bitsLeft == 0){
            _byteStream.push_back('\0');
            _bitsLeft = 8;
        }

        if(_bitsLeft >= numBits){
            u_char c = '\0';
            c |= (code << (_bitsLeft - numBits));
            _bitsLeft -= numBits;
            _totalBits += numBits;
            _byteStream.back() |= c;
            return;
        }
        else{
            u_char c = '\0';
            c |= (code >> (numBits - _bitsLeft));
            ClearBitsWriter(code, numBits - _bitsLeft);
            _byteStream.back() |= c;
            _totalBits += _bitsLeft;
            numBits -= _bitsLeft;
            _bitsLeft = 0;
        }
    }
}

/**
 * encodes src and writes it to a file
 * @param src - a text file to be encoded
 * @param dest - the file to to write to
 * @param tree - contains the tree and keytable for encoding
 * @return
 */
bool EncWriter::WriteToFile(ifstream &src, ofstream &dest, HuffTree& tree) {
    tree.GetKey().SaveToFile(dest);
    GenerateByteStream(src, tree.GetKey());

    uint32_t num = 0;
    for(int i = 0; i < 4; i++){
        num |= _byteStream[i];
        if(i != 3) num <<= 8;
    }

    u_char a[4];
    UintToBytes(_totalBits, a);

    for(int i = 0; i < 4; i++)
        _byteStream.insert(_byteStream.begin(), a[i]);
    dest.write((char*)_byteStream.data(), _byteStream.size());
    return true;
}













