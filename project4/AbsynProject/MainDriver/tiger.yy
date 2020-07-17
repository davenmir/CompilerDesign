/*
PROGRAMMER: Michael Davenport
Program #: Program4
Due Date: 3/31/20
Instructor: Dr. Zhijiang Dong

Description: Abstract Syntax Tree construction
*/
%debug
%verbose
%locations
%code requires {
#include <iostream>
#include "ErrorMsg.h"
#include <FlexLexer.h>
#include "Absyn.h"

int yylex(void); /* function prototype */
void yyerror(char *s);	//called by the parser whenever an eror occurs
void yyerror(int, int, char *s);	//called by the parser whenever an eror occurs

}

%union {
	int			ival;	//integer value of INT token
	string*		sval;	//pointer to name of IDENTIFIER or value of STRING	
					//I have to use pointers since C++ does not support 
					//string object as the union member
	absyn::Exp*				exp;
	absyn::Var*				var;
	absyn::Ty*				ty;
	absyn::Dec*				dec;
	absyn::DecList*			declist;
	absyn::ExpList*			explist;
}

%{
void yyerror(YYLTYPE loc, char *s);
absyn::Exp*		root;	//pointer to the root of abstract syntax tree
%}

%token <sval> ID STRING
%token <ival> INT

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

%type <var>				lvalue
%type <exp>				exp expseq program
%type <dec>				dec vardec typedec
%type <ty>				tpinfo
%type <declist>			decs t_decs
%type <explist>			explist t_explist paramlist t_paramlist


%nonassoc ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%start program

%%

/* This is a skeleton grammar file, meant to illustrate what kind of
 * declarations are necessary above the %% mark.  Students are expected
 *  to replace the two dummy productions below with an actual grammar. 
 */

program	:	exp	
			{ $$ = $1; root=$$; }
		|
			{ $$ = NULL; root =$$; }
		;

 /*decs: a sequence of declarations, may be an empty sequence */
decs	:	
			{ $$ = NULL; }
		|	t_decs	
			{ $$ = $1; }
		;

 /*t_decs: a non-empty sequence of declarations */
t_decs	:	dec t_decs	
			{	$$ = new absyn::DecList($1, $2);	}
		|	dec			
			{ $$ = new absyn::DecList($1, NULL); }
		|	t_decs error
		;
 /*dec: one type or variable declaration */
dec		:	typedec
			{ $$ = $1; }
		|	vardec
			{ $$ = $1; }
		;

 /*typedec: one type declaration */
typedec	:	TYPE ID EQ tpinfo	
			{	
				$$ = new absyn::TypeDec(@1.first_line, @1.first_column, *$2, $4, NULL);
			}
		| TYPE error EQ tpinfo
		;

 /*tpinfo: type information of a type declaration */
tpinfo	:	ID					
			{	$$ = new absyn::NameTy(@1.first_line, @1.first_column, *$1);	}
		|	ARRAY OF ID			
			{	$$ = new absyn::ArrayTy(@1.first_line, @1.first_column, *$3);
			}
		;

 /*vardec: a variable declaration */
vardec	:	VAR ID ASSIGN exp
			{	
				$$ = new absyn::VarDec(@1.first_line, @1.first_column, *$2, NULL, $4); 
			}
		|	VAR ID COLON ID ASSIGN exp
			{	
				$$ = new absyn::VarDec(@1.first_line, @1.first_column, *$2, new absyn::NameTy(@4.first_line, @4.first_column, *$4), $6); 
			}
		;

 /*lvalue: an variable or an array element*/
lvalue	:	ID
			{ $$ = new absyn::SimpleVar(@1.first_line, @1.first_column, *$1); }				
		/*|	lvalue LBRACK exp RBRACK   //readd later
		|	ID LBRACK exp RBRACK*/
		;

 /*exp: one expresion */
