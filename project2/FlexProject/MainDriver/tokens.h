/*
PROGRAMMER: Michael Davenport
Program #: Program2
Due Date: 2/17/20
Instructor: Dr. Zhijiang Dong

Description: Lexical Analyzer Rules
*/
#pragma once

//YYSTYPE defines the structure to hold values
//assoicated with tokens. This structure will 
//be defined in the parser //in the future
typedef union  {
	int		ival;	//integer value of INT token
	string* sval;	//pointer to name of IDENTIFIER or value of STRING	
					//I have to use pointers since C++ does not support 
					//string object as the union member
} YYSTYPE;

extern YYSTYPE yylval;	//yylval will be defined in the driver.

# define ID 257
# define STRING 258
# define INT 259
# define COMMA 260
# define COLON 261
# define SEMICOLON 262
# define LPAREN 263
# define RPAREN 264
# define LBRACK 265
# define RBRACK 266
# define LBRACE 267
# define RBRACE 268
# define DOT 269
# define PLUS 270
# define MINUS 271
# define TIMES 272
# define DIVIDE 273
# define EQ 274
# define NEQ 275
# define LT 276
# define LE 277
# define GT 278
# define GE 279
# define AND 280
# define OR 281
# define ASSIGN 282
# define ARRAY 283
# define IF 284
# define THEN 285
# define ELSE 286
# define WHILE 287
# define FOR 288
# define TO 289
# define DO 290
# define LET 291
# define IN 292
# define END 293
# define OF 294
# define BREAK 295
# define NIL 296
# define FUNCTION 297
# define VAR 298
# define TYPE 299

