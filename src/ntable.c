#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

/* returns a pointer to an empty nametable 
 */
NameTable*
newNameTable () {
	NameTable *nt = malloc (sizeof(NameTable));
	nt->maxsize = _TABLE_SIZE;
	nt->name = malloc (nt->maxsize*sizeof(Name));
	nt->size = 0;
	return nt;
}

/* returns the pointer to the resized nametable that is provided
 */
NameTable*
resizeNameTable (NameTable *nt) {
	nt->maxsize *= 2;
	nt->name = realloc (nt, nt->maxsize);
	if (!nt->name) {
		fprintf (stderr, "memory error in resizeNameTable(NameTable*)\n");
		exit(EXIT_FAILURE);
	}
	return nt;
}

/* properly clean up a name table
 */
void
freeNameTable (NameTable *nt) {
	size_t idx;
	for (idx = 0; idx < nt->size; idx++) {
		free (nt->name[idx].label);
	}
	free (nt->name);
	free (nt);
}

/* print nametable
 */
void 
printNameTable (NameTable *nt) {
	if (!nt) return;
	size_t idx;
	for (idx = 0; idx < nt->size; idx++) {
		if (nt->name[idx].is_covariable) {
			printf ("(v%s%s) ", nt->name[idx].label, nt->name[idx+1].label);
			idx++; // skip next name in table
		}
	}
}

/* returns the pointer to the possibly resized nametable after adding
 * the provided name to it. 
 * status tells the function parse that a name error has occurred
 */
NameTable*
addName (NameTable *nt, char *name, size_t *status) {
	size_t idx;
	for (idx = 0; idx < nt->size; idx++) {
		if (strcmp (nt->name[idx].label, name) == 0) {
			printf ("name error: %s already declared\n", name);
			*status = 0;
		}
	}		
	if (*status == 1) {
		idx = nt->size++;
		if (nt->size >= nt->maxsize) {
			nt = resizeNameTable (nt);
		}
		nt->name[idx].label = strdup (name);
	}
	return nt;
}

/* returns the pointer to the nametable augmented with a provided pair
 * of labels for names that form a covariable
 */
NameTable*
insertCovariable (NameTable *nt, char *x, char *y, size_t *status) {
	size_t idx = nt->size;
	nt = addName (nt, x, status);
	nt = addName (nt, y, status);
	if (*status != 0) {
		nt->name[idx].is_covariable = 1;
		nt->name[idx+1].is_covariable = 1;
	}
	return nt;
} 


