/**
 * File: encoding.h
 * ----------------
 * Defines the Encoding class to manage all aspects of the
 * Huffman compression and decompression algorithms.
 */

#ifndef _encoding_
#define _encoding_

#include "bstream.h"
#include "pqueue.h"
#include "map.h"
#include "string"

struct Node {
    Node(){}
    ~Node(){
        left = NULL;
        right = NULL;
    }
    Node(int _value, int _pri){
        value = _value;
        left = NULL;
        right = NULL;
        prior = _pri;
    }

    int value;
    int prior;
    Node *left;
    Node *right;
};

class Encoding {
public:
    Encoding(){}
    ~Encoding(){}

    void compress(ibstream& infile, obstream& outfile);
    void decompress(ibstream& infile, obstream& outfile);

private:
    PQueue<Node*> pq;
    Map<int, string> table;
};

#endif
