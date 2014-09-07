/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <ctime>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "filelib.h"
#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

// Prototypes
static void welcome();
static void startListeningForClick();
static bool hasClickOccurred();
static void readGrid(Grid<int> &grid);
static void updateScreen(Grid<int> &curGene, LifeDisplay &display);
static void readSpeedOption(int &choice);
static void simulation(int option, Grid<int> &curGene, Grid<int> &nextGene, LifeDisplay &display);
static void nextGeneration(Grid<int> &curGene, Grid<int> &nextGene, LifeDisplay &display);
int main() {
    LifeDisplay display;
    Grid<int> curGene;
    Grid<int> nextGene;
    int option = -1;
    display.setTitle("Game of Life");
    int flag = -1;
    welcome();
    //Note: the console window will not close on its own even after the program exits.
    // You'll need to manually close it.

    //Enter the simulation file and read file display the grid
    do{
        readGrid(curGene);
        display.setDimensions(curGene.numRows(), curGene.numCols());
        nextGene.resize(curGene.numRows(), curGene.numCols());
        updateScreen(curGene, display);

        //Enter the simulation speed opition and start simulate
        readSpeedOption(option);
        while(option > 4 || option < 1){
            readSpeedOption(option);
        }

        simulation(option, curGene, nextGene, display);

        //listen to click or stable to stop and ask open another or exit

        flag = getInteger("Would you like to run another one 1 for yes 0 for no");
    }while(flag == 1);

    return 0;
}

static void readSpeedOption(int &choice){
    cout << "choose the speed of simulation" << endl;
    cout << "1 for as fast as possible" << endl;
    cout << "2 for fast " << endl;
    cout << "3 for slow to see clearly" << endl;
    cout << "4 for click next manually" << endl;
    choice = getInteger("Your choice: ");
}

static void nextGeneration(Grid<int> &curGene, Grid<int> &nextGene, LifeDisplay &display){
    for (int i = 0; i < curGene.numRows(); i++){
        for (int j = 0; j < curGene.numCols(); j++){
            int num = 0;
            for (int di = -1; di < 2; di++){
                for (int dj = -1; dj < 2; dj++){
                    if(curGene.inBounds(i + di, j + dj) && curGene[i + di][j + dj] > 0){
                        if (!(di==0 && dj == 0))
                            ++num;
                    }
                }
            }
            if (num == 0 || num == 1 || num > 3){
                nextGene[i][j] = 0;
            }else if(num == 2){
                nextGene[i][j] = curGene[i][j] > 0 ? curGene[i][j] + 1 : 0;
            }else if(num == 3){
                nextGene[i][j] = curGene[i][j] + 1;
            }else{
                cout << "error";
            }
            display.drawCellAt(i,j,nextGene[i][j]);
        }
    }
}

static void simulation(int option, Grid<int> &curGene, Grid<int> &nextGene, LifeDisplay &display){
    switch (option){
        case 1:
            startListeningForClick();
            while(true){
                if(hasClickOccurred()){
                    break;
                }
                nextGeneration(curGene, nextGene, display);
                curGene = nextGene;
                //updateScreen(curGene, display);
            }
            cout << 1 << endl;
        break;

        case 2:
            startListeningForClick();
            while(true){
                if(hasClickOccurred()){
                    break;
                }

                nextGeneration(curGene, nextGene, display);
                curGene = nextGene;
                //updateScreen(curGene, display);
                pause(1000);
            }
            cout << 2 << endl;
        break;

        case 3:
            startListeningForClick();
            while(true){
                if(hasClickOccurred()){
                    break;
                }
                nextGeneration(curGene, nextGene, display);
                curGene = nextGene;
                //updateScreen(curGene, display);
                pause(3*1000);
            }
            cout << 3 << endl;
        break;

        case 4:
             while(true){
                 if(hasClickOccurred()){
                     nextGeneration(curGene, nextGene, display);
                     curGene = nextGene;
                     //updateScreen(curGene, display);
                 }
             }
        break;

        default:
        return;
    }

}


static void updateScreen(Grid<int> &curGene, LifeDisplay &display){
    for (int i = 0; i < curGene.numRows(); i++){
        for (int j = 0; j < curGene.numCols(); j++){
            display.drawCellAt(i,j,curGene[i][j]);
        }
    }
}

static void readGrid(Grid<int> &grid){
    ifstream stream;
    string line;
    int row, col = -1;
    promptUserForFile(stream, "Input the simulation file: ");
    while(true){
        getline(stream, line);
        if (stream.fail()) break;
        if (line[0] == '#')
            continue;
        else{
            std::istringstream iss(line);
            iss >> row;
            break;
        }
    }
    stream >> col;
    getline(stream, line);
    grid.resize(row, col);
    for (int i = 0; i < row; i++){
        getline(stream, line);
        for (int j = 0; j < col; j++)
        {
            grid[i][j] = line[j] == 'X' ? 1 : 0;
        }
    }
}

/* Print out greeting for beginning of program. */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

/* Clears previously recorded mouse events so that the program will
 * not react to a mouse click from before this method is called. */
static void startListeningForClick()
{
    while(true){
        GMouseEvent me = getNextEvent(MOUSE_EVENT);
        if(me.getEventType() == NULL_EVENT)
            return;
    }
}

/* Checks to see if a click has occurred since the last time
 * "startListeningForClick" was called. */
static bool hasClickOccurred()
{
    while(true){
        GMouseEvent me = getNextEvent(MOUSE_EVENT);
        if (me.getEventType() == MOUSE_CLICKED)
            return true;
        else if (me.getEventType() == NULL_EVENT)
            return false;
    }
}
