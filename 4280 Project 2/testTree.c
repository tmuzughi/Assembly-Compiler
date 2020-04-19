#include <stdio.h>
#include "parser.h"

void printInorder(node* nodey)
{
	node* node = nodey;
	if (node == NULL)
		return;
	/* first recur on 0 child */
		//if (node->child0 != NULL)


	//int index = node->index;
	int i;
	//if (node->depth != 0)
	printf("%*c", (node->depth * 2), ' ');
	printf("%s ", node->label);
	if (node->tk0.tokenInstance != NULL)
		printf("%s ", node->tk0.tokenInstance);
	if (node->tk1.tokenInstance != NULL)
		printf("%s", node->tk1.tokenInstance);
	printf("\n");


	printInorder(node->child0);
	printInorder(node->child1);
	printInorder(node->child2);
	printInorder(node->child3);
}