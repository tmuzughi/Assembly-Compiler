#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 100



int main(int argc, char *argv[]){

//Determine if there is an argument
bool isArg = false;
if (argc > 1)
	isArg = true;

char array[SIZE][SIZE];
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
	//put that input into the array
	strcpy(array[index], holdIt);
	}
}

int i;
for (i = 0; i < index; i++)
	printf("%s\n", array[i]);

return 0;
}
