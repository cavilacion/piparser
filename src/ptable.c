#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "parser.h"

/* returns a pointer to an empty process table
 */
ProcessTable*
newProcessTable () {
	ProcessTable* pt = malloc (sizeof(ProcessTable));
	pt->maxsize = _TABLE_SIZE;
	pt->thread = malloc (sizeof(Thread)*pt->maxsize);
	pt->size = 0;
	return pt;
}

/* returns the pointer to the resized process table
 */
ProcessTable*
resizeProcessTable (ProcessTable* pt) {
	pt->maxsize *= 2;
	pt->thread = realloc (pt, pt->maxsize);
	if (!pt->thread) {
		fprintf (stderr, "memory error in resizeProcessTable(Processtable*)\n");
		exit(EXIT_FAILURE);
	}
	return pt;
}

void cleanActionStream (Action *a) {
	if (!a) return;
	Action *next = a->next;
	free (a);
	cleanActionStream (next);
}

void cleanThread (Thread P) {
	freeNameTable (P.nt);
	cleanActionStream (P.stream);
}

/* properly clean the processtable 
 */
void
freeProcessTable (ProcessTable *pt) {
	size_t idx;
	for (idx = 0; idx < pt->size; idx++) {
		cleanThread (pt->thread[idx]);
	}
	free (pt->thread);
	free (pt);
}

ProcessTable* addThread (ProcessTable *pt, Thread P) { return pt; }
