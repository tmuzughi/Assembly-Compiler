#include <stdio.h>
#include "scanner.h"
#include "token.h"
#include <string.h>
#define SIZE 100

/* array of strings to match with tokenID of type enum */
char tokenType[SIZE][SIZE] = { "BEGIN_TK", "END_TK", "LOOP_TK", "VOID_TK", "VAR_TK", "RETURN_TK", "IN_TK",
	"OUT_TK", "PROGRAM_TK", "IFFY_TK", "THEN_TK", "LET_TK", "DATA_TK",
	"EQUALS_TK", "EQUALS_EQUALS_TK", "PLUS_TK", "MINUS_TK", "ASTERISK_TK", "FORWARD_SLASH_TK", "PERCENT_TK",
	"LESS_THAN_TK", "GREATER_THANK_TK", "COLON_TK", "PERIOD_TK", "SEMI_COLON_TK", "COMMA_TK", "OPEN_PARENTHESIS_TK",
	"CLOSE_PARENTHESIS_TK", "OPEN_BRACE_TK", "CLOSE_BRACE_TK", "OPEN_BRACKET_TK", "CLOSE_BRACKET_TK", "ID_TK",
	"INT_TK", "EOF_TK" };

void testScanner(FILE *inputFile) {
	token currentToken;
	 //Loop until EOF token 
	printf("\nType, Instance, Line #\n");
	while (currentToken.tokenID != 34) {
		currentToken = scanner(inputFile, currentToken);
		printf("%s %s %d\n", tokenType[currentToken.tokenID], currentToken.tokenInstance, currentToken.lineNumber);
	}
	

	
}
