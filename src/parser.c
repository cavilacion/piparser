#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

/* returns whether the stream is currently looking at a restriction
 * for example: (new x y)
 */
size_t 
expectRestriction (Token *stream) {
	if (!stream) return 0;
	if (stream->type != L_PAR) return 0;
	stream = stream->next;
	if (!stream) return 0;
	if (stream->type != RESTR) return 0;
	stream = stream->next;
	if (!stream) return 0;
	if (stream->type != NAME) return 0;
	stream = stream->next;
	if (!stream) return 0;
	if (stream->type != NAME) return 0;
	stream = stream->next;
	if (!stream) return 0;
	if (stream->type != R_PAR) return 0;
	return 1;
}

/* returns the tokenpointer where the supposed restriction-free process 
 * begins
 * the provided indirect pointer to a nametable will be filled with the
 * covariables
 */ 
Token*
parseRestrictions (Token *stream, NameTable **nt, size_t *status) {
	size_t count=0;
	*status=1;
	while (expectRestriction (stream)) {
		stream = stream->next->next; // skip '(' and 'new'
		char *name1 = stream->label;
		char *name2 = stream->next->label;
		*nt = insertCovariable (*nt, name1, name2, status);
		count++;
		stream = stream->next->next->next;
	}
	return stream;
}

ProcessTable*
parseThreads (Token *stream, NameTable **nt, size_t *status) {
	ProcessTable* pt = newProcessTable();
	return pt;
}

void 
parsingDone (NameTable **nt, ProcessTable *pt) {
	freeNameTable (*nt);
	free (nt);
	if (pt) freeProcessTable (pt);
}

void
parse (Token *stream) {
	/* parse restrictions */
	NameTable** nt = malloc (sizeof(NameTable*));
	*nt = newNameTable ();
	size_t status=0;
	stream = parseRestrictions (stream, nt, &status);
	
	/* show results */
	printNameTable (*nt);
	
	/* check status */
	if (!status) {
		parsingDone (nt, NULL);
		return;
	}
	status=0;
	
	/* parse threads, so "write" as example P = P0 . (P1 | P2) | P3 */
	ProcessTable* pt = parseThreads (stream, nt, &status);
	
	/* cleanup */
	parsingDone (nt, pt);
	return;
}
