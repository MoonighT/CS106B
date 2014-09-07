/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */
 
#include <cctype>
#include <iostream>
using namespace std;

#include <iostream>
#include <string>
#include <ctype.h>
#include "lexicon.h"
#include "simpio.h"
#include "bogglegui.h"
#include "grid.h"
#include "set.h"
#include "random.h"

using namespace BoggleGUI;

const string DICTIONARY_FILE = "EnglishWords.dat";
const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

struct coor{
    coor(){
        x = -1;
        y = -1;
    }

    coor(int i, int j){
        x = i;
        y = j;
    }

    int x;
    int y;
};

bool operator < (const coor& lhs, const coor& rhs){
    if (lhs.x < rhs.x)
        return true;
    else if (lhs.x == rhs.x){
        return lhs.y < rhs.y;
    }else{
        return false;
    }
}

ostream & operator << (ostream &os, const coor& output){
    return os << "x => " << output.x << " y = > " << output.y << endl;
}

static void welcome();
static void giveInstructions();
static void cubeSetup();
static void shakeBoard(Grid<char> &board);
static void humanPlay(Grid<char> &board, Lexicon &dictionary);
static bool containsWord(string word, Grid<char> &board);
static void computerPlay(Grid<char> &board, Lexicon &dictionary);

int main() {
   Lexicon dictionary(DICTIONARY_FILE);
   welcome();
   if (getYesOrNo("Do you need instructions? ")) {
      giveInstructions();
   }

    //TODO: Place your code here.
   Grid<char> board;
   board.resize(4,4);
   cubeSetup();

   while(true){
        string conf = getLine("Do you need configure your board? y for yes, n for random");
        if(conf == "n"){
            shakeBoard(board);
        }else{
            cout << "provide configure file" << endl;
            exit(1);
        }
        // human turn
        humanPlay(board, dictionary);

        computerPlay(board, dictionary);
        string end = getLine("Do you want another one? ");
        if(end == "n")
            break;
    }
    shutdown(); // Closes Stanford graphics
    return 0;
}

static void findWords(int i, int j, string prefix, Grid<char> &board, Set<string> &result, Lexicon &english,Set<coor> &hist){
    if(!english.containsPrefix(prefix))
        return;
    if(english.contains(prefix) && prefix.size() >= 4){
        result.add(prefix);
    }
    for (int di=-1; di<=1; di++){
        for (int dj=-1; dj<=1; dj++){
            if((di||dj) && board.inBounds(i+di, j+dj)){
                hist.add(coor(i,j));
                findWords(i+di, j+dj, prefix+board.get(i,j),board, result,english, hist);
                hist.remove(coor(i,j));
            }
        }
    }
}

static void computerPlay(Grid<char> &board, Lexicon &dictionary){
    setStatusMessage("computer turn");
    Set<string> allWords;
    Set<coor> hist;
    for (int i=0; i<board.numRows(); ++i){
        for (int j=0; j<board.numCols(); ++j){
            hist.clear();
            findWords(i,j,"", board,allWords,dictionary,hist);
        }
    }
    cout << allWords << endl;
}

static bool findWord(int i, int j, string word, Grid<char> &board, Set<coor> &history){
    cout << "word is " << word << endl;
    if(word == "")
        return true;

    for (int di=-1; di<=1; di++){
        for (int dj=-1; dj<=1; dj++){
            if((di||dj) && board.inBounds(i+di, j+dj)){
                cout << coor(i+di, j+dj) << endl;
                if(history.contains(coor(i+di, j+dj))){
                    cout << "set contains" << coor(i+di, j+dj) << endl;
                    continue;
                }
                if(tolower(word[0]) == tolower(board.get(i+di, j+dj))){
                    history.add(coor(i+di, j+dj));
                    if(findWord(i+di, j+dj, word.substr(1), board, history)){
                        cout << coor(i+di, j+dj) << endl;
                        return true;
                    }else{
                        history.remove(coor(i+di, j+dj));
                    }
                }

            }
        }
    }

    return false;
}

static bool containsWord(string word, Grid<char> &board){
    Set<coor> hist;
    for (int i=0; i<board.numRows(); ++i){
        for (int j=0; j<board.numCols(); ++j){

            if(tolower(board.get(i,j)) == tolower(word[0])){
                //start recursive
                hist.clear();
                hist.add(coor(i, j));
                if (findWord(i,j,word.substr(1),board, hist)){
                    cout << coor(i, j) << endl;
                    return true;
                }
            }
        }
    }
    return false;
}

static void humanPlay(Grid<char> &board, Lexicon &dictionary){
    setStatusMessage("human turn");
    string humanWord = getLine("Enter a word: ");
    while(humanWord != ""){
        if (humanWord.size() < 4){
            cout << "too short" << endl;
        }else if(!dictionary.contains(humanWord)){
            cout << "no such word" << endl;
        }else{
            if (containsWord(humanWord, board)){
                cout << "find this word !" << endl;

            }else{
                cout << "cannot form this word" << endl;
            }
        }
        humanWord = getLine("Enter another word: ");
    }
}


static void shakeBoard(Grid<char> &board){
    Vector<string> cubes;
    for(int i=0; i<16; i++){
        cubes.add(kStandardCubes[i]);
    }
    //random shuffle the vector
    for(int i=0; i<cubes.size(); i++){
        int ranIndex = randomInteger(i, cubes.size()-1);
        string temp = cubes[i];
        cubes[i] = cubes[ranIndex];
        cubes[ranIndex] = temp;
    }

    for(int i=0; i<board.numRows(); i++){
        for(int j=0; j<board.numCols(); j++){
            string temp = cubes[i*4+j];
            int ranIndex = randomInteger(0,temp.size()-1);
            board.set(i,j,temp[ranIndex]);
            labelCube(i,j,board.get(i,j));
        }
    }
}

static void cubeSetup(){
    initialize(4,4);
    while (!isInitialized()){
        initialize(4,4);
        cout << "error " << endl;
    }
}

static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

static void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}
