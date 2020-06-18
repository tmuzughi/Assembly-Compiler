#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "sem.h"

#define SIZE 100

/*Global Variables*/
char globalArray[SIZE][SIZE];
int newSize;
int inOrderFlag;
char baseName[SIZE];
int numberOfArguments;
bool isKeyboardInput = false;
char name[SIZE];
char label[SIZE];
char nameAr0[SIZE][SIZE]; //store name of temp variables
char realAr0[SIZE][SIZE];
//int nameAr1[SIZE];  //store value of temp variables
char labelAr0[SIZE][SIZE];
int tempCounter = 0;
int realCounter = 0;
int labCounter = 0;
node* temp;
node* temp1;


/*Function Prototypes*/
void getKeyboardInput(int);
void theLandP4Time(node*, FILE*);
int numberOfChildren(node*);
void newName();
void newLabel();

int main(int argc, char* argv[]) {
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
	/* TEMP REMOVE EMPTY CATCH */
	/*Catch for empty file*/
	if (newSize == 0) {
		if (argc == 2) {
			printf("File given is empty.\n");
			return 0;
		}
		else
		{
			printf("No strings entered.\n");
			return 0;
		}
	}

	/* Call test scanner */
	mode = "r";
	if (argc == 2)
		inputFile = fopen(argv[1], mode);
	else
		inputFile = fopen(outputFilename, mode);
	//testScanner(inputFile);

	node * root = parser(inputFile);
	semantics(root);
	/* The Land P4 Time */
	if (isKeyboardInput == true) {
		strcpy(baseName, "kb");
	}
	strcat(baseName, ".asm");
	FILE* asmFile;
	mode = "w";
	asmFile = fopen(baseName, mode);
	fclose(asmFile);
	mode = "a";
	asmFile = fopen(baseName, mode);
	/* Call file constructor */
	theLandP4Time(root, asmFile);

	fclose(asmFile);
	fclose(inputFile);
	printf("%s\n", baseName);
	return 0;
}

