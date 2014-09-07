/**
 * File: huffman.cpp 
 * -------------------
 * Huffman main module.  Build it!
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include "bstream.h"
#include "encoding.h"
#include "string"
using namespace std;

int main() {
    string command;
    ibstream infile;
    obstream outfile;
    Encoding huffman;
    while(true){
        infile.clear();
        outfile.clear();
        getLine("compress(c) or decompress(d)", command);
        if(command == "c"){
            huffman.compress(infile, outfile);
        }else if(command == "d"){
            huffman.decompress(infile, outfile);
        }else if(command == "exit")
            break;
        else{
            cout << "invalid command" << endl;
        }
    }
    return 0;
}
