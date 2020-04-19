#ifndef PARSER_H
#define PARSER_H
#include "token.h"

#define max_janikow 100
typedef struct {

	token tk0;
	token tk1;

	struct node* child0;
	struct node* child1;
	struct node* child2;
	struct node* child3;

	char* label;
	int depth;

} node;

node* parser();
node* program();
node* block();
node* vars();
node* expr();
node* N();
node* A();
node* M();
node* R();
node* stat();
node* stats();
node* mStat();
node* in();
node* out();
node* IF();
node* loop();
node* assign();
node* RO();

#endif