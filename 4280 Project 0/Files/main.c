#include "tree.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define SIZE 100

/*Global Variables*/
char globalArray[SIZE][SIZE];
int newSize;
int inOrderFlag;
char baseName[SIZE];
int numberOfArguments;

/*Function Prototypes*/
void getKeyboardInput(int);

int main(int argc, char *argv[]) {
	/*set the global flags for later*/
	inOrderFlag = 0;

	if (argc == 2) {
		strcpy(baseName, argv[1]);
	}

	/*------------------------------------
		Keyboard input or redirecting preparation
		--------------------------------------*/
		/*Error handling*/
	if (argc > 2) {
		printf("Error reading command line argument.\n");
		exit(1);
	}
	getKeyboardInput(argc);
	/*-----------------------------------
			Regular file input section
		-----------------------------------*/
	FILE* inputFile;
	char* mode = "r";
	char outputFilename[] = "outputFile.txt";
	if (argc == 2) {
		/*check for extension*/
		//46 is int value of "."
		if (!strrchr(argv[1], 46)) {		
			/*add file extension to argument*/
			strcat(argv[1], ".sp2020");			
		}
		inputFile = fopen(argv[1], mode);
	}
	else
		inputFile = fopen(outputFilename, mode);
	/*Error handling*/
	if (inputFile == NULL) {
		printf("Error reading file.\n");
		exit(1);
	}
	/*Determine the number of strings in the file*/
	int numberOfStrings = 0;
	char tempVariable[SIZE][SIZE];
	while (fscanf(inputFile, "%s", tempVariable[0]) != EOF)	numberOfStrings++;	
	/*Close and reopen file*/
	fclose(inputFile);
	if (argc == 2) 
		inputFile = fopen(argv[1], mode);
	else
		inputFile = fopen(outputFilename, mode);
	/*Put strings from file into array*/
	char inputArray[SIZE][SIZE];
	int i = 0;
	while (fscanf(inputFile, "%s", inputArray[i]) != EOF) i++;
	fclose(inputFile);
	/*Remove duplicates from array*/
	int index = 0;
	newSize = 0;
	for (i = 0; i < numberOfStrings; i++) {
		int j;
		for (j = 0; j < i; j++)
			if (strcmp(inputArray[i], inputArray[j]) == 0)
				break;
		if (j == i) {
			strcpy(globalArray[index++], inputArray[i]);
			newSize++;
		}
	}

	/*--------------------------------
			  Caw Caw Calls
	---------------------------------*/
	buildTree();

	FILE* inOrderOutputFile;
	char* inOrderMode = "w";
	if (argc == 2) {
		strcpy(argv[1], baseName);
		strcat(argv[1], ".inorder");		
		inOrderOutputFile = fopen(argv[1], inOrderMode);
	}
	else
	{		
		char inOrderFilename[] = "output.inorder";
		inOrderOutputFile = fopen(inOrderFilename, inOrderMode);
	}

	printInorder(&root, inOrderOutputFile);
		fclose(inOrderOutputFile);
	
	FILE* preOrderOutputFile;
	char* preOrderMode = "w";
	if (argc == 2) {
		strcpy(argv[1], baseName);
		strcat(argv[1], ".preorder");
		preOrderOutputFile = fopen(argv[1], preOrderMode);
	}
	else
	{
		char preOrderFilename[] = "output.preorder";
		preOrderOutputFile = fopen(preOrderFilename, preOrderMode);
	}

	printPreorder(&root, preOrderOutputFile);
		fclose(preOrderOutputFile);
	
		FILE* postOrderOutputFile;
		char* postOrderMode = "w";
		if (argc == 2) {
			strcpy(argv[1], baseName);
			strcat(argv[1], ".postorder");
			postOrderOutputFile = fopen(argv[1], postOrderMode);
		}
		else
		{
			char postOrderFilename[] = "output.postorder";
			postOrderOutputFile = fopen(postOrderFilename, postOrderMode);
		}
	printPostorder(&root, postOrderOutputFile);
		fclose(postOrderOutputFile);

	return 0;
}

void getKeyboardInput(int numberOfArguments) {
	/*Keyboard or redirecting?*/
	if (isatty(STDIN_FILENO)) {
		if (numberOfArguments == 1) {
			/*Take input from keyboard*/
			FILE* outputFile;
			char* mode = "w";
			char outputFilename[] = "outputFile.txt";
			outputFile = fopen(outputFilename, mode);
			/*More error handling*/
			if (outputFile == NULL) {
				printf("Error creating temp file.");
			}
			char holdIt[SIZE];
			//prompt user for terminator
			printf("Press 'CTRL + D' (or 'CTRL + Z') to stop.\n");
			while (scanf("%s", holdIt) != EOF)
				fprintf(outputFile, "%s\n", holdIt);
			fclose(outputFile);
		}
	}
	else
	{
		if (numberOfArguments != 2) {
			/*Take input from redirected file*/
			FILE* outputFile;
			char* mode = "w";
			char outputFilename[] = "outputFile.txt";
			outputFile = fopen(outputFilename, mode);
			/*More error handling*/
			if (outputFile == NULL) {
				printf("Error creating temp file.");
			}
			char holdIt[SIZE];
			while (scanf("%s", holdIt) != EOF)
				fprintf(outputFile, "%s\n", holdIt);

			fclose(outputFile);
		}

	}
}
