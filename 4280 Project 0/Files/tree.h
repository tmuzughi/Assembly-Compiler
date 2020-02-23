#ifndef TREE_H
#define TREE_H

struct node root;

void buildTree();
void printInorder(struct node*);
void printPreorder(struct node*);
void printPostorder(struct node*);

#endif