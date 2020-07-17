/*
PROGRAMMER: Michael Davenport
Program #: Program2
Due Date: 2/17/20
Instructor: Dr. Zhijiang Dong

Description: Lexical Analyzer Rules
*/


%option noyywrap
%option c++

%{
#include <iostream>
#include <string>
#include <sstream>
#include "tokens.h"
#include "ErrorMsg.h"

using std::string;
using std::stringstream;

ErrorMsg			errormsg;	//error handler

int		c_depth = 0;	// depth of the nested comment
string	value = "";			// the value of current string
//holds message for error code

int			beginLine=-1;	//beginning line no of a string or comment
int			beginCol=-1;	//beginning column no of a string or comment

int		linenum = 1;		//beginning line no of the current matched token
int		colnum = 1;			//beginning column no of the current matched token
int		tokenCol = 1;		//column no after the current matched token

//positions of comment blocks
int x=0;
int y=0;


//the following defines actions that will be taken automatically after 
//each token match. It is used to update colnum and tokenCol automatically.
#define YY_USER_ACTION {colnum = tokenCol; tokenCol=colnum+yyleng;}

int string2int(string);			//convert a string to integer value
void newline(void);				//trace the line #
void error(int, int, string);	//output the error message referring to the current token
string illegalTokenCode;
%}

ALPHA		[A-Za-z]
DIGIT		[0-9]
INT			[0-9]+
IDENTIFIER	{ALPHA}(({ALPHA}|{DIGIT}|"_")*)


/*exclusive start condition for COMMENT_COND and STRING_COND*/
%x COMMENT_COND
%x STRING_COND

/*
First there are a list of special characters and keywords
Followed by what are to be recognized as Comments
And finally what are to be recognized as Strings
*/
%%
","				{ return COMMA; }
":"				{ return COLON; }
";"				{ return SEMICOLON; }
"("				{ return LPAREN; }
")"				{ return RPAREN; }
"["				{ return LBRACK; }
"]"				{ return RBRACK; }
"{"				{ return LBRACE; }
"}"				{ return RBRACE; }
"."				{ return DOT; }
"+"				{ return PLUS; }
"-"				{ return MINUS; }
"*"				{ return TIMES; }
"/"				{ return DIVIDE; }
"="				{ return EQ; }
"<>"			{ return NEQ; }
"<"				{ return LT; }
">"				{ return GT; }
"<="			{ return LE; }
">="			{ return GE; }
"&"				{ return AND; }
"|"				{ return OR; }
":="			{ return ASSIGN; }
"while"			{ return WHILE; }
"for"			{ return FOR; }
"to"			{ return TO; }
"break"			{ return BREAK; }
"in"			{ return IN; }
"end"			{ return END; }
"function"		{ return FUNCTION; }
"var"			{ return VAR; }
"type"			{ return TYPE; }
"array"			{ return ARRAY; }
"if"			{ return IF; }
"then"			{ return THEN; }
"else"			{ return ELSE; }
"do"			{ return DO; }
"of"			{ return OF; }
"nil"			{ return NIL; }
"let"			{ return LET; }

"/*"			{	/* entering comment */
					c_depth ++;
					x = linenum;
					y = colnum;
					BEGIN(COMMENT_COND);	
}

<COMMENT_COND>"/*"	{	/* nested comment */
					c_depth ++;
}

<COMMENT_COND>[^*/\n]*	{	/*eat anything that's not a '*' */ }

<COMMENT_COND>"/"+[^/*\n]*  {	/*eat anything that's not a '*' */ }

<COMMENT_COND>"*"+[^*/\n]*	{	/* eat up '*'s not followed by '/'s */	}

<COMMENT_COND>\n		{	/* trace line # and reset column related variable */
					newline();
}

<COMMENT_COND>"*"+"/"	{	/* close of a comment */
						c_depth --;
						if ( c_depth == 0 )
						{
							BEGIN(INITIAL);	
						}
					}
<COMMENT_COND><<EOF>>	{	/* unclosed comments */
						error(x, y, "unclosed comments");
						yyterminate();
}


"\""			{   /*Begin a string thread*/
					value = "";
					beginLine = linenum;
					beginCol = colnum;
					BEGIN(STRING_COND);
}

<STRING_COND>"\""		{   /*Exit a string*/
					yylval.sval = new string(value);
					BEGIN(INITIAL);
					return STRING;
}

<STRING_COND><<EOF>>	{   /*finding an unclosed string */
					error(linenum, colnum, "unclosed string");
					yyterminate();
}

<STRING_COND>[^"\n\t\\"]*	{ /*Appending strings that do not contain newline and tab*/  value += YYText();  }

<STRING_COND>\n			{ /*ignore newline and append, then reset the string*/
					error(beginLine, beginCol, "unclosed string");
					yylval.sval = new string(value);
					newline();
					BEGIN(INITIAL);
					return STRING;
				}
<STRING_COND>\\n {  /*appending a newline to a string*/ value += "\n";}
<STRING_COND>\\t { /*appending a tab to a string*/ value += "\t";}
<STRING_COND>\\\\ {/*appending a double slash to a string*/ value +="\\";}
<STRING_COND>\\\" {/*appending a double quote into a string*/ value += "\"";}
<STRING_COND>\\[^"nt\\] {/*finding an illegal token*/
					illegalTokenCode = YYText();
					error(linenum, colnum, illegalTokenCode+" illegal token");
				}

" "				{}
\t				{}
\b				{}
\n				{newline(); }

{IDENTIFIER} 	{ value = YYText(); yylval.sval = new string(value); return ID; }
{INT}		 	{ yylval.ival = string2int(YYText()); return INT; }

<<EOF>>			{	yyterminate(); }
.				{	error(linenum, colnum, string(YYText()) + " illegal token");}

%%

int string2int( string val )
{
	stringstream	ss(val);
	int				retval;

	ss >> retval;

	return retval;
}

void newline()
{
	linenum ++;
	colnum = 1;
	tokenCol = 1;
}

void error(int line, int col, string msg)
{
	errormsg.error(line, col, msg);
}
