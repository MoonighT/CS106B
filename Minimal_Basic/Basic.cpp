/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for Assignment #6 (BASIC).
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include "console.h"
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvaluationContext & context);

/* Main program */

int main() {
   EvaluationContext context;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, context);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, context);
 * -------------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */



void processLine(string line, Program & program, EvaluationContext & context) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.setInput(line);

   //add/delete into program
   if(line.empty())
       return;
   if(line == "List") {
       //show the list of program
       cout << "show list" << endl;
       int n = program.getFirstLineNumber();
       while(n>0){
           cout << program.getSourceLine(n) << endl;
           n = program.getNextLineNumber(n);
       }
   }else if(line == "Clear") {
       //clear the program
       cout << "clear program" << endl;
       program.clear();
   }else if(line == "Run") {
       //start run the program
       int nLine = program.getFirstLineNumber();
       while( nLine > 0)
       {
           Statement *s = program.getParsedStatement(nLine);
           context.setCurrentLine(nLine);
           s->execute(context);
           if(context.getCurrentLine() != nLine)
               nLine = context.getCurrentLine();
           else
               nLine = program.getNextLineNumber(nLine);
       }

   }else{
       scanner.scanNumbers();
       string lineNumber = scanner.nextToken();
       if(lineNumber != "") {
           int number = atoi(lineNumber.c_str());
           if(number > 0) {
                scanner.scanStrings();
                if(scanner.hasMoreTokens())
                {
                    //add into program
                    cout << "add into program" << endl;
                    program.addSourceLine(number, line);
                    Statement *parsed = parseStatement(scanner);
                    program.setParsedStatement(number, parsed);
                }else{
                    //remove from the program
                    cout << "remove from program" << endl;
                    program.removeSourceLine(number);
                }
           }else{
               cout << "invalid number" << endl;
           }
       }else {
           cout << "invalid command" << endl;
       }
   }
}
