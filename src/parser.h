#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define _TABLE_SIZE 256 

#define covar(i) (1-(i%2))+(i/2)*2 

typedef struct Name {
	char *label;
	size_t is_covariable;
} Name;

typedef struct NameTable {
	size_t size;
	size_t maxsize;
	Name *name;
} NameTable;

typedef enum ActionType { INPUT, OUTPUT, TERMINATION } ActionType;

typedef struct ActionData {
	size_t x_idx; // idx for variables in
	size_t y_idx; // 		x<y> or x(y)
} ActionData;

typedef struct Action* ActionPtr;
typedef struct Action {
	ActionType type;
	ActionData data;
	ActionPtr next;
} Action;
	
typedef struct Thread {
	NameTable *nt;
	Action *stream;
} Thread;

typedef struct ProcessTable {
	size_t size;
	size_t maxsize;
	Thread *thread;
} ProcessTable;


/* ntable.c */
NameTable* newNameTable ();
NameTable* resizeNameTable (NameTable *nt);
NameTable* addName (NameTable *nt, char *name, size_t *status);
NameTable* insertCovariable (NameTable *nt, char *x, char *y, size_t *status);
void freeNameTable (NameTable *nt);
void printNameTable (NameTable *nt);

/* ptable.c */
ProcessTable* newProcessTable ();
ProcessTable* resizeProcessTable (ProcessTable *pt);
ProcessTable* addThread (ProcessTable *pt, Thread P);
void freeProcessTable (ProcessTable *pt);

/* parser.c */
size_t expectRestriction (Token *stream);
Token* parseRestrictions (Token *stream, NameTable **nt, size_t *status);
ProcessTable* parseThreads (Token *stream, NameTable **nt, size_t *status);
void parse (Token *stream);


#endif
