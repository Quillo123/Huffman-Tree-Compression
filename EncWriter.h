/*
 Filename: EncWriter.h
 Description: Declaration of the EncWriter class, which Writes the encoded data to a file
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/
#ifndef HUFFMAN_QUILLO123_ENCWRITER_H
#define HUFFMAN_QUILLO123_ENCWRITER_H

#include <fstream>
#include <string>
#include "KeyTable.h"
#include "HuffTree.h"

using std::ifstream;
using std::ofstream;
using std::string;

class EncWriter{

    vector<u_char> _byteStream;
    uint32_t _totalBits;
    uint8_t _bitsLeft = 8;

    bool GenerateByteStream(ifstream &src, KeyTable keyTable);
    void AddBits(uint32_t code, uint8_t numBits);

public:
    EncWriter(): _totalBits(0) {
    }

    bool WriteToFile(ifstream &src, ofstream &dest, HuffTree& tree);
};


#endif //HUFFMAN_QUILLO123_ENCWRITER_H
