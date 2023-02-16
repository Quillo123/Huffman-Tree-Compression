/*
 Filename: main.cpp
 Description: contains the main function for the Huffman Tree Encoder
 Author: Noah Gallo
 Date: 05/30/2020
 Course: Data Structures II
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <bitset>

#include "HuffTree.h"
#include "KeyTable.h"
#include "EncReader.h"
#include "EncWriter.h"

using namespace  std;

bool EncodeFile(ifstream &src, ofstream &dest);
bool DecodeFile(ifstream& src, ofstream &dest);

int main(int argc, char *argv[]){
    if(argc != 4){
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    ifstream src;
    src.open(argv[2], ios::out | ios::binary);
    if(!src.is_open()){
        cerr << "Cannot open source file \"" << argv[2] << "\"\n";
        return false;
    }

    ofstream dest;
    if(strcmp(argv[1], "-decode") == 0)
        dest.open(argv[3]);
    else
        dest.open(argv[3], ios::out | ios::binary);
    if(!dest.is_open()){
        cerr << "Cannot open destination file \"" << argv[3] << "\"\n";
        return 1;
    }

    if(strcmp(argv[1], "-encode") == 0) {
        if (!EncodeFile(src, dest)) {
            return 1;
        }
    }
    else if(strcmp(argv[1], "-decode") == 0) {
        if (!DecodeFile(src, dest)) {
            return 1;
        }
    }
    else{
        cerr << "Invalid Argument: " << argv[1] << endl;
        return 1;
    }

    dest.close();
    src.close();

    return 0;
}

/**
 * Encodes a text file using a huffman tree
 * @param src - text file to be encoded
 * @param dest - file to output encoded data to
 * @return if the encoding succeeded
 */
bool EncodeFile(ifstream &src, ofstream &dest) {
    HuffTree* hTree = nullptr;
    if(!HuffTree::CreateTreeFromUnencodedFile(src, hTree)){
        return false;
    }

    hTree->GetKey().Print(cout);

    src.clear();
    src.seekg(0, ios_base::beg);
    EncWriter writer;

    if(!writer.WriteToFile(src, dest, *hTree)){
        return false;
    }

    delete hTree;
    return true;
}

/**
 * decodes an encoded file using a huffman tree
 * @param src - encoded file to be decoded
 * @param dest - file to output decoded text to
 * @return if the decoding succeeded
 */
bool DecodeFile(ifstream& src, ofstream &dest) {

    EncReader reader;
    if(!reader.LoadByteStream(src))
        return false;

    HuffTree* hTree = nullptr;
    if(!HuffTree::CreateTreeFromEncodedFile(src, hTree, reader)){
        return false;
    }

    if(!hTree->DecodeFile(reader, dest)){
        delete hTree;
        return false;
    }



    delete hTree;
    return true;

}
