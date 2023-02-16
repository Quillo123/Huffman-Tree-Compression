/*
 Filename: EncReader.h
 Description: Declaration of the class EncReader, which reads information from an ncoded file
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/

#ifndef HUFFMAN_QUILLO123_ENCREADER_H
#define HUFFMAN_QUILLO123_ENCREADER_H

#include <fstream>
#include "KeyTable.h"

using std::ifstream;
using std::ofstream;

class EncReader{
    vector<u_char> _byteStream;
    uint32_t _totalBits{};
    uint8_t _bitsLeft{};



public:
    EncReader();

    bool LoadByteStream(ifstream& src);
    bool GetNextBit(u_char& byte);
    KeyTable GetKeyTable();

    bool eof();

};


#endif //HUFFMAN_QUILLO123_ENCREADER_H
