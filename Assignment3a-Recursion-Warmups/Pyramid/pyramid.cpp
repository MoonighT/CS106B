/**
 * File: pyramid.cpp
 * ---------------------------
 * This program calculates the weight carried by each person
 * in a human pyramid.
 */

using namespace std;

#include <math.h>
#include <iomanip>
#include "console.h"
#include "vector.h"
#include "random.h"
#include "simpio.h"

struct Person{
    double weight;
    double weightOnKnee;
    Person(){
        weight = -1;
        weightOnKnee = -1;
    }

    Person(double _weight){
        weight = _weight;
        weightOnKnee = -1;
    }
};

static double roundValue(double value);
static double weightOnKnees(int row, int col, Vector<Vector<Person>>& weights);
static void buildWeights(int numCol, Vector<Vector<Person>> &weights);
static void printWeights(Vector<Vector<Person>> &weights);
static void printWeightsOnKnees(Vector<Vector<Person>> &weights);
const int MIN_WEIGHT = 50;
const int MAX_WEIGHT = 150;

int main() {
    cout << fixed << setprecision(2); // DO NOT REMOVE. Sets cout to print doubles to 2 decimal places
    while (true) {
        int numCols = getInteger("How many people are on the bottom row? ");

        //TODO: add code here to populate random weights, print the original weights, and print the weight on knees pyramid
        Vector<Vector<Person>> weights(numCols);

        buildWeights(numCols, weights);
        printWeights(weights);
        printWeightsOnKnees(weights);
        string response = getLine("Make another pyramid? [y/n] ");
        if (response != "y")
            break;
        cout << endl;
    }

    return 0;
}


static void buildWeights(int numCol, Vector<Vector<Person> > &weights){
    for(int i=0; i<weights.size(); i++){
        for(int j=0; j<=i; j++){
            weights[i].add(Person(roundValue(randomReal(50,150))));
        }
    }
}

static void printWeights(Vector<Vector<Person> > &weights){
    for(int i=0; i<weights.size(); i++){
        for(int j=0; j<weights[i].size(); j++){
            if(j!=0)
                cout << " ";
            cout << weights[i][j].weight;
        }
        cout << endl;
    }
}

static void printWeightsOnKnees(Vector<Vector<Person> > &weights){
    for(int i=0; i<weights.size(); i++){
        for(int j=0; j<weights[i].size(); j++){
            if(j!=0)
                cout << " ";
            cout << weightOnKnees(i, j, weights);
        }
        cout << endl;
    }

}

/* Rounds the given value to 2 decimal places. */
static double roundValue(double value){
    return floor(value*100 + 0.5)/100.0;
}

/* TODO: Add a comment for this method explaining what it does. */
static double weightOnKnees(int row, int col, Vector<Vector<Person>>& weights) {
    // TODO: Write your implementation here. Remember that it must be recursive.
    if (row < 0 || col < 0 || col > row)
        return 0;
    else{
        if(weights[row][col].weightOnKnee > 0)
            return weights[row][col].weightOnKnee;
    else
        return weights[row][col].weightOnKnee = weights[row][col].weight + 0.5 * weightOnKnees(row - 1, col - 1, weights) + 0.5 * weightOnKnees(row - 1, col, weights);
    }
}
