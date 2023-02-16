/*
 Filename: KeyTable.cpp
 Description: Contains the definitions for the KeyTable class
 Author: Noah Gallo
 Date: 06/02/2020
 Course: Data Structures II
*/

#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <bitset>
#include "KeyTable.h"

using std::ostream;
using std::ofstream;
using std::ifstream;
using std::string;

void PrintCompressed(ostream& out, uint32_t num, size_t numBits);


/**Constructor
 *
 */
KeyTable::KeyTable(){
    for(size_t i = 0; i < 256; i++){
        _keyTable[i].value = i;
    }
}

/**
 * inserts an element into the keyTable
 * @param val - the char of the inserted element
 * @param code - the code of the element
 * @param numBits - the number of importants bits of code
 * @return succeed/fail
 */
bool KeyTable::Insert(char val, uint32_t code, size_t numBits) {
    Key element;
    _keyTable[static_cast<size_t>(val)].keyCode = code;
    _keyTable[static_cast<size_t>(val)].numBits = numBits;
    return true;
}

/**
 * creates a sorted list of all the elements of _keyTable that have value
 */
void KeyTable::CreateList() {
    for(const auto & i: _keyTable){
        if(i.numBits != 0){
            if(_list.empty()) _list.push_back(&i);
            else{
                for(size_t j = 0; j < _list.size(); j++){
                    if(i.numBits == _list[j]->numBits){
                        if(i.value < _list[j]->value){
                            _list.insert(_list.begin() + j, &i);
                            break;
                        }
                    }
                    if(i.numBits < _list[j]->numBits) {
                        _list.insert(_list.begin() + j, &i);
                        break;
                    }
                    if(j == _list.size() - 1){
                        _list.push_back(&i);
                        break;
                    }
                }
            }
        }
    }
}

/**
 * finds the key at the given index
 * @param index - the index in the list to find
 * @return the Key of the element
 */
KeyTable::Key KeyTable::at(size_t index) {
    if(_list.empty())
        CreateList();
    return *_list[index];
}

/**
 *
 * @return the size of _list
 */
size_t KeyTable::ListSize(){
    return _list.size();
}

/**
 * saves an encoded version of the KeyTable to a file
 * @param dest - the file to save to
 */
void KeyTable::SaveToFile(ofstream &dest) const {
    vector<u_char> toFile;
    for(const auto & i: _keyTable){
        if(i.numBits != 0){
            toFile.push_back(i.value);
            toFile.push_back(i.numBits);
            for(int j = 3; j >= 0; j--){
                u_char temp = '\0';
                temp |= i.keyCode >> (j * 8);
                toFile.push_back(temp);
            }
        }
    }
    uint32_t tBytes = toFile.size();
    for(int i = 0; i < 4; i++){
        u_char temp = '\0';
        temp |= tBytes >> (i * 8);
        toFile.insert(toFile.begin(), temp);
    }
    dest.write((char*)toFile.data(), toFile.size());
}

/**
 * Prints the key to an ostream
 * @param out - the output stream
 */
void KeyTable::Print(ostream &out) const{
    if(_list.empty()){
        vector<Key> printList;
        /**Creates Sorted Vector**/
        for(const auto & i: _keyTable){
            if(i.numBits != 0){
                if(printList.empty()) printList.push_back(i);
                else{
                    for(size_t j = 0; j < printList.size(); j++){
                        if(i.numBits == printList[j].numBits){
                            if(i.value < printList[j].value){
                                printList.insert(printList.begin() + j, i);
                                break;
                            }
                        }
                        if(i.numBits < printList[j].numBits) {
                            printList.insert(printList.begin() + j, i);
                            break;
                        }
                        if(j == printList.size() - 1){
                            printList.push_back(i);
                            break;
                        }
                    }
                }
            }
        }
        /**Prints Vector**/
        for(auto & i: printList){
            if (i.value == '\n')
                out << "{key:" << std::setw(3) << "CR" << ", keyCode: ";
            else if (i.value == '\r')
                out << "{key:" << std::setw(3) << "LF" << ", keyCode: ";
            else if(i.value == '\0')
                out << "{key:" << std::setw(3) << "NU" << ", keyCode: ";
            else
                out << "{key:" << std::setw(3) << i.value << ", keyCode: ";
            PrintCompressed(out, i.keyCode, i.numBits);
            out << "}\n";
        }
    }
    else {
        /**Prints Vector**/
        for (auto &i: _list) {
            if (i->value == '\n')
                out << "{key:" << std::setw(3) << "CR" << ", keyCode: ";
            else if (i->value == '\r')
                out << "{key:" << std::setw(3) << "LF" << ", keyCode: ";
            else if (i->value == '\0')
                out << "{key:" << std::setw(3) << "NU" << ", keyCode: ";
            else
                out << "{key:" << std::setw(3) << i->value << ", keyCode: ";
            PrintCompressed(out, i->keyCode, i->numBits);
            out << "}\n";
        }
    }

}

/**
 * prints the desired binary from the given number
 * @param out - the stream to output to
 * @param num - the number to print
 * @param numBits - the amount of bits to print from the number
 */
void PrintCompressed(ostream& out, uint32_t num, size_t numBits){
    std::bitset<32> bit = std::bitset<32>(num);
    for(int i = numBits - 1; i >= 0; i--){
        out << bit[i];
    }
}

/**
 * using the given keyCode, retriences a char from the keyTable
 * @param code - the keyCode to search for
 * @param val - the value to return to
 * @return true/false based on if it could find the keyCode
 */
bool KeyTable::GetVal(char &val, uint32_t code) const {
    for(const auto & i : _keyTable){
        if(i.keyCode == code) {
            val = i.value;
            return true;
        }
    }
    return false;
}

/**
 * returns the given char's code
 * @param val - the char to search for
 * @param numBits - the number of important bits of the code
 * @param code - the code of thr char
 * @return succeed/fail
 */
bool KeyTable::GetCode(char val, uint8_t &numBits, uint32_t& code) const {
    if(_keyTable[static_cast<size_t>(val)].numBits == 0) return false;
    else{
        code = _keyTable[static_cast<size_t>(val)].keyCode;
        numBits = _keyTable[static_cast<size_t>(val)].numBits;
    }
    return true;
}




