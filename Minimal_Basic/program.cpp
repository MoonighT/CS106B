/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code
    elements.clear();
    statements.clear();
}

Program::~Program() {
   // Replace this stub with your own code
    elements.clear();
    statements.clear();
}

void Program::clear() {
   // Replace this stub with your own code
    elements.clear();
    statements.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
   // Replace this stub with your own code
    elements[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
    if(elements.find(lineNumber) != elements.end())
        elements.erase(lineNumber);
    return;
}

string Program::getSourceLine(int lineNumber) {
    if(elements.find(lineNumber) != elements.end())
        return elements[lineNumber];
    else
        return "";    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
    statements[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(statements.find(lineNumber) != statements.end())
        return statements[lineNumber];
    else
        return NULL;  // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
    if(elements.size() <= 0)
        return -1;
    else{
        return elements.begin()->first;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = elements.find(lineNumber);
    if(it != elements.end()) {
        it++;
        if(it == elements.end())
            return -1;
        else
            return it->first;
    }else
        return -1;     // Replace this stub with your own code
}
