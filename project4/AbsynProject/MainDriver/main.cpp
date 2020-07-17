#include <iostream>
#include <fstream>
#include <string>
#include <FlexLexer.h>
#include "ErrorMsg.h"
#include "Absyn.h"
#include "Print.h"

using namespace std;

extern int yyparse(void);
extern ErrorMsg	errormsg;
extern absyn::Exp*	root;

yyFlexLexer		lexer;

void parse(string fname) 
{
	ifstream	ifs(fname);
	errormsg.reset(fname);
	lexer.switch_streams(&ifs, NULL);

	if ( yyparse() == 0 ) /* parsing worked */
		cout << "Parsing successful!\n" << endl;
	else
		cout << "Parsing failed\n" << endl;
}

extern int yydebug;


void f(int i) { cout << "integer" << endl; }
void f(bool i) { cout << "bool" << endl; }

int main(int argc, char **argv) 
{
	f(true);

	yydebug = 0;
	if (argc!=2) 
	{
		cerr <<"usage: " << argv[0] << " filename" << endl; 
		return 1;
	}
	parse( argv[1] );

	absyn::Print	print(cout);
	print.prExp(root, 0);
	cout << endl << endl;
	return 0;
}