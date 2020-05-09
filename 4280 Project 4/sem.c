#include <stdio.h>
#include "parser.h"
#include <stdbool.h>
#include <string.h>
#define maxJanikow 100

/* Global */
char trapperKeeper[maxJanikow][maxJanikow];
bool oneTime = true;

bool verify(char* string) {
	int i;
	for (i = 0; i < maxJanikow; i++) {
		/* does given string match anything in our array */
		if (strcmp(trapperKeeper[i], string) == 0) {
			/* We found a duplicate */
			return true;
		}
	}
	return false;
}

void insert(char* string) {
	int i;
	for (i = 0; i < maxJanikow; i++) {
		/* find first "empty" spot */
		if (strcmp(trapperKeeper[i], "...")) {
			/* Insert ID into empty spot */
			strcpy(trapperKeeper[i], string);
			break;
		}
	}
}

void init() {
	int i;
	for (i = 0; i < maxJanikow; i++) {
		/* initialize all slots with ellipses */
		trapperKeeper[i] == "...";
	}
}
void semantics(node* noduh) {
	/* call function to initialize trapperKeeper array */
	if (oneTime == true) {
		init();
		oneTime = false;
	}
	/* escape if we're in an empty node */
	node* node = noduh;
	if (node == NULL)
		return;

	/* See if you are in an "identifier node" (verify/insert) */
	if (strcmp(node->label, "<vars>") == 0) {
		/* Check for duplicates */
		/* but only if tk0 contains a proper identifier */
		if (isalpha(node->tk0.tokenInstance[0]) != 0) {
			if (verify(node->tk0.tokenInstance) == true) {
				/* error/exit */
				printf("Error: Declaration failed. %s already exists.\n", node->tk0.tokenInstance);
				exit(1);
			}		
			/* No duplicates found, add ID to trapperKeeper */
			insert(node->tk0.tokenInstance);
		}
	}
	/* See if you are in an "identifier node" (verify only) */
	if (strcmp(node->label, "<R>") == 0 || strcmp(node->label, "<in>") == 0 ||
		strcmp(node->label, "<assign>") == 0) {
		/* Check for duplicates */
		/* but only if tk0 contains a proper identifier */
		if (isalpha(node->tk0.tokenInstance[0]) != 0) {
			if (verify(node->tk0.tokenInstance) == false) {
				/* error/exit */
				printf("Error: %s used before declaration.\n", node->tk0.tokenInstance);
				exit(1);
			}
		}
	}

	semantics(node->child0);
	semantics(node->child1);
	semantics(node->child2);
	semantics(node->child3);
}