#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"


token tk;
int depth;
FILE* file;

node* parser(FILE* inputFile) {
	file = inputFile;
	node* root;
	tk = scanner(file, tk);
	root = program();
	//program();
	if (tk.tokenID == EOF_TK) {
		//printf("Parser cool\n");	//confirmation
		return root;
	}
	else {
		printf("Error: ln: %d, Expected EOF but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
}
node* program() {
	depth = 0;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->label = "<program>";
	nodeyBoi->depth = depth;
	nodeyBoi->child0 = vars();
	nodeyBoi->child1 = block();
	nodeyBoi->child2 = NULL;
	nodeyBoi->child3 = NULL;
	return nodeyBoi;
}
node* block() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<block>";
	if (tk.tokenID == BEGIN_TK) 
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected begin but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = vars();
	nodeyBoi->child1 = stats();
	if (tk.tokenID == END_TK) {
		tk = scanner(file, tk);
		return nodeyBoi;
	}
	else {
		printf("Error: ln %d Expected end but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
}
node* vars() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<vars>";
	if (tk.tokenID == DATA_TK)
		tk = scanner(file, tk);
	else
		return NULL;
	if (tk.tokenID == ID_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
	}
	else {
		printf("Error: ln %d Expected identifier but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == EQUALS_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected = but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == INT_TK) {
		nodeyBoi->tk1 = tk;
		tk = scanner(file, tk);
	}
	else {
		printf("Error: ln %d Expected integer but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == PERIOD_TK) {
		tk = scanner(file, tk);
	}
	else {
		printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = vars();
	return nodeyBoi;
}
node* expr() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<expr>";
	nodeyBoi->child0 = N();
	if (tk.tokenID == MINUS_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		nodeyBoi->child1 = expr();
	}
	return nodeyBoi;
}
node* N() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<N>";
	nodeyBoi->child0 = A();
	if (tk.tokenID == FORWARD_SLASH_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		nodeyBoi->child1 = N();
		return nodeyBoi;
	}
	if (tk.tokenID == ASTERISK_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		nodeyBoi->child1 = N();
		return nodeyBoi;
	}
	return nodeyBoi;
}
node* A() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<A>";
	nodeyBoi->child0 = M();
	if (tk.tokenID == PLUS_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		nodeyBoi->child1 = A();
		return nodeyBoi;
	}
	return nodeyBoi;
}
node* M() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<M>";
	if (tk.tokenID == ASTERISK_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		nodeyBoi->child0 = M();
		return nodeyBoi;
	}
	nodeyBoi->child0 = R();
	return nodeyBoi;
}
node* R() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<R>";
	if (tk.tokenID == OPEN_PARENTHESIS_TK) {
		tk = scanner(file, tk);
		nodeyBoi->child0 = expr();
		if (tk.tokenID == CLOSE_PARENTHESIS_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected ) but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	if (tk.tokenID == ID_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		return nodeyBoi;
	}
	if (tk.tokenID == INT_TK) {
		nodeyBoi->tk1 = tk;
		tk = scanner(file, tk);
		return nodeyBoi;
	}
	printf("Error: ln %d Expected (, identifier, or integer but found %s\n", tk.lineNumber, tk.tokenInstance);
	exit(1);
}
node* stats() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<stats>";
	nodeyBoi->child0 = stat();
	nodeyBoi->child1 = mStat();
	return nodeyBoi;
}
node* mStat() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<mStat>";
	if (tk.tokenID == IN_TK || tk.tokenID == OUT_TK || tk.tokenID == BEGIN_TK ||
		tk.tokenID == IFFY_TK || tk.tokenID == LOOP_TK || tk.tokenID == ID_TK) {
		nodeyBoi->child0 = stat();
		nodeyBoi->child1 = mStat();
		return nodeyBoi;
	}
	return nodeyBoi;
}
node* stat() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<stat>";
	if (tk.tokenID == IN_TK) {
		nodeyBoi->child0 = in();
		if (tk.tokenID == PERIOD_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	if (tk.tokenID == OUT_TK) {
		nodeyBoi->child0 = out();
		if (tk.tokenID == PERIOD_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	if (tk.tokenID == BEGIN_TK) {
		nodeyBoi->child0 = block();
		return nodeyBoi;
	}	
	if (tk.tokenID == IFFY_TK) {
		nodeyBoi->child0 = IF();
		if (tk.tokenID == PERIOD_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	if (tk.tokenID == LOOP_TK) {
		nodeyBoi->child0 = loop();
		if (tk.tokenID == PERIOD_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	if (tk.tokenID == ID_TK) {
		nodeyBoi->child0 = assign();
		if (tk.tokenID == PERIOD_TK) {
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		else {
			printf("Error: ln %d Expected . but found %s\n", tk.lineNumber, tk.tokenInstance);
			exit(1);
		}
	}
	printf("Error: ln %d Expected in, out, begin, iffy, loop, or identifier but found %s\n", tk.lineNumber, tk.tokenInstance);
	exit(1);
}
node* in() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<in>";
	if (tk.tokenID == IN_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected in but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == ID_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		return nodeyBoi;
	}
	else {
		printf("Error: ln %d Expected identifier but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	printf("Error: ln %d Unexpected token %s\n", tk.lineNumber, tk.tokenInstance);
	exit(1);
}
node* out() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<out>";
	if (tk.tokenID == OUT_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected out but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = expr();
	return nodeyBoi;
}
node* IF() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<if>";
	if (tk.tokenID == IFFY_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected iffy but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == OPEN_BRACKET_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected [ but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = expr();
	nodeyBoi->child1 = RO();
	nodeyBoi->child2 = expr();
	if (tk.tokenID == CLOSE_BRACKET_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected ] but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == THEN_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected then but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child3 = stat();
	return nodeyBoi;
}
node* loop() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<loop>";
	if (tk.tokenID == LOOP_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected loop but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == OPEN_BRACKET_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected [ but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = expr();
	nodeyBoi->child1 = RO();
	nodeyBoi->child2 = expr();
	if (tk.tokenID == CLOSE_BRACKET_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected ] but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child3 = stat();
	return nodeyBoi;
}
node* assign() {
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<assign>";
	if (tk.tokenID == ID_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
	}
	else {
		printf("Error: ln %d Expected identifier but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	if (tk.tokenID == EQUALS_TK)
		tk = scanner(file, tk);
	else {
		printf("Error: ln %d Expected = but found %s\n", tk.lineNumber, tk.tokenInstance);
		exit(1);
	}
	nodeyBoi->child0 = expr();
	return nodeyBoi;
}
node* RO() {	
	depth++;
	/* Create node */
	node* nodeyBoi = (node*)malloc(sizeof(node));
	nodeyBoi->depth = depth;
	nodeyBoi->label = "<RO>";
	if (tk.tokenID == LESS_THAN_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		if (tk.tokenID == LESS_THAN_TK) {
			nodeyBoi->tk1 = tk;
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		if (tk.tokenID == GREATER_THAN_TK) {
			nodeyBoi->tk1 = tk;
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		return nodeyBoi;
	}
	if (tk.tokenID == GREATER_THAN_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		if (tk.tokenID == GREATER_THAN_TK) {
			nodeyBoi->tk1 = tk;
			tk = scanner(file, tk);
			return nodeyBoi;
		}
		return nodeyBoi;
	}
	if (tk.tokenID == EQUALS_EQUALS_TK) {
		nodeyBoi->tk0 = tk;
		tk = scanner(file, tk);
		return nodeyBoi;
	}
	printf("Error: ln %d Unexpected token %s\n", tk.lineNumber, tk.tokenInstance);
	exit(1);
	
}
