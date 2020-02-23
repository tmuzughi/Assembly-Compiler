#include "node.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 100

struct node root;


void buildTree() {
	/*Initalize the root node*/
	struct node* rootPtr = &root;	
	root.level = 0;
	root.stringLength = strlen(globalArray[0]);
	root.index = 0;
	root.rightChild = NULL;
	root.leftChild = NULL;
	
	int currentLevel = 0;
	int i;
	struct node* currentNode = &root;
	
	for (i = 0; i < newSize; i++) {		
		/*Check current string length against node*/
		if (strlen(globalArray[i]) == currentNode->stringLength) {
			strcpy(&currentNode->stringArray[currentNode->index], globalArray[i]);
			currentNode->index++;
			currentNode->level = currentLevel;
			/*Set currentNode back to root*/
			currentNode = &root;
			currentLevel = 0;
		} 
		else if (strlen(globalArray[i]) > currentNode->stringLength) {			
			if (currentNode->rightChild == NULL) {
				insertRight(currentNode, strlen(globalArray[i]));
			}
				currentNode = currentNode->rightChild;
				currentLevel++;
				i--;			
		}
		else
		{
			if (currentNode->leftChild == NULL) {
				insertLeft(currentNode, strlen(globalArray[i]));
			}
			currentNode = currentNode->leftChild;
			currentLevel++;
			i--;
		}
		
	}
	
	
}

void printInorder(struct node* node, FILE* file)
{
	if (node == NULL)
		return;
	
	/* first recur on left child */
	printInorder(node->leftChild, file);
	int index = node->index;
	int i;
	if (node->level != 0)
		fprintf(file, "%*c", (node->level * 2), ' ');
	fprintf(file, "%d ", node->stringLength);
	for (i = 0; i < index; i++)
		fprintf(file, "%s ", node->stringArray[i]);
	fprintf(file, "\n");

	/* now recur on right child */
	printInorder(node->rightChild, file);
}

void printPreorder(struct node* node, FILE* file)
{
	if (node == NULL)
		return;
	
	char outputFilename[] = "outputFile.txt";

	int index = node->index;
	int i;
	if (node->level != 0)
		fprintf(file, "%*c", (node->level * 2), ' ');
	fprintf(file, "%d ", node->stringLength);
	for (i = 0; i < index; i++)
		fprintf(file, "%s ", node->stringArray[i]);
	fprintf(file, "\n");

	/* then recur on left sutree */
	printPreorder(node->leftChild, file);

	/* now recur on right subtree */
	printPreorder(node->rightChild, file);
}

void printPostorder(struct node* node, FILE* file)
{
	if (node == NULL)
		return;
	
	// first recur on left subtree 
	printPostorder(node->leftChild, file);

	// then recur on right subtree 
	printPostorder(node->rightChild, file);

	int index = node->index;
	int i;
	if (node->level != 0)
		fprintf(file, "%*c", (node->level * 2), ' ');
	fprintf(file, "%d ", node->stringLength);
	for (i = 0; i < index; i++)
		fprintf(file, "%s ", node->stringArray[i]);
	fprintf(file, "\n");
}

struct node* createNode(int stringLength) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->stringLength = stringLength;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	return newNode;
}

insertLeft(struct node* currentNode, int stringLength) {
	currentNode->leftChild = createNode(stringLength);
}

insertRight(struct node* currentNode, int stringLength) {
	currentNode->rightChild = createNode(stringLength);
}
