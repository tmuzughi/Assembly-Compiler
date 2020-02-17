#include "node.h"
#include "P0.h"
#include <stdio.h>
#include <string.h>

void buildTree() {
	/*Initalize the root node*/
	struct node root;
	root.level = 0;
	root.stringLength = strlen(globalArray[0]);
	strcpy(root.stringArray[0], globalArray[0]);

	printf("Root is on level %d\n", root.level);
	printf("First string in root is %s\n", root.stringArray[0]);
	printf("String length is %d\n", root.stringLength);

	int i;
	for (i = 1; i < newSize; i++) {
		//do stuff
		
	}
	//get the first cell of globalArray and see how many characters are in it
	//look through all existing nodes to see if one exists for that length
	//if so place string into that node
	//else create node, initialize it, and place string in THAT node



	/*printf("I build happy little trees!\n");
	struct node root;
	root.level = 0;
	strcpy(root.stringArray[0], globalArray[0]);
	printf("Root is on level %d\n", root.level);
	printf("First string in root is %s\n", root.stringArray[0]);*/
}

void printInorder() {
	//printf("I do Inorder!\n");
}

void printPreorder() {
	//printf("I do Preorder!\n");
}

void printPostorder() {
	//printf("I do Postorder!\n");
}