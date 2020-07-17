/*
PROGRAMMER: Michael Davenport
Program #: Program3
Due Date: 3/9/20
Instructor: Dr. Zhijiang Dong

Description: Generate CFG
*/



%debug
%verbose	/*generate file: tiger.output to check grammar*/
%{
#include <iostream>
#include "ErrorMsg.h"
#include <FlexLexer.h>

int yylex(void);		/* function prototype */
void yyerror(char *s);	//called by the parser whenever an eror occurs

%}

%union {
	int		ival;	//integer value of INT token
	string* sval;	//pointer to name of IDENTIFIER or value of STRING	
					//I have to use pointers since C++ does not support 
					//string object as the union member
}

/* TOKENs and their associated data type */
%token <sval> ID STRING
%token <ival> INT

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

/* add your own predence level of operators here */ 

%nonassoc ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc UMINUS

%start program

%%

/* This is a skeleton grammar file, meant to illustrate what kind of
 * declarations are necessary above the %% mark.  Students are expected
 *  to replace the two dummy productions below with an actual grammar. 
 */

program	:	expr
		;

expr	:	STRING
		|	INT
		|	NIL
		|	MINUS expr %prec UMINUS
		|	expr PLUS expr
		|	expr MINUS expr
		|	expr TIMES expr
		|	expr DIVIDE expr
		|	expr EQ expr
		|	expr NEQ expr
		|	expr GT expr
		|	expr LT expr
		|	expr LE expr
		|	expr GE expr
		|	expr AND expr
		|	expr OR expr
		|	lValue
		|	lValue ASSIGN expr
		|	LPAREN exprSeq RPAREN
		|	BREAK
		|	FOR ID ASSIGN expr TO expr DO expr
		|	WHILE expr DO expr
		|	IF expr THEN expr
		|	IF expr THEN expr ELSE expr
		|	ID LPAREN exprList RPAREN
		|	ID LPAREN RPAREN
		|	ID array_expr OF expr
		|	LET declarationList IN exprSeq END
		|	LET declarationList IN END
		|	ID LBRACE fieldList RBRACE
		|	ID LBRACE RBRACE
		|	error
		;

array_expr	:	LBRACK expr RBRACK
			|	array_expr LBRACK expr RBRACK
			;
			//expression sequence
exprSeq		:	expr
			|	exprSeq SEMICOLON expr
			//|	error SEMICOLON expr
			;

exprList	:	expr
			|	exprList COMMA expr
			;

fieldList	:	ID EQ expr
			|	fieldList COMMA ID EQ expr
			;

lValue		:	ID
			|	lValue DOT ID
			|	ID array_expr
			|	lValue DOT ID array_expr
			//|	ID LBRACK error RBRACK
			//|	lValue LBRACK error RBRACK
			;
				//single declaration
declaration		:	typeDeclaration
				|	variableDeclaration
				|	functionDeclaration
				;

					//multiple declarations
declarationList		:	declaration
					|	declarationList declaration
					//|	declarationList error
					;

					//declaration of type
typeDeclaration		:	TYPE ID EQ type
					//|	TYPE error EQ type
					;

		//casting a type
type	:	ID
		|	LBRACE typeFields RBRACE
		|	LBRACE RBRACE
		|	ARRAY OF ID
		;

			
typeField	:	ID COLON ID
			//|	ID error
			;


typeFields	:	typeField
			|	typeFields COMMA typeField
			;

					//variable declarations
variableDeclaration	:	VAR ID ASSIGN expr
					|	VAR ID COLON ID ASSIGN expr
					;

					//function declarations
functionDeclaration	:	FUNCTION ID LPAREN typeFields RPAREN COLON ID EQ expr
					|	FUNCTION ID LPAREN RPAREN COLON ID EQ expr
					|	FUNCTION ID LPAREN typeFields RPAREN EQ expr
					|	FUNCTION ID LPAREN RPAREN EQ expr
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

