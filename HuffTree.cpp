/*
 Filename: HuffTree.cpp
 Description: Contains the definitions for the HuffTree class
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include "HuffTree.h"
#include "KeyTable.h"

using std::ifstream;
using std::vector;
using std::string;
using std::istream;
using std::endl;
using std::cerr;

/**Helper function**
 * Clears Non-Essential bits to 0
 * @param b - the variable to clear bits on
 * @param start - amount of bits to keep
 */
void ClearBits(uint32_t& b, uint8_t start)
{
    unsigned int mask = (0x1 << start) - 1;
    b &= mask;
}

/**Helper Function**
 * gets the next bit based on the number of bits stored
 * @param code - the code to get the next bit from
 * @param numBits - number of bits stored in 'code'
 * @return true for 1, false for 0
 */
bool NextBit(uint32_t code, uint8_t numBits){
    code >>= numBits - 1;
    ClearBits(code, 1);
    return code;

}

/**Private**/

/**Constructor
 *
 * @param root - the tree to be used
 * @param height - the height of the tree
 */
HuffTree::HuffTree(Node *root, size_t height) : _root(root), _height(height){
    CreateKey();
    _key.CreateList();
}

/**Constructor
 *
 * @param key - Keytable from which to construct the tree
 */
HuffTree::HuffTree(KeyTable key): _key(key){
    _key.CreateList();
    _key.Print(std::cout);
    _root = new Node;
    for(size_t i = 0; i < _key.ListSize(); i++)
        CreateTreeFromKey(_key.at(i).value, _key.at(i).keyCode, _key.at(i).numBits, _root);
}

/**
 * clears the tree
 * @param root - the tree to be deleted
 */
void HuffTree::clear(HuffTree::Node *root) {
    if(root == nullptr) return;
    else{
        clear(root->left);
        clear(root->right);
        delete root;
    }
}

/**
 * Creates a tree from _key recursively
 * @param val - the next value to be added
 * @param code - the code of that value
 * @param numBits - the number of bits in the code that matter
 * @param root - the tree to insert into
 */
void HuffTree::CreateTreeFromKey(const u_char &val, uint32_t code, uint8_t numBits, Node *root) {
    if(NextBit(code, numBits)){
        if(root->left == nullptr){
            if(numBits == 1){
                Node* neo = new Node;
                neo->value = val;

                root->left = neo;
                neo->parent = root;
                return;
            }
            else{
                Node* neo = new Node;
                root->left = neo;
                neo->parent = root;
            }
        }
        CreateTreeFromKey(val, code, numBits - 1, root->left);
        return;
    }
    else{
        if(root->right == nullptr){
            if(numBits == 1){
                Node* neo = new Node;
                neo->value = val;

                root->right = neo;
                neo->parent = root;
                return;
            }
            else{
                Node* neo = new Node;
                root->right = neo;
                neo->parent = root;
            }
        }
        CreateTreeFromKey(val, code, numBits - 1, root->right);
        return;
    }
}

/**
 * Creates the frequency table based on a text file
 * @param src - the text file to build the table from
 * @param compTable - the frequency table
 */
void HuffTree::CreateFrequencyTable(ifstream &src, vector<Node*>& compTable) {
    Node** table = new Node*[256];

    for(size_t i = 0; i < 256; i++){
        table[i] = new Node;
        table[i]->value = i;
    }

    char in;
    while(src.read(&in, 1)){
        if(src.gcount() != 1){
            cerr << "Invalid Read" << endl;
        }
        table[static_cast<size_t>(in)]->count++;
    }

    for(size_t i = 0; i < 256; i++){
        if(table[i]->count != 0) {
            compTable.push_back(table[i]);
        }
    }

    SortTable(compTable);

    for(size_t i = 0; i < 256; i++){
        if(table[i]->count == 0)
            delete table[i];
    }
    delete[] table;
}

/**
 * creates _key from the tree
 */
void HuffTree::CreateKey(){
    KeyTraverse(_root, 0, 0);
}

/**
 * recursively traverses the tree in order to generate the key
 * @param root - the tree to be traversed
 * @param numBits - the number of bits currently in the code
 * @param code - the code for getting to the current Node
 */
void HuffTree::KeyTraverse(HuffTree::Node *root, size_t numBits, uint32_t code) {
    if(root->left == nullptr && root->right == nullptr){
        _key.Insert(root->value, code, numBits);
        if(numBits > _height)
            _height = numBits;
    }
    else{
        code <<= 1;
        numBits++;
        if(numBits == 32) {
            std::cerr << "Invalid keyCode size" << std::endl;
            assert(false);
        }
        KeyTraverse(root->right, numBits, code);
        KeyTraverse(root->left, numBits, code | 1);
    }
}

/**
 * recursively prints the tree to a stream (for testing purposes)
 * @param out - the stream to outpu to
 * @param root - the tree to be outputted
 * @param num - how deep in the tree i currently am
 */
