#ifndef NODE_H
#define NODE_H
#define SIZE 100
typedef struct node {
	int stringLength;
	char stringArray[SIZE][SIZE]; //note: I would like to change this dynamically to conserve memory
} node;
#endif