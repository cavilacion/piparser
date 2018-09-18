#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType 
{ 
	NAME,					// variable
	OP_SEQ, 			// .
	OP_PAR, 			// |
	RESTR,				// new
	L_PAR, 				// (
	R_PAR,  			// )
	L_BRACK,			// <
	R_BRACK, 			// >
	STOP,					// 0
	END_OF_INPUT, // <<EOF>>
	INVALID_TOKEN // syntax error
} TokenType;



typedef struct Token {
	TokenType type;
	char *label;
	struct Token *next;
	struct Token *prev; 
	
	size_t pos; 				// used for tracking syntax errors
} Token;

Token *newEmptyToken ();
Token *handleToken (Token *prevToken, TokenType tt, size_t *len);
void printTokenStream (Token *tok);
char *indicateError (Token *tok);
void cleanTokenStream (Token *tok);

Token *scan (Token **invalid);

#endif