void HuffTree::PrintToStream(ostream &out, HuffTree::Node *root, int num) {
    if(root == nullptr) {
        out << "\n";
        return;
    }
    else{
        if(root->value == '\r')
            out << "KeyTable: " << "LF" << ", Count: "<< root->count << '\n';
        else if(root->value == '\n')
            out << "KeyTable: " << "CR" << ", Count: "<< root->count << '\n';
        else
            out << "KeyTable: " << root->value << ", Count: "<< root->count << '\n';
        out << std::setw(num * 2);
        PrintToStream(out, root->left, num + 1);
        out << std::setw(num * 2);
        PrintToStream(out, root->right, num + 1);
    }
}

/**
 * Gets the char from the tree based on the bits pulled from reader
 * @param reader - the EncReader from which to get the bits
 * @param node - the current node
 * @param fail - determines if the functions fails
 * @return
 */
u_char HuffTree::GetChar(EncReader &reader, HuffTree::Node *node, bool &fail) {
    if(node->left == nullptr && node->right == nullptr)
        return node->value;
    u_char c;
    if(!reader.GetNextBit(c)){
        cerr << "Reached end of file without new code" << endl;
        fail = true;
        return '\0';
    }
    if(c){
        return GetChar(reader, node->left, fail);
    }
    else{
        return GetChar(reader, node->right, fail);
    }

}

/**Table Mutator and accessors**/
/**
 * searches the table for the given element
 * @param element - the item to be found in the table
 * @param table - the table to search
 * @return - the index if found, -1 if not found
 */
int HuffTree::FindTableIndex(char element, vector<HuffTree::Node *>& table) {
    for(size_t i = 0; i < table.size(); i++){
        if(table[i]->value == element) return i;
    }
    return -1;
}

/**
 * sorts the table
 * @param table - the table to be sorted
 */
void HuffTree::SortTable(vector<Node*>& table){
    size_t start = 0;
    while(start < table.size()){
        int index = start;
        for(size_t i = start + 1; i < table.size(); i++){
            if(table[i]->count < table[index]->count) index = i;
        }
        Node* temp = table[index];
        table.erase(table.begin() + index);
        table.insert(table.begin(), temp);
        start ++;
    }
}

/**
 * Inserts Element into the correct position in the table
 * @param element - the element to be inserted
 * @param table - the table to insert into
 */
void HuffTree::TableInsert(Node *element, vector<Node*>& table){
    if(table.empty()) {
        table.push_back(element);
        return;
    }
    for(size_t i = 0; i < table.size(); i++){
        if(element->count > table[i]->count){
            table.insert(table.begin() + i, element);
            return;
        }
    }
    table.push_back(element);
}


/**Public**/

/**Destructor
 *
*/
HuffTree::~HuffTree(){
    clear(_root);
}

/**
 * Creates the tree from a text file
 * @param src - the text file from which to build the tree
 * @param tree - a pointer that will be set to the new tree
 * @return succeed/fail
 */
bool HuffTree::CreateTreeFromUnencodedFile(ifstream &src, HuffTree*& tree) {
    vector<Node*> compTable;
    CreateFrequencyTable(src, compTable);

    size_t height = 0;
    while(compTable.size() != 1){
        Node* neo = new Node;
        neo->left = compTable.at(compTable.size() - 1);
        compTable.pop_back();
        neo->right = compTable.at(compTable.size() - 1);
        compTable.pop_back();

        neo->left->parent = neo;
        neo->right->parent = neo;

        neo->count = neo->left->count + neo->right->count;
        TableInsert(neo, compTable);
    }

    tree = new HuffTree(compTable.at(0), height);
    compTable.pop_back();

    return true;
}

/**
 * Creates the tree from an encoded file
 * @param src - the encoded file to create from
 * @param tree - a pointer that will be set to the new tree
 * @param reader - the EncReader that will be used to read the file
 * @return succeed/fail
 */
bool HuffTree::CreateTreeFromEncodedFile(ifstream &src, HuffTree *&tree, EncReader &reader) {
    KeyTable key = reader.GetKeyTable();
    tree = new HuffTree(key);
    return true;
}

bool HuffTree::DecodeFile(EncReader &reader, ostream &dest) {
    bool fail = false;
    while(!reader.eof()){
        char c = static_cast<char>(GetChar(reader, _root, fail));
        if(fail)
            break;
        dest.write(&c, 1);
    }
    return !fail;
}

/**
 * Prints the tree to an ostream (for testing purposes)
 * @param out - the ostream to print to
 */
void HuffTree::PrintToStream(ostream &out) {
    out << "Height: " << _height << '\n';
    PrintToStream(out, _root, 0);
}

/**
 * Gets the key
 * @return private pointer _key
 */
KeyTable HuffTree::GetKey() const {
    return _key;
}












