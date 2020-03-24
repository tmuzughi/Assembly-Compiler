#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#define SIZE 100

int lineNumber = 1;
char currentChar = NULL;
int nextChar;
bool inComment = false;
FILE* file;

char keywords[SIZE][SIZE] = { "begin", "end", "loop", "void", "var", "return", "in", "out", "program", "iffy",
"then", "let", "data" };

token FADriver(token);
void filter();
void getNextChar();

int fsaTable[22][23] = {
	/*     WS    L     D     =     +     -     *     /     %     <     >     :     .     ;     ,     (     )     {    }     [     ]     EOF    else*/
		{  0  ,  1  ,  2  ,  3 ,   5 ,   6  ,  7  ,  8  ,  9 ,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  -1 ,   -2},
		{ 101,   1,    1,   101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101,  101}, /* Identifier Token */
		{ 102,  102,   2,   102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102,  102}, /* Number Token */
		{ 103,  103,  103,   4 ,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103,  103}, /* = Token */
		{ 104,  104,  104,  104 , 104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,  104}, /* == Token */
		{ 105,  105,  105,  105 , 105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105,  105}, /* + Token */
		{ 106,  106,  106,  106 , 106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106,  106}, /* - Token */
		{ 107,  107,  107,  107 , 107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,  107}, /* * Token */
		{ 108,  108,  108,  108 , 108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108,  108}, /* / Token */
		{ 109,  109,  109,  109 , 109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,  109}, /* % Token */
		{ 110,  110,  110,  110 , 110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110}, /* < Token */
		{ 111,  111,  111,  111 , 111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111,  111}, /* > Token */
		{ 112,  112,  112,  112 , 112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112,  112}, /* : Token */
		{ 113,  113,  113,  113 , 113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113,  113}, /* . Token */
		{ 114,  114,  114,  114 , 114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,  114}, /* ; Token */
		{ 115,  115,  115,  115 , 115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,  115}, /* , Token */
		{ 116,  116,  116,  116 , 116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116,  116}, /* ( Token */
		{ 117,  117,  117,  117 , 117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117,  117}, /* ) Token */
		{ 118,  118,  118,  118 , 118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118,  118}, /* { Token */
		{ 119,  119,  119,  119 , 119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119,  119}, /* } Token */
		{ 120,  120,  120,  120 , 120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120}, /* [ Token */
		{ 121,  121,  121,  121 , 121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121,  121} /* ] Token */

		

};

token scanner(FILE *inputFile, token thisToken) {
	/* set file */
	file = inputFile;

	/* Call filter initial */
	filter();
	
	/* Call driver */
	thisToken = FADriver(thisToken);
	
	/* Unget unless newline */
	if (currentChar != '\n')
	ungetc(currentChar, file);

	return thisToken;
}
 
token FADriver(token currentToken) {
	int state = 0;
	int nextState;
	char S[SIZE] = "";

	while (state < 100 && state >= -1) {
		/* Set nextState */
		nextState = fsaTable[state][nextChar];
		/*-----------------------------------
			Unexpected Characters Check
		-----------------------------------*/
		if (nextState == -2) {
			fprintf(stderr, "Error: %c character is not allowed\n", currentChar);
			exit(1);
		}
		/*------------------------------------
					 End Of File
		------------------------------------*/
		if (nextState == -1) {
			currentToken.tokenID = EOF_TK;
			strcpy(currentToken.tokenInstance, "EOF");
			currentToken.lineNumber = lineNumber;

			return currentToken;
		}
		/* If nextState is FINAL */
		if (nextState > 100) { /* final */
			/*------------------------------------
						 IDENTIFIERS
			------------------------------------*/
			if (nextState == 101) {
				/* Check if we have a keyword */
				int i;
				for (i = 0; i < 13; i++) {
					if (strcmp(S, keywords[i]) == 0) {
						currentToken.tokenID = i;
						strcpy(currentToken.tokenInstance, S);
						currentToken.lineNumber = lineNumber;

						return currentToken;
					}
				}
				/* We have a regular identifier */
				currentToken.tokenID = ID_TK;
				strcpy(currentToken.tokenInstance, S);
				currentToken.lineNumber = lineNumber;

				return currentToken;
			}
			/*------------------------------------
						   INTEGERS
			------------------------------------*/
			if (nextState == 102) {
				currentToken.tokenID = INT_TK;
				strcpy(currentToken.tokenInstance, S);
				currentToken.lineNumber = lineNumber;

				return currentToken;
			}
			/*--------------------------------------
					OPERATORS/DELIMETERS
			--------------------------------------*/
			if (nextState > 102 && nextState < 122) {
				int index = (nextState - 90);
				currentToken.tokenID = index;
				strcpy(currentToken.tokenInstance, S);
				currentToken.lineNumber = lineNumber;

				return currentToken;
			}
		}
		else { /* not final */
			state = nextState;
			/* keep comment symbol out of S */
			if (!(currentChar == '@') && !(currentChar == '\n') && !(isspace(currentChar)))
			strncat(S, &currentChar, 1); 
			filter(); 
		}
	}
}

