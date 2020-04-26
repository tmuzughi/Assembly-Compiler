#include <stdio.h>
#include "parser.h"
#include <stdbool.h>
#include <string.h>
#define maxJanikow 100

/* Global */
char trapperKeeper[maxJanikow][maxJanikow];
bool oneTime = true;

void insert(char* string) {
	int i;
	for (i = 0; i < maxJanikow; i++) {
		if (strcmp(trapperKeeper[i], " ")) {
			/* Insert ID into empty spot */
			strcpy(trapperKeeper[i], string);
			//printf("%s has been inserted\n", trapperKeeper[i]);
			break;
		}
	}
}

bool verify(char* string) {
	int i;
	for (i = 0; i < maxJanikow; i++) {
		if (strcmp(trapperKeeper[i], string) == 0) {
			/* We found a duplicate */
			return true;
		}
	}
	return false;
}

void semantics(node* noduh) {
	
	if (oneTime == true) {
		int i;
		for (i = 0; i < maxJanikow; i++) {
			trapperKeeper[i] == " ";
		}
		oneTime = false;
	}
	
	node* node = noduh;
	if (node == NULL)
		return;
	//printf("%s\n", node->label);
	/* See if you are in an "identifier node" */
	if (strcmp(node->label, "<vars>") == 0) {
		/* Check for duplicates */
		/* but only if tk0 contains a proper identifier */
		if (isalpha(node->tk0.tokenInstance[0]) != 0) {
			if (verify(node->tk0.tokenInstance) == true) {
				printf("Error: Declaration failed. %s already exists.\n", node->tk0.tokenInstance);
				exit(1);
			}		
			/* No duplicates found, add ID to trapperKeeper */
			insert(node->tk0.tokenInstance);
		}
	}
	if (strcmp(node->label, "<R>") == 0 || strcmp(node->label, "<in>") == 0 ||
		strcmp(node->label, "<assign>") == 0) {
		if (isalpha(node->tk0.tokenInstance[0]) != 0) {
			if (verify(node->tk0.tokenInstance) == false) {
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

