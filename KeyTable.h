/*
 Filename: KeyTable.h
 Description: Declaration of the KeyTable class, to hold the key of each character contained in the huffman tree.
 Author: Noah Gallo
 Date: 06/02/2020
 Course: Data Structures II
*/

#ifndef HUFFMAN_QUILLO123_KEYTABLE_H
#define HUFFMAN_QUILLO123_KEYTABLE_H

#include <vector>
#include <ostream>
#include <iostream>


using std::ofstream;

using std::vector;
using std::ostream;

class KeyTable{
    struct Key{
        uint32_t keyCode = 0;
        uint8_t numBits = 0;
        u_char value = '\0';
    };
    Key _keyTable[256];
    vector<const Key*> _list;


public:
    KeyTable();

    bool Insert(char val, uint32_t code, size_t numBits);

    void CreateList();
    Key at(size_t index);
    size_t ListSize();

    void SaveToFile(ofstream &dest) const;

    bool GetVal(char &val, uint32_t code) const;
    bool GetCode(char val, uint8_t &numBits, uint32_t& code) const;

    void Print(ostream& out) const;



};


#endif //HUFFMAN_QUILLO123_KEYTABLE_H