void filter() {
	/* If newline increment line number */
	if (currentChar == '\n') {
		lineNumber++;		
	}	
	/*----------------------------------------------
					DO COMMENTS
	----------------------------------------------*/
	if (currentChar == 64)
		inComment = true;
	
	if (inComment) {
		/* Get next character */
		currentChar = fgetc(file);
		/* Loop until we find the closing @ */
		do {
			currentChar = fgetc(file);
			/* check for newline */
			if (currentChar == '\n') {
				lineNumber++;
			}
			/* Error/EOF check */
			if (currentChar < 0) {
				fprintf(stderr, "Fatal error: Unclosed comment detected\n");
				exit(1);
			}
			inComment = false;
			
		} while (currentChar != 64);
	}
	/*----------------------------------------------*/	
	getNextChar();
}

void getNextChar() {
	bool foundBadChar = true;
	if ((currentChar = fgetc(file)) != EOF) {
		if (isalpha(currentChar)) {
			nextChar = 1;
			foundBadChar = false;
		}
		if (isdigit(currentChar)) {
			nextChar = 2;
			foundBadChar = false;
		}
		if (isspace(currentChar) || currentChar == '\n' || currentChar == '\t') {
			nextChar = 0;
			foundBadChar = false;
		}
		switch (currentChar) {
		case '=':
			nextChar = 3;
			foundBadChar = false;
			break;
		case '+':
			nextChar = 4;
			foundBadChar = false;
			break;
		case '-':
			nextChar = 5;
			foundBadChar = false;
			break;
		case '*':
			nextChar = 6;
			foundBadChar = false;
			break;
		case '/':
			nextChar = 7;
			foundBadChar = false;
			break;
		case '%':
			nextChar = 8;
			foundBadChar = false;
			break;
		case '<':
			nextChar = 9;
			foundBadChar = false;
			break;
		case '>':
			nextChar = 10;
			foundBadChar = false;
			break;
		case ':':
			nextChar = 11;
			foundBadChar = false;
			break;
		case '.':
			nextChar = 12;
			foundBadChar = false;
			break;
		case ';':
			nextChar = 13;
			foundBadChar = false;
			break;
		case ',':
			nextChar = 14;
			foundBadChar = false;
			break;
		case '(':
			nextChar = 15;
			foundBadChar = false;
			break;
		case ')':
			nextChar = 16;
			foundBadChar = false;
			break;
		case '{':
			nextChar = 17;
			foundBadChar = false;
			break;
		case '}':
			nextChar = 18;
			foundBadChar = false;
			break;
		case '[':
			nextChar = 19;
			foundBadChar = false;
			break;
		case ']':
			nextChar = 20;
			foundBadChar = false;
			break;
		case '@':
			foundBadChar = false;
			break;
		default:
			break;
		}
		if (foundBadChar) {
			nextChar = 22;
		}
	}
	else
	{
		nextChar = 21;
	}
}