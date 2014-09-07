/**
 * File: mnemonics.cpp
 * ---------------------------
 * This program takes in a string of digits and generates all possible
 * corresponding mnemonics.
 */

using namespace std;

#include "console.h"
#include "simpio.h"
#include "vector.h"

static void generateMnemonics(string digits, Vector<string>& mnemonics);
static void generateMnemonics(string prefix, string digits, Vector<string>& mnemonics);
static string getLettersForDigit(int digit);
static void printMnemonics(int mnemonicLength, Vector<string>& mnemonics);

const int CONSOLE_WIDTH = 50;

int main() {
    while(true){
        string digits = getLine("Enter digits [press enter to quit]: ");
        int mnemonicLength = digits.size();
        if (mnemonicLength == 0){
            break;
        }
        Vector<string> mnemonics;
        generateMnemonics(digits, mnemonics);
        printMnemonics(mnemonicLength, mnemonics);
    }
    cout << "Have a nice day!" << endl;
    return 0;
}

/* Add a comment for this method */
static void generateMnemonics(string digits, Vector<string>& mnemonics){
    // TODO: Write your implementation here. Remember that it must be recursive.
    generateMnemonics("", digits, mnemonics);
}

static void generateMnemonics(string prefix, string digits, Vector<string> &mnemonics){
    if (digits.size() == 0){
        mnemonics.add(prefix);
        return;
    }
    char temp = digits[0];
    digits = digits.substr(1);
    cout << "digits = " << digits.size() << endl;
    string letters = getLettersForDigit(temp-'0');
    for(int i=0; i<letters.size(); i++){
        generateMnemonics(prefix + letters[i], digits, mnemonics);
    }
}

/* Return the letters associated with the given digit. */
static string getLettersForDigit(int digit){
    switch(digit){
    case 2:
        return "ABC";
    case 3:
        return "DEF";
    case 4:
        return "GHI";
    case 5:
        return "JKL";
    case 6:
        return "MNO";
    case 7:
        return "PQRS";
    case 8:
        return "TUV";
    case 9:
        return "WXYZ";
    default:
        return "";
    }
}

/* Prints the mnemonics in the vector to the console in a nice format. */
static void printMnemonics(int mnemonicLength, Vector<string> &mnemonics){
    int perRow = CONSOLE_WIDTH / (mnemonicLength+1);
    for(int i = 0; i < mnemonics.size(); i++){
        if(i % perRow == 0) cout << endl;
        cout << mnemonics[i] << " ";
    }
    cout << endl;
}


