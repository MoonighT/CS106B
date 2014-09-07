// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include "console.h"
#include "simpio.h"
#include "map.h"
#include "stack.h"
#include "queue.h"
#include "set.h"
#include "vector.h"
using namespace std;

void readDictionary(Map<string, int> &dict);
void findWordLadder(string source, string destination, Map<string, int> &dict, Vector<string> &result);
void neighbourWords(Map<string, int> &dict, Vector<string> &result, string sour);
int main() {
    // TODO: Finish the program!

    // Read dictionary
    Map<string, int> dict;
    readDictionary(dict);
    // Input check invalid word, same length different words.
    string source;
    string destination;
    while (true){
        source= getLine("Enter the source word: ");
        destination = getLine("Enter the destination word: ");
        transform(source.begin(), source.end(), source.begin(), ::tolower);
        transform(destination.begin(), destination.end(), destination.begin(), ::tolower);
        if(source != destination){
            if(dict.containsKey(source) && dict.containsKey(destination)){
                break;
            }else{
                cout << "words are not in dictionary" << endl;
            }
        }else{
             cout << "words are the same" << endl;
        }
    }
    Vector<string> result;
    findWordLadder(source, destination, dict, result);
    cout << result << endl;
    cout << "Have a nice day." << endl;
    return 0;
}

void readDictionary(Map<string, int> &dict){
    ifstream infile;
    infile.open("dictionary.txt");
    if(infile.fail()){
        cerr << "error occur" << endl;
    }else{
        string line;
        while (getline(infile, line)){
            dict[line] = 1;
        }
    }
}

void findWordLadder(string source, string destination, Map<string, int> &dict, Vector<string> &result){
    Queue<Vector<string>> myqueue;
    Vector<string> mystack;
    Set<string> myset;
    myset.add(source);
    mystack.add(source);
    myqueue.enqueue(mystack);
    while (!myqueue.isEmpty()){
        Vector<string> top = myqueue.dequeue();
        string word = top[top.size()-1];
        Vector<string> neighbour;
        neighbourWords(dict, neighbour, word);
        for (int i = 0; i < neighbour.size(); i++){
            string target = neighbour[i];
            if (!myset.contains(target)){
                if(target == destination){
                    top.add(target);
                    result = top;
                    return;
                }
                else{
                    Vector<string> copy = top;
                    copy.add(target);
                    myset.add(target);
                    myqueue.enqueue(copy);
                }
            }
        }

    }

}

void neighbourWords(Map<string, int> &dict, Vector<string> &result, string sour){
    string target = sour;
    for(int i = 0; i < sour.length(); ++i){
        for(int j = 0; j < 26; ++j){
            sour[i] = 'a' + j;
            if((sour != target) && dict.containsKey(sour)){
                result.add(sour);
            }
        }
        sour[i] = target[i];
    }
}
