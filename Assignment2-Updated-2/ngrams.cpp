// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "queue.h"
#include "hashmap.h"
#include "random.h"
using namespace std;

int main() {
    // TODO: Finish the program!
    // Read file name
    ifstream infile;
    promptUserForFile(infile, "Input file? ", "Can`t find this file, reiput?");
    // Read N  ( N > 1)
    int N;
    while(true){
        N = getInteger("Input N? ");
        if( N > 1)
            break;
        else
            cout << "N should be > 1" << endl;
    }

    // Build the word map
    // Declare a map, queue for window as key in map
    Queue<string> firstWindow;
    Queue<string> window;
    HashMap<Queue<string>, Vector<string>> nGrams;

    // Read first N - 1 words in window
    string word;
    for(int i = 0; i < N-1; ++i){
        infile >> word;
        window.enqueue(word);
    }
    firstWindow = window;

    while(infile >> word){
        if (nGrams.containsKey(window)){
            nGrams[window].add(word);
        }else{
            Vector<string> wordList;
            wordList.add(word);
            nGrams[window] = wordList;
        }
        window.dequeue();
        window.enqueue(word);
    }
    for (int i = 0; i < N-1; ++i){
        string word = firstWindow.dequeue();
        if (nGrams.containsKey(window)){
            nGrams[window].add(word);
        }else{
            Vector<string> wordList;
            wordList.add(word);
            nGrams[window] = wordList;
        }
        window.dequeue();
        window.enqueue(word);
    }

    // Read word num  (num >= N - 1)
    int numOfWords;
    while(true){
        numOfWords = getInteger("# of random words to generate (0 to quit)?");
        if (numOfWords >= N-1){
            // generate random text
            cout << "generate random text" << endl;
            // Choose start point
            Vector<Queue<string>> keys = nGrams.keys();
            Vector<string> list;
            int random_index = randomInteger(0, keys.size()-1);
            window = keys[random_index];
            for(int i=0; i<numOfWords-1; i++){
                // choose next word
                list = nGrams[window];
                random_index = randomInteger(0, list.size()-1);
                word = list[random_index];
                window.enqueue(word);
                cout << window.dequeue() << " ";
            }
        }else if (numOfWords == 0)
            break;
        else
            cout << "Invalid # of words" << endl;
    }

    cout << "Exiting." << endl;
    return 0;
}
