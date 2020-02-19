#include "node.h"
#include "P0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 100

void buildTree() {
	/*Initalize the root node*/
	struct node root;
	root.level = 0;
	root.stringLength = strlen(globalArray[0]);
	//strcpy(root.stringArray[0], globalArray[0]);
	root.index = 0;
	root.rightChild = NULL;
	root.leftChild = NULL;
	


	/*
	printf("Root is on level %d\n", root.level);
	printf("First string in root is %s\n", root.stringArray[0]);
	printf("String length is %d\n", root.stringLength);
	*/

	int i;
	struct node* currentNode = &root;
	/*
	printf("%d\n", currentNode->stringLength);
	printf("***\n");
	for (i = 0; i < newSize; i++) {
		printf("%d\n", strlen(globalArray[i]));
	}
	*/

	for (i = 0; i < newSize; i++) {
		//printf("string length of current node at start of loop is %d\n", currentNode->stringLength);
		/*Check current string length against node*/
		if (strlen(globalArray[i]) == currentNode->stringLength) {
			strcpy(&currentNode->stringArray[currentNode->index], globalArray[i]);
			currentNode->index++;
			/*Set currentNode back to root*/
			currentNode = &root;
		} 
		else if (strlen(globalArray[i]) > currentNode->stringLength) {			
			if (currentNode->rightChild == NULL) {
				//printf("right child = null\n");
				insertRight(currentNode, strlen(globalArray[i]));
			}
				currentNode = currentNode->rightChild;
				i--;			
		}
		else
		{
			if (currentNode->leftChild == NULL) {
				//printf("left child = null\n");
				insertLeft(currentNode, strlen(globalArray[i]));
			}
			currentNode = currentNode->leftChild;
			i--;
		}
		
		/*Reset currentNode to root*/
		//currentNode = &root;
	}
	/*
	for (i = 0; i < 5; i++) {
		printf("%s\n", root.stringArray[i]);
	}
	for (i = 0; i < 5; i++) {
		currentNode = root.rightChild;
		printf("%s\n", currentNode->stringArray[i]);
	}
	for (i = 0; i < 5; i++) {
		currentNode = root.leftChild;
		printf("%s\n", currentNode->stringArray[i]);
	}
	currentNode = root.rightChild;
	if (currentNode->rightChild != NULL) {
		currentNode = currentNode->rightChild;
		for (i = 0; i < 5; i++) {
			printf("%s\n", currentNode->stringArray[i]);
		}
	}
	currentNode = root.leftChild;
	if (currentNode->leftChild != NULL) {
		currentNode = currentNode->leftChild;
		for (i = 0; i < 5; i++) {
			printf("%s\n", currentNode->stringArray[i]);
		}
	}
	currentNode = root.leftChild;
	if (currentNode->leftChild != NULL) {
		currentNode = currentNode->leftChild;
		if (currentNode->rightChild != NULL) {
			currentNode = currentNode->rightChild;
			for (i = 0; i < 5; i++) {
				printf("%s\n", currentNode->stringArray[i]);
			}
		}
		
	}
	*/
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

struct node* createNode(int stringLength) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->stringLength = stringLength;
	//printf("string length of created node is %d\n", newNode->stringLength);
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;
	//printf("i created a node i think\n");
	return newNode;
}

insertLeft(struct node* currentNode, int stringLength) {
	//printf("string length of current string before insertion is %d\n", stringLength);
	//printf("string length of passed node before insertion is %d\n", currentNode->stringLength);
	currentNode->leftChild = createNode(stringLength);
}

insertRight(struct node* currentNode, int stringLength) {
	//printf("string length of current string before insertion is %d\n", stringLength);
	//printf("string length of passed node before insertion is %d\n", currentNode->stringLength);
	currentNode->rightChild = createNode(stringLength);
}
