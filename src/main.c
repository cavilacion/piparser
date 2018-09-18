#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

extern int *yytext;

char*
indicateError (Token *tok) {
	char *indicator = malloc (tok->pos + strlen(tok->label) + 10);
	size_t idx;
	for (idx = 0; idx < tok->pos; ++idx) {
		indicator[idx] = ' ';
	}
	for ( ; idx < tok->pos + strlen (tok->label); ++idx) {
		indicator[idx] = '^';
	}
	indicator[idx]='\0';
	return indicator;
}

void
printSyntax (Token *invalid) {
	printf ("syntax error at symbol: '%s'\n", invalid->label);
}

void 
printUsage (char *prgName) {
	printf ("Usage: %s <file> - where file contains a process\n", prgName);
	return;
}

Token*
scan (Token **invalid) {
	/* handleToken needs a pointer to the position of tokens */
	size_t *pos = malloc (sizeof(size_t));
	(*pos)=0;
	
	/* start reading tokens */
	Token *tp=NULL;
	TokenType tt = yylex();
	while (tt!=END_OF_INPUT) {
		tp = handleToken (tp, tt, pos);
		tt = yylex();
	} 
	free (pos);
	
	/* handle empty input */
	if (tp==NULL) {
		printf ("file descriptor provided no input\n");
		return EXIT_SUCCESS;
	}
	
	/* go back to first token */
	Token *first=tp;
	*invalid=NULL;
	while (first->prev!=NULL) {
		first = first->prev;
		if (first->type == INVALID_TOKEN) {
			*invalid=first;
		}
	}
	
	/* if invalid token was scanned (*invalid) now points to the first 
	 * one */
	return first;
}

int 
main(int argc, char *argv[]) {
	/* program needs exactly one command line parameter */
	if (argc!=2) {
		printUsage(argv[0]);
		return EXIT_SUCCESS;
	}
	
	/* redirect stdin */
	stdin = fopen (argv[1], "r");
	if (!stdin) {
		printf ("could not open file descriptor for path: %s\n", argv[1]);
		return EXIT_SUCCESS;
	}
	
	Token **invalid = malloc (sizeof (Token*));
	Token *stream = scan (invalid);
	
	printTokenStream (stream);
	
	if (*invalid) {
		char *error_indication = indicateError (*invalid);
		fprintf (stdout, "%s\n", error_indication);
		free (error_indication);
		printSyntax(*invalid); 
	}
	
	else {
		parse (stream);
	}
	
	free (invalid);
	cleanTokenStream (stream);
	fclose (stdin);
	return 0;
}
