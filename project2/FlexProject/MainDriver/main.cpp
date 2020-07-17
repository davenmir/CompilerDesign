/*
PROGRAMMER: Michael Davenport
Program #: Program2
Due Date: 2/17/20
Instructor: Dr. Zhijiang Dong

Description: Lexical Analyzer Rules
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "ErrorMsg.h"
#include "tokens.h"
#include <FlexLexer.h>

using namespace std;

string toknames[] = {
"ID", "STRING", "INT", "COMMA", "COLON", "SEMICOLON", "LPAREN",
"RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS",
"MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT", "GE",
"AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE", "FOR",
"TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
"VAR", "TYPE"
};


string tokname(int tok) {
  return tok<257 || tok>299 ? "BAD_TOKEN" : toknames[tok-257];
}

yyFlexLexer			lexer;
YYSTYPE				yylval;

int main(int argc, char **argv) {
	ifstream			ifs; 
	extern int			beginLine, beginCol;	//beginning position of string or comment
	extern int			linenum, colnum;			//beginning position of all other tokens
	extern ErrorMsg		errormsg;
	int					tok;
	
	if (argc!=2) 
	{
		cerr << "usage: " << argv[0] << " filename" << endl;
		return 1;	
	}
	ifs.open( argv[1] );
	if( !ifs ) 
	{
		cerr << "Input file cannot be opened.\n";
        return 0;
	}
	cout << "Lexcial Analysis of the file " << argv[1] << endl;
	errormsg.reset( argv[1] );
	lexer.switch_streams(&ifs, NULL);
	for(;;) {
		tok = lexer.yylex();
		if (tok==0) break;
		switch(tok) {
		case ID: 
			cout << setw(10) << tokname(tok) << " " << setw(2) << linenum << ":" 
				 << setw(3) << colnum << " " << *(yylval.sval) << endl;
			break;
		case STRING:
			cout << setw(10) << tokname(tok) << " " << setw(2) << beginLine << ":" 
				 << setw(3) << beginCol << " " << *(yylval.sval) << endl;
			break;
		case INT:
			cout << setw(10) << tokname(tok) << " " << setw(2) << linenum << ":" 
				 << setw(3) << colnum << " " << yylval.ival << endl;
			break;
		default:
			cout << setw(10) << tokname(tok) << " " << setw(2) << linenum << ":" 
				 << setw(3) << colnum << endl;
		}
	}
	return 0;
}