void getKeyboardInput(int numberOfArguments) {
	/*Keyboard or redirecting?*/
	if (isatty(STDIN_FILENO)) {
		if (numberOfArguments == 1) {
			isKeyboardInput = true;
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

void theLandP4Time(node * node, FILE * file) {
	/* If empty, abandon ship */
	if (node == NULL)
		return;
	/*-----------PROGRAM-------------*/
	if (strcmp(node->label, "<program>") == 0) {
		//cont traversal to vars
		theLandP4Time(node->child0, file);
		//cont traversal to block
		theLandP4Time(node->child1, file);
		fprintf(file, "STOP\n");
		int i;
		for (i = 0; i < tempCounter; i++) {
			fprintf(file, "%s 0\n", nameAr0[i]);
		}
		for (i = 0; i < realCounter; i++) {
			fprintf(file, "%s 0\n", realAr0[i]);
		}
		return;
	}
	/*------------BLOCK--------------*/
	if (strcmp(node->label, "<block>") == 0) {
		//cont traversal to vars
		theLandP4Time(node->child0, file);
		//cont traversal to stats
		theLandP4Time(node->child1, file);
		return;
	}
	/*--------------VARS--------------*/
	if (strcmp(node->label, "<vars>") == 0) {
		//printf("<vars>\n");
		if (node->tk0.tokenInstance == NULL)
			return;	
		strcpy(realAr0[realCounter], node->tk0.tokenInstance);
		realCounter++;
		fprintf(file, "LOAD %s\n", node->tk1.tokenInstance);
		fprintf(file, "STORE %s\n", node->tk0.tokenInstance);
		//printf("COPY %s %s\n", node->tk0.tokenInstance, node->tk1.tokenInstance);
		//cont traversal to vars
		theLandP4Time(node->child0, file);
		return;		
	}
	/*--------------EXPR--------------*/
	if (strcmp(node->label, "<expr>") == 0) {
		//printf("<expr>\n");
		if (numberOfChildren(node) == 2) {
			//cont traversal to expr
			theLandP4Time(node->child1, file);
			//create our new name
			int rn3 = tempCounter;
			newName();
			char aLocalName[SIZE];
			strcpy(aLocalName, name);
			strcpy(nameAr0[rn3], aLocalName); //set name in array
			fprintf(file, "STORE %s\n", aLocalName);
			//cont traversal to N
			theLandP4Time(node->child0, file);
			fprintf(file, "SUB %s\n", aLocalName);
			return;
		}
		else {
			//cont traversal to N
			theLandP4Time(node->child0, file);
			return;
		}
	}
	/*----------------N----------------*/
	if (strcmp(node->label, "<N>") == 0) {
		//printf("<N>\n");
		if (numberOfChildren(node) == 2) {
			//two children
			if (node->tk0.tokenID == FORWARD_SLASH_TK) {
				theLandP4Time(node->child1, file);
				int aNum = tempCounter;
				newName();
				strcpy(nameAr0[aNum], name); //set name in array
				char localBoi[SIZE];
				strcpy(localBoi, name);
				fprintf(file, "STORE %s\n",localBoi);
				//cont traversal to A
				theLandP4Time(node->child0, file);
				fprintf(file, "DIV %s\n", localBoi);
				return;

			}
			else {
				//token == ASTERISK_TK
				theLandP4Time(node->child1, file);
				int aNum2 = tempCounter;
				newName();
				strcpy(nameAr0[aNum2], name); //set name in array
				char localBoi2[SIZE];
				strcpy(localBoi2, name);
				fprintf(file, "STORE %s\n", localBoi2);
				//cont traversal to A
				theLandP4Time(node->child0, file);
				fprintf(file, "MULT %s\n", localBoi2);
				return;
			}
		}
		else {
			//one child
			//cont traversal to A
			theLandP4Time(node->child0, file);
			return;
		}
	}
	/*----------------A----------------*/
	if (strcmp(node->label, "<A>") == 0) {
		//printf("<A>\n");
		if (numberOfChildren(node) == 2) {
			//printf("<A> has two kids\n");
			//cont traversal to A
			theLandP4Time(node->child1, file);
			//create our new name
			int rn = tempCounter;
			newName();
			strcpy(nameAr0[rn], name); //set name in array
			fprintf(file, "STORE %s\n", name);
			//cont traversal to M
			theLandP4Time(node->child0, file);
			fprintf(file, "ADD %s\n", name);
			return;
		}
		else {
			//printf("<A> has one kid\n");
			//traverse to M
			//<A> only has 1 child so it should be in child0
			theLandP4Time(node->child0, file);
			return;			
		}
	}
	/*----------------M----------------*/
	if (strcmp(node->label, "<M>") == 0) {
		//printf("<M>\n");
		if (node->tk0.tokenID == ASTERISK_TK) {
			//case 1
			//cont traversal to M
			theLandP4Time(node->child0, file);
			fprintf(file, "MULT -1\n");
			return;
		}
		else {
			//case 2
			//cont traversal to R
			theLandP4Time(node->child0, file);
			return;
		}
	}
	/*----------------R----------------*/
	if (strcmp(node->label, "<R>") == 0) {
		//printf("<R>\n");
		if (numberOfChildren(node) == 1) {
			//first case 1 child
			//cont traversal to expr
			theLandP4Time(node->child0, file);
			return;
		}
		else {
			if (node->tk0.tokenID == ID_TK) {
				//second case, Identifier
				fprintf(file, "LOAD %s\n", node->tk0.tokenInstance);
				return;
			} 
			else if (node->tk1.tokenID == INT_TK) {
				//third case, Integer
				fprintf(file, "LOAD %s\n", node->tk1.tokenInstance);
				return;
			}
			
		}
	}
	/*--------------STATS--------------*/
	if (strcmp(node->label, "<stats>") == 0) {
		//printf("<stats>\n");
		//cont traversal to stat
		theLandP4Time(node->child0, file);
		//cont traversal to mStat
		theLandP4Time(node->child1, file);
		return;
	}
	/*--------------MSTAT--------------*/
	if (strcmp(node->label, "<mStat>") == 0) {
		//printf("<mStats>\n");
		if (numberOfChildren(node) == 2) {
			//cont traversal to stat
			theLandP4Time(node->child0, file);
			//cont traversal to mStat
			theLandP4Time(node->child1, file);
			return;
		}
		else {
			return;
		}
	}
	/*---------------STAT--------------*/
	if (strcmp(node->label, "<stat>") == 0) {
		//printf("<stat>\n");
		//cont traversal into whatever is next
		theLandP4Time(node->child0, file);
	}
	/*----------------IN---------------*/
	if (strcmp(node->label, "<in>") == 0) {
		//printf("<in>\n");
		fprintf(file, "READ %s\n", node->tk0.tokenInstance);
		return;
	}
	/*---------------OUT---------------*/
	if (strcmp(node->label, "<out>") == 0) {
		//printf("<out>\n");
		//cont traversal to expr
		int o = tempCounter;
		char outName[SIZE];
		newName();
		strcpy(outName, name);
		strcpy(nameAr0[o], name);
		theLandP4Time(node->child0, file);
		fprintf(file, "STORE %s\n", outName);
		fprintf(file, "WRITE %s\n", outName);
		return;
	}
	/*----------------IF----------------*/
	if (strcmp(node->label, "<if>") == 0) {
		//printf("<if>\n");
		//cont traversal to expr right
		theLandP4Time(node->child2, file);
		int rn2 = tempCounter;
		char localLabel[SIZE];
		int ln = labCounter;
		newLabel();
		strcpy(labelAr0[ln], label);
		strcpy(localLabel, label);
		char localName[SIZE];
		newName();
		strcpy(nameAr0[rn2], name); //set name in array
		strcpy(localName, name);
		fprintf(file, "STORE %s\n", localName);
		//cont traversal to expr left
		theLandP4Time(node->child0, file);
		fprintf(file, "SUB %s\n", localName);  //ACC = eL - eR
			//local label already created
		temp = node->child0;
		temp1 = node->child1;
		if (temp1->tk0.tokenID == EQUALS_EQUALS_TK) {
			fprintf(file, "BRNEG %s\n", localLabel);
			fprintf(file, "BRPOS %s\n", localLabel);
		}
		if (temp1->tk0.tokenID == LESS_THAN_TK) {
			if (temp1->tk1.tokenID == LESS_THAN_TK) {
				fprintf(file, "BRPOS %s\n", localLabel);
			}
			else if (temp1->tk1.tokenID == GREATER_THAN_TK) {
				fprintf(file, "BRZERO %s\n", localLabel);
			}
			else
			fprintf(file, "BRZPOS %s\n", localLabel);			
		}
		if (temp1->tk0.tokenID == GREATER_THAN_TK) {
			if (temp1->tk1.tokenID == GREATER_THAN_TK) {
				fprintf(file, "BRNEG %s\n", localLabel);
			}
			else
			fprintf(file, "BRZNEG %s\n", localLabel);
		}
		//cont traversal to stat
		theLandP4Time(node->child3, file);
		fprintf(file, "%s: NOOP\n", localLabel);
		return;
	}
	/*--------------LOOP--------------*/
	if (strcmp(node->label, "<loop>") == 0) {
		//printf("<loop>\n");
		//cont traversal to expr right
		
		
		int rn21 = tempCounter;
		char localLabel1[SIZE];
		int ln = labCounter;
		newLabel();
		strcpy(labelAr0[ln], label);
		strcpy(localLabel1, label);
		char localName1[SIZE];
		newName();
		strcpy(nameAr0[rn21], name); //set name in array
		strcpy(localName1, name);

		int rn22 = tempCounter;
		char localLabel2[SIZE];
		int ln2 = labCounter;
		newLabel();
		strcpy(labelAr0[ln2], label);
		strcpy(localLabel2, label);
		
		fprintf(file, "%s: NOOP\n", localLabel2);

		theLandP4Time(node->child2, file);
		fprintf(file, "STORE %s\n", localName1);
		//cont traversal to expr left
		theLandP4Time(node->child0, file);
		fprintf(file, "SUB %s\n", localName1);  //ACC = eL - eR
			//local label already created
		temp = node->child0;
		temp1 = node->child1;
		if (temp1->tk0.tokenID == EQUALS_EQUALS_TK) {
			fprintf(file, "BRNEG %s\n", localLabel1);
			fprintf(file, "BRPOS %s\n", localLabel1);
		}
		if (temp1->tk0.tokenID == LESS_THAN_TK) {
			if (temp1->tk1.tokenID == LESS_THAN_TK) {
				fprintf(file, "BRPOS %s\n", localLabel1);
			}
			else if (temp1->tk0.tokenID == GREATER_THAN_TK) {
				fprintf(file, "BRZERO %s\n", localLabel1);
			}
			else
				fprintf(file, "BRZPOS %s\n", localLabel1);
		}
		if (temp1->tk0.tokenID == GREATER_THAN_TK) {
			if (temp1->tk1.tokenID == GREATER_THAN_TK) {
				fprintf(file, "BRNEG %s\n", localLabel1);
			}
			else
				fprintf(file, "BRZNEG %s\n", localLabel1);
		}
		//cont traversal to stat
		theLandP4Time(node->child3, file);
		fprintf(file, "BR %s\n", localLabel2);
		fprintf(file, "%s: NOOP\n", localLabel1);
		return;
	}
	/*--------------ASSIGN--------------*/
	if (strcmp(node->label, "<assign>") == 0) {
		//printf("<assign>\n");
		//cont traversal to expr
		theLandP4Time(node->child0, file);
		fprintf(file, "STORE %s\n", node->tk0.tokenInstance);
		return;
	}
	/*---------------RO---------------*/
	if (strcmp(node->label, "<RO>") == 0) {
		//printf("<RO>\n");
	}
	
}

int numberOfChildren(node* node) {
	int howMany = 0;
	if (node->child0 != NULL)
		howMany++;
	if (node->child1 != NULL)
		howMany++;
	if (node->child2 != NULL)
		howMany++;
	if (node->child3 != NULL)
		howMany++;
	return howMany;
}

void newName() {
	sprintf(name, "T%d", tempCounter++);
}
void newLabel() {
	sprintf(label, "L%d", labCounter++);
}