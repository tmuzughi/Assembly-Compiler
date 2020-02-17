#ifndef NODE_H
#define NODE_H
#define SIZE 100

struct node {
	int level;
	int stringLength;
	char stringArray[SIZE][SIZE]; 
	struct node* leftChild;
	struct node* rightChild;
};

#endif