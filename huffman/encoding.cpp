/**
 * File: encoding.cpp
 * ------------------
 * Place your Encoding class implementation here.
 */

#include "encoding.h"
#include "simpio.h"
#include "string"
#include "map.h"
#include "vector.h"
using namespace std;

void constructTable(Map<int, string> &table, Node *tar, string pattern)
{
    if(tar == NULL)
        return;
    if(tar->value>0){
        //leaf
        table[tar->value] = pattern;
        return;
    }
    else{
        constructTable(table, tar->left, pattern+"0");
        constructTable(table, tar->right, pattern+"1");
    }
}

void printNode(Node *n){
    if(n->left != NULL)
        printNode(n->left);
    cout << "value " << n->value << " pri " << n->prior << endl;
    if(n->right != NULL)
        printNode(n->right);
}

void Encoding::compress(ibstream &infile, obstream &outfile){
    // read the character in pq
    // build the table  "a" => '0010'
    // write line the header
    // based on table write bit in file
    string filename;
    while(true){
        getLine("enter the file to compressed", filename);
        filename.insert(0,"test/encodeDecode/");
        cout << filename << endl;
        infile.open(filename.c_str());
        if(!infile.fail())
            break;
        else
            cout << "open file error, re-enter file name" << endl;
    }
    int temp;
    Map<int, int> mymap;
    temp = infile.get();
    while(temp != EOF){
        if (mymap.containsKey(temp))
            mymap[temp] += 1;
        else
            mymap[temp] = 1;
        temp = infile.get();
    }
    infile.close();
    cout << mymap << endl;
    for(int i=0; i<mymap.keys().size(); i++){
        int key = mymap.keys().get(i);
        Node *n = new Node(key, mymap[key]);
        pq.enqueue(n, mymap[key]);
    }

    //for eof 128
    Node *spe = new Node(256, 1);
    pq.enqueue(spe, 1);

    //debug
    //while(pq.size() > 0){
    //    Node *x = pq.extractMin();
    //    cout << "prior " << x->prior << "value" << x->value << endl;
    //}



    Node *one;
    Node *two;
    Node *three;
    while(true){
        if(pq.size() <= 1)
            break;
        one = pq.extractMin();
        two =  pq.extractMin();
        three = new Node();
        three->value = -1;
        three->left = one;
        three->right = two;
        three->prior = one->prior + two->prior;
        pq.enqueue(three, three->prior);
    }
    Node *last = pq.extractMin();
    //cout << last->prior << " value " << last->value << endl;
    //printNode(last);


    Map<int, string> table;
    constructTable(table, last, "");

    //correct
    cout << table << endl;
    string outname = filename + ".zip";
    outfile.open(outname.c_str());
    if(outfile.fail()){
        cout << "out file error" << endl;
        return;
    }
    for(int i=0; i<table.keys().size(); i++){
        int k = table.keys().get(i);
        outfile << k << " = " << table[k] << endl;
    }
    outfile << 256 << " = " << table[256] << endl;


    infile.open(filename.c_str());
    infile.rewind();
    int temp2;
    temp2 = infile.get();
    while(temp2 != EOF){
        string bits = table[temp2];
        for(int i=0; i<bits.size(); i++){
            outfile.writebit('1' - bits[i]);
        }
        cout << bits << endl;
        temp2 = infile.get();
    }
    for(int i=0; i<table[256].size(); i++){
        outfile.writebit('1' - table[256][i]);
    }
    outfile.close();
    cout << table[256] << endl;
}

void Encoding::decompress(ibstream &infile, obstream &outfile){
    // read the header
    // build the map that "0010" => "a"
    // read the compressed file read bit until match a pattern, write the char

}
