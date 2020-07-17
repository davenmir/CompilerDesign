/*
Name: Michael Davenport
Date due: 02/04/20
Dr. Jhijiang Dong CSCI4160
*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

typedef unordered_map<string, int>	SymbolTable;

//abstract base class
class Stm
{
public:
	//interprets the statement using the given symboltable
	virtual void interp(SymbolTable&) = 0;

	//Calculate the maximum number of arguments of any print statement 
	//within any subexpression of a given statement. 
	//virtual int maxargs( void ) = 0;
};

//abstract base class
class Exp
{
public:
	//interprets the expression using the given symboltable
	virtual int interp(SymbolTable&) = 0;

	//Calculate the maximum number of arguments of any print statement 
	//within any subexpression of a given expression. 
	//virtual int maxargs( void ) = 0;
};

//abstract base class
class ExpList
{
public:
	//interprets the expression using the given symboltable
	virtual void interp(SymbolTable&) = 0;

	//Calculate the maximum number of arguments of any print statement 
	//within any subexpression of a given expression. 
	//virtual int maxargs( void ) = 0;

	//returns the number of top level sub-expressions in the list
	//virtual int size() const = 0;
};

class CompoundStm : public Stm
{
private:
	Stm		*stm1, *stm2;
public:
	CompoundStm(Stm *s1, Stm *s2)
		: stm1(s1), stm2(s2)
	{/*empty body*/
	}

	~CompoundStm()
	{
		delete stm1; delete stm2;
	}


	/* To be implemented by stduents */
	void interp(SymbolTable&);
};

class AssignStm : public Stm
{
private:
	string		id;
	Exp			*exp;
public:
	AssignStm(string i, Exp *e)
		: id(i), exp(e)
	{/*empty body*/
	}

	~AssignStm() { delete exp; }

	/* To be implemented by stduents */
	void interp(SymbolTable&);
};

class PrintStm : public Stm
{
private:
	ExpList		*exps;
public:
	PrintStm(ExpList *e)
		: exps(e)
	{/*empty body*/
	}

	~PrintStm() { delete exps; }

	/* To be implemented by stduents */
	void interp(SymbolTable&);
};


class IdExp : public Exp
{
private:
	string	id;
public:
	IdExp(string i)
		:id(i)
	{ /*empty body*/
	}

	~IdExp() {}

	/* To be implemented by stduents */
	int interp(SymbolTable&);
};

class NumExp : public Exp
{
private:
	int		num;
public:
	NumExp(int n)
		: num(n)
	{ /*empty body*/
	}

	~NumExp() {}

	/* To be implemented by stduents */
	int interp(SymbolTable&);
};

class OpExp : public Exp
{
public:
	enum OperType { PLUS, MINUS, TIMES, DIV };
private:
	Exp			*left, *right;
	OperType	oper;
public:
	OpExp(Exp *l, OperType o, Exp *r)
		: left(l), oper(o), right(r)
	{ /*empty body*/
	}

	~OpExp() { delete left; delete right; }

	/* To be implemented by stduents */
	int interp(SymbolTable&);
};

class EseqExp : public Exp
{
private:
	Stm		*stm;
	Exp		*exp;
public:
	EseqExp(Stm *s, Exp *e)
		: stm(s), exp(e)
	{ /*empty body*/
	}

	~EseqExp() { delete stm; delete exp; }

	/* To be implemented by stduents */
	int interp(SymbolTable&);
};


class PairExpList : public ExpList
{
private:
	Exp		*head;
	ExpList *tail;
public:
	PairExpList(Exp *h, ExpList *t)
		: head(h), tail(t)
	{ /* empty body */
	}

	~PairExpList() { delete head; delete tail; };
	const Exp* getHead(void) const { return head; }
	const ExpList* getTail(void) const { return tail; }

	/* To be implemented by stduents */
	void interp(SymbolTable&);
	//int size() const;
};

class LastExpList : public ExpList
{
private:
	Exp		*head;
public:
	LastExpList(Exp *h)
		: head(h)
	{ /*empty body */
	}

	~LastExpList() { delete head; };
	const Exp* getHead(void) const { return head; }

	/* To be implemented by stduents */
	void interp(SymbolTable&);
	//int size() const; 
};
