#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

extern char *yytext;

Token*
newEmptyToken () {
	Token *new = malloc (sizeof(Token));
	new->next = NULL;
	new->prev = NULL;
	return new;
}

void 
showToken (Token *tok) {
	switch(tok->type) {
		case STOP: printf ("0"); break;
		case OP_SEQ: printf ("."); break;
		case OP_PAR: printf ("|"); break;
		case RESTR: printf ("new "); break;
		case NAME: printf ("%s", tok->label); break;
		case L_PAR: printf ("("); break;
		case R_PAR: printf (")"); break;
		case L_BRACK: printf ("<"); break;
		case R_BRACK: printf (">"); break;
		case INVALID_TOKEN: printf ("%s", tok->label); break;
		default: 
		  printf ("error: could not interpret symbol: %s\n", tok->label);
			break;
	}
}

Token*
handleToken (Token *prevToken, TokenType tt, size_t *len) {
	Token *new = newEmptyToken();
	new->type = tt;
	new->label = strdup (yytext);
	new->pos = (*len);
	(*len) += strlen (new->label);
	new->prev = prevToken;
	if (prevToken) {
		prevToken->next = new;
	}
	return new;
}

void 
printTokenStream (Token *tok) {
	if (tok==NULL) {
		printf ("\n");
		return;
	}
	showToken (tok);
	printTokenStream (tok->next);
}

void 
cleanTokenStream (Token *tok) {
	if (tok==NULL) return;
	Token *next = tok->next;
	free (tok->label);
	free (tok);
	cleanTokenStream(next);
}
		 
		 
