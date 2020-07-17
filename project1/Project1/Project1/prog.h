/*
Name: Michael Davenport
Date due: 02/04/20
Dr. Jhijiang Dong CSCI4160
*/
#include "slp.h"

//Example program 1
// a:=5+3; b := (print(a, a-1), 10*a); print(b);
CompoundStm	prog1 =
CompoundStm(new AssignStm("a", new OpExp(new NumExp(5), OpExp::PLUS,
	new NumExp(3))),
	new CompoundStm(new AssignStm("b",
		new EseqExp(new PrintStm(new PairExpList(new IdExp("a"),
			new LastExpList(new OpExp(new IdExp("a"), OpExp::MINUS,
				new NumExp(1))))),
			new OpExp(new NumExp(10), OpExp::TIMES, new IdExp("a")))),
		new PrintStm(new LastExpList(new IdExp("b")))));
// Produces the following output:
// 8 7
// 80

//Example program 2
// print(10, (print(2*5, -4), 6))
PrintStm prog2 =
PrintStm(new PairExpList(new NumExp(10),
	new LastExpList(new EseqExp(new PrintStm(new PairExpList(
		new OpExp(new NumExp(2), OpExp::TIMES, new NumExp(5)),
		new LastExpList(new NumExp(-4)))), new NumExp(6)))));

// Produces the following output:
// 10 10 -4
// 6


//Example program 3
//   a:=30; b:=a/5; print(b)
// 
CompoundStm prog3 =
CompoundStm(new AssignStm("a", new NumExp(30)),
	new CompoundStm(
		new AssignStm("b", new OpExp(new IdExp("a"),
			OpExp::DIV,
			new NumExp(5))),
		new PrintStm(
			new LastExpList(new IdExp("b")))));

// Produces the following output:
// 6

