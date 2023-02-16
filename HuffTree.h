/*
 Filename: HuffTree.h
 Description: Declaration of the class HuffTree which represents a binary Huffman Tree for Encoding/decoding
 Author: Noah Gallo
 Date: 05/31/2020
 Course: Data Structures II
*/

#ifndef HUFFMAN_QUILLO123_HUFFTREE_H
#define HUFFMAN_QUILLO123_HUFFTREE_H

#include <fstream>
#include <vector>
#include "EncReader.h"
#include "KeyTable.h"

using std::ifstream;
using std::vector;
using std::istream;
using std::ostream;

class HuffTree {
    struct Node {
        Node* parent = nullptr;
        Node* right = nullptr;
        Node* left = nullptr;
        u_char value = 0;
        size_t count = 0;
    };

    Node* _root;
    KeyTable _key;
    size_t _height;

    HuffTree(Node *root, size_t height);
    HuffTree(KeyTable key);

    void clear(Node* root);

    void CreateTreeFromKey(const u_char &val, uint32_t code, uint8_t numBits, Node *root);

    static void CreateFrequencyTable(ifstream &src, vector<Node*>& compTable);
    void CreateKey();
    void KeyTraverse(Node* root, size_t numBits, uint32_t code);

    void PrintToStream(ostream& out, Node* root, int num);

    u_char GetChar(EncReader &reader, HuffTree::Node *node, bool &fail);

    //Table Manipulation
    static int FindTableIndex(char element, vector<HuffTree::Node*>& table);
    static void SortTable(vector<Node *> &table);
    static void TableInsert(Node *element, vector<Node*>& table);
public:

    ~HuffTree();
    static bool CreateTreeFromUnencodedFile(ifstream &src, HuffTree*& tree);
    static bool CreateTreeFromEncodedFile(ifstream &src, HuffTree *&tree, EncReader &reader);

    bool DecodeFile(EncReader& reader, ostream& dest);

    void PrintToStream(ostream& out);
    KeyTable GetKey() const;

};


#endif //HUFFMAN_QUILLO123_HUFFTREE_H
