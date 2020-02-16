#include "tree.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char *argv[]) {
	if (argc == 2) {
		/*-----------------------------------
			Regular file input detected
		-----------------------------------*/
		FILE* inputFile;
		char* mode = "r";
		inputFile = fopen(argv[1], mode);
		/*Error handling*/
		if (inputFile == NULL) {
			printf("Error reading file.\n");
			exit(1);
		}		
		/*Determine the number of strings in the file*/
		int numberOfStrings = 0;
		char tempVariable[SIZE][SIZE];
		while (fscanf(inputFile, "%s", tempVariable[0]) != EOF) numberOfStrings++;
		printf("%d\n", numberOfStrings);
		/*Close and reopen file*/
		fclose(inputFile);
		inputFile = fopen(argv[1], mode);
		/*Put strings from file into array*/
		char inputArray[SIZE][numberOfStrings];
		int i = 0;
		while (fscanf(inputFile, "%s", inputArray[i]) != EOF) i++;
		fclose(inputFile);
		/*Remove duplicates from array*/
		char newArray[SIZE][numberOfStrings];
		int index = 0;
		int newSize = 0;
		for (i = 0; i < numberOfStrings; i++) {
			int j;
			for (j = 0; j < i; j++) 
				if (strcmp(inputArray[i], inputArray[j]) == 0)
					break;
			if (j == i) {
				strcpy(newArray[index++], inputArray[i]);
				newSize++;
			}																
		}
	}
	else {
		/*------------------------------------
		Keyboard input or redirecting detected
		--------------------------------------*/
		/*Error handling*/
		if (argc > 2) {
			printf("Error reading command line argument.\n");
			exit(1);
		}
		/*Keyboard or redirecting?*/
		if (isatty(STDIN_FILENO)) {
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
			printf("Press 0 to stop.\n");
			bool inputLoopTrigger = true;
			int index = -1;
			while (inputLoopTrigger) {
				index++;
				//get some input
				scanf("%s", &holdIt);
				//check for terminator
				if (strchr(holdIt, '0'))
					inputLoopTrigger = false;
				if (inputLoopTrigger) {
					//put that input into output file
					fprintf(outputFile, "%s\n", holdIt);
				}
			}
		}			
		else
		{
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
	return 0;
}