exp	:		lvalue
			{
				$$ = new absyn::VarExp( @1.first_line, @1.first_column, $1);
			}				
		|	exp PLUS exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::PLUS, $3 );	}
		|	exp MINUS exp
			{ $$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::MINUS, $3 );	}
		|	exp TIMES exp
			{ $$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::MUL, $3 );	}
		|	exp DIVIDE exp
			{ $$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::DIV, $3 );	}
		|	MINUS exp	
			{	$$= new absyn::OpExp(	@1.first_line, @1.first_column,
										new absyn::IntExp(@1.first_line, @1.first_column, 0), 
										absyn::OpExp::MINUS,
										$2 );	
			}	%prec UMINUS
		|	expseq
			{ 	$$ = $1;	}
		|	INT
			{ 	$$ = new absyn::IntExp( @1.first_line, @1.first_column, $1);	}
		|	STRING
			{ $$ = new absyn::StringExp( @1.first_line, @1.first_column, *$1);	}
		|	ID LPAREN paramlist RPAREN		/*function call expression*/
			{ 	$$ = new absyn::CallExp( @1.first_line, @1.first_column, *$1, $3 );	}
		|	exp EQ exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::EQ, $3 );	}
		|	exp NEQ exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::NE, $3 );	}
		|	exp LT exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::LT, $3 );	}
		|	exp LE exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::LE, $3	);	}
		|	exp GT exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::GT, $3 );	}
		|	exp GE exp
			{	$$ = new absyn::OpExp( @1.first_line, @1.first_column, $1, absyn::OpExp::GE, $3 );	}
		|	exp AND exp
			{	$$ = new absyn::IfExp( @1.first_line, @1.first_column, $1, $3, new absyn::IntExp(@1.first_line, @1.first_column, 0));	}
		|	exp OR exp
			{   $$ = new absyn::IfExp( @1.first_line, @1.first_column, $1, new absyn::IntExp(@1.first_line, @1.first_column, 1), $3);	}
		|	ID LBRACK exp RBRACK OF exp	  /*array expression*/
			{	$$ = new absyn::ArrayExp( @1.first_line, @1.first_column, *$1, $3, $6); }
		|	lvalue ASSIGN exp			/* assignment expression */
			{   $$ = new absyn::AssignExp( @1.first_line, @1.first_column, $1, $3);	}	
		|	IF exp THEN exp ELSE exp
			{ 	$$ = new absyn::IfExp( @1.first_line, @1.first_column, $2, $4, $6); }
		|	IF exp THEN exp
			{ 	$$ = new absyn::IfExp( @1.first_line, @1.first_column, $2, $4); }
		|	WHILE exp DO exp
			{ 	$$ = new absyn::WhileExp( @1.first_line, @1.first_column, $2, $4); }			
		|	FOR	ID ASSIGN exp TO exp DO exp	
			{ 	$$ = new absyn::ForExp( @1.first_line, @1.first_column, new absyn::VarDec(@2.first_line, @2.first_column, *$2, NULL, $4), $6, $8); }
		|	BREAK
			{ 	$$ = new absyn::BreakExp(@1.first_line, @1.first_column); }
		|	LET decs IN explist END
			{   $$ = new absyn::LetExp(@1.first_line, @1.first_column, $2, new absyn::SeqExp(@4.first_line, @4.first_column, $4)); }
		|	NIL
			{ 	$$ = new absyn::NilExp(@1.first_line, @1.first_column); }
		;
 /*expseq: a sequence of expressions enclosed by a pair of parenthesis */
expseq		:	LPAREN explist RPAREN
				{	$$ = new absyn::SeqExp(@1.first_line, @1.first_column, $2); }
			;

 /*explist: a sequence of expressions, separated by semicolon */
explist		:	
			|	t_explist
				{	$$ = $1; }
			;

 /*t_explist: a non-empty sequence of expressions, separated by semicolon */
t_explist	:	exp
				{	$$ = new absyn::ExpList($1, NULL); }
			|	exp SEMICOLON t_explist
				{	$$ = new absyn::ExpList($1, $3); }
			;
 /*paramlist: a sequence of expressions, separated by comma*/
paramlist	: 
			|	t_paramlist
				{	$$ = $1; }
			;
 /*t_paramlist: a non-empty sequence of expressions, separated by comma*/
t_paramlist	:	exp
				{	$$ = new absyn::ExpList($1, NULL); }
			|	exp COMMA t_paramlist
				{	$$ = new absyn::ExpList($1, $3); }
			;

%%
extern yyFlexLexer	lexer;
int yylex(void)
{
	return lexer.yylex();
}

void yyerror(char *s)
{
	extern int	linenum;			//line no of current matched token
	extern int	colnum;
	extern void error(int, int, std::string);

	error(linenum, colnum, s);
}

void yyerror(int lineno, int column, char *s)
{
	extern void error(int, int, std::string);

	error(lineno, column, s);
}

void yyerror(YYLTYPE loc, char *s)
{
	extern void error(int, int, std::string);

	error(loc.first_line, loc.first_column, s);
}