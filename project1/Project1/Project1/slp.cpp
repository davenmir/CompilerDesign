/*
Name: Michael Davenport
Date due: 02/04/20
Dr. Jhijiang Dong CSCI4160
*/
#include <algorithm>
#include "slp.h"


using namespace std;

/* To be implemented by stduents */
void CompoundStm::interp(SymbolTable& symbols)
{
	//interprets stm1 and stm2
	stm1->interp(symbols);
	stm2->interp(symbols);
}

/* To be implemented by stduents */
void AssignStm::interp(SymbolTable& symbols)

{
	//assign value by id in the map
	symbols[id] = exp->interp(symbols);
}

void PrintStm::interp(SymbolTable& symbols)
{
	//interpret expressions
	exps->interp(symbols);
	cout << '\n';
}

int IdExp::interp(SymbolTable& symbols)
{
	//returns the symbol based on ID
	return (symbols[id]);
}

int NumExp::interp(SymbolTable& symbols)
{
	//returns the number stored
	return num;
}


int OpExp::interp(SymbolTable& symbols)
{
	//check which operator is being used
	if (oper == PLUS) {
		//add left operand to right operand for addition
		return (left->interp(symbols) + right->interp(symbols));
	}
	else if (oper == MINUS) {
		//subtract right operand from left operand
		return (left->interp(symbols) - right->interp(symbols));
	}
	else if (oper == TIMES) {
		//multiply the right operand by the left operand
		return (left->interp(symbols) * right->interp(symbols));
	}
	else if (oper == DIV) {
		//divide the left operand by the right operand
		return (left->interp(symbols) / right->interp(symbols));
	}
	else {
		//handle any unknown operands
		cout << "Error, operator not recognized." << endl;
		return -1;
	}
}



int EseqExp::interp(SymbolTable& symbols)
{
	//interpret stm and return the interpretation of the exp symbol
	stm->interp(symbols);
	return exp->interp(symbols);
}


void PairExpList::interp(SymbolTable& symbols)
{
	//print out the head expression with a divider for clear printing then interpret the tail symbol
	cout << head->interp(symbols) << " ";
	tail->interp(symbols);
}

void LastExpList::interp(SymbolTable& symbols)

{
	//print out the head of the exp list after interpretation
	cout << head->interp(symbols);
}

