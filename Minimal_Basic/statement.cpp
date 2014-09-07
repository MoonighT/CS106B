/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

//print
PrintStmt::PrintStmt(TokenScanner &scanner)
{
    exp = readE(scanner, 0);
    if(scanner.hasMoreTokens()){
        error("Extraneous token " + scanner.nextToken());
    }
}

PrintStmt::~PrintStmt()
{
    delete exp;
}

void PrintStmt::execute(EvaluationContext &context)
{
    cout << exp->eval(context) << endl;
}


//rem
RemStmt::RemStmt(TokenScanner &scanner) {
    remark_ = "";
    scanner.scanStrings();
    while(scanner.hasMoreTokens())
        remark_ += scanner.nextToken() + " ";
}

RemStmt::~RemStmt() {
}

void RemStmt::execute(EvaluationContext & context) {
    cout << remark_ << endl;
}

//let
LetStmt::LetStmt(TokenScanner &scanner) {
    scanner.scanStrings();
    Expression *expr = readE(scanner,0);
    cout << expr->toString();
    if (expr->getOperator() == "=")
    {
        variable = expr->getLHS()->toString();
        exp = expr->getRHS();
    }else{
        error("not a valid let statement");
    }
}

LetStmt::~LetStmt() {
    delete exp;
}

void LetStmt::execute(EvaluationContext &context) {
    context.setValue(variable, exp->eval(context));
}

//Goto
GotoStmt::GotoStmt(TokenScanner &scanner) {
    scanner.scanNumbers();
    Expression *expr = readE(scanner,0);
    lineN_ = expr->getConstantValue();
}

GotoStmt::~GotoStmt() {
}

void GotoStmt::execute(EvaluationContext &context) {
    context.setCurrentLine(lineN_);
}


//If
IfStmt::IfStmt(TokenScanner &scanner) {
    scanner.scanStrings();
    exp1 = readE(scanner, 1);
    op = scanner.nextToken();
    exp2 = readE(scanner, 1);
    string then = scanner.nextToken();
    lineN_ = readE(scanner, 0)->getConstantValue();
}

IfStmt::~IfStmt() {
    delete exp1;
    delete exp2;
}

void IfStmt::execute(EvaluationContext &context) {
    bool flag = false;
    if(op == ">") {
        if(exp1->eval(context) > exp2->eval(context))
            flag = true;
    }
    if(op == "<") {
        if(exp1->eval(context) < exp2->eval(context))
            flag = true;
    }
    if(op == "=") {
        if(exp1->eval(context) == exp2->eval(context))
            flag = true;
    }
    if (flag == true)
        context.setCurrentLine(lineN_);
}
