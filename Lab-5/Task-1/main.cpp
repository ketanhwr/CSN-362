#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

typedef struct {
	int number;
	char * c;
} Body;

typedef struct {
	int number;
	Body * list;
} Production;

int getInput(int * variables, int * terminals, Production productions[256]);
int computeFirst(int * variables, int * terminals, Production * productions, char ** first);
int firstOf(int * variables, int * terminals, Production * productions, int symbol, char * first);
int computeFollow(int * variables, int * terminals, Production * productions, char ** firstArray);
int isNullable(Production production);
int expandFollow(int * variables, char ** follow);
int displayFollow(char ** follow);

int main()
{
	int variables[256] = {0};
	int terminals[256] = {0};
	Production productions[256];
	int i;
	int j;
	char ** first = (char **) malloc(256 * sizeof(char *));
	for(i = 0; i < 256; i++) {
		first[i] = NULL;
	}
	getInput(variables, terminals, productions);
	computeFirst(variables, terminals, productions, first);
	computeFollow(variables, terminals, productions, first);
	return 0;
}

int computeFollow(int * variables, int * terminals, Production * productions, char ** firstArray)
{
	int head;
	int i;
	int j;
	int k;
	int l;
	int m;
	char ** follow = (char **) malloc(256 * sizeof(char *));
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			follow[i] = (char *) malloc(256 * sizeof(char));
			for (j = 0; j < 256; j++) {
				follow[i][j] = 0;
			}
			if (i == 'S') {
				follow[i]['$'] = 1;
			}
		} else {
			follow[i] = NULL;
		}
	}
	for (head = 0; head < 256; head++) {
		//head is the head of the production

		//iterate over the number of productions for a given head
		for (j = 0; j < productions[head].number; j++) {
			char symbol = '\0';
			char lookahead = '\0';

			//iterate over body of the production
			for(k = 0; k < productions[head].list[j].number; k++) {
				symbol = productions[head].list[j].c[k];
				//symbol is the current symbol being processed

				if (variables[symbol]) {
					//if the current symbol is a variable then 

					//if current symbols is a variable and it is the last in production body
					// then add the head to the follow of symbol
					if (k == productions[head].list[j].number - 1) {
						follow[symbol][head] = 1;
					}

					for (l = k + 1; l < productions[head].list[j].number; l++) {
						lookahead = productions[head].list[j].c[l];
						if (terminals[lookahead]) {
							if ((l == k + 1)) {
								follow[symbol][lookahead] = 1;
							}
							break;
						} else if (variables[lookahead]) {

							//add the first of the variable to the symbol
							for (m = 0; m < 256; m++) {
								if (firstArray[lookahead][m]) {
									follow[symbol][m] = 1;
								}
							}
							if (isNullable(productions[lookahead])) {

								//check if the variables in last in the list
								if (l == productions[head].list[j].number - 1) {

									follow[symbol][head] = 1;
								}
								//continue the loop
							} else {
								break;
							}
						} else {
							printf("Error: no clue\n");
							break;
						}
					}
				}
			}
		}
	}
	expandFollow(variables, follow);
	displayFollow(follow);
}

int expandFollow(int * variables, char ** follow)
{
	int i;
	int j;
	int k;
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			int boolean = 1;
			int z = 3;
			while (boolean) {
				boolean = 0;
				for (j = 0; j < 256; j++) {
					if ((follow[i][j] == 1) && variables[j]) {
						boolean = 1;
						//copy follow of variable 'j' into 'i'
						for (k = 0; k < 256; k++) {
							if ((follow[j][k] == 1) && (follow[i][k] == 0)) {

								follow[i][k] = 1;
							}
						}
						follow[i][j] = -1;

					}
				}
			}
		}
	}
}

int displayFollow(char ** follow)
{
	int i;
	int j;
	printf("Follow\n");
	for (i = 0; i < 256; i++) {
		if (follow[i]) {
			printf("follow( %c ) = { ", i);
			for (j = 1; j < 256; j++) {
				if (follow[i][j] == 1) {
					printf("%c ", j);
				}
			}
			printf("}\n");
		}
	}	
}

int computeFirst(int * variables, int * terminals, Production * productions, char ** first)
{
	int i;
	int j;
	int k;
	char symbol;
	printf("First\n");
	for (i = 0; i < 256; i++) {
		//if it is a terminal
		if (variables[i]) {
			first[i] = (char *) malloc(256 * sizeof(char));
			firstOf(variables, terminals, productions, i, first[i]);
			//display first of the terminal
			printf("%c -> { ", i);
			if (first[i][0]) {
				printf("epsilon ");
			}
			for (j = 1; j < 256; j++) {
				if (first[i][j]) {
					printf("%c ", j);
				}
			}
			printf("}\n");
		}
	}
	return 0;
}

int firstOf(int * variables, int * terminals, Production * productions, int symbol, char * first)
{
	int lookup;
	int j;
	int k;

	for (j = 0; j < productions[symbol].number; j++) {
		for (k = 0; k < productions[symbol].list[j].number; k++) {
			lookup = productions[symbol].list[j].c[k];
			if (variables[lookup]) {
					;
					/* if not nullable find first of 
					*/
					firstOf(variables, terminals, productions, lookup, first);
					if (!isNullable(productions[lookup])) {
						/*break the loop*/
						break;
					}
			} else if (terminals[lookup]) {
				first[lookup] = 1;
				break;

			} else if (lookup == '\0') {
				first[lookup] = 1;
			} else{
				printf("error -> %d symbol neither a terminal nor a variable", lookup);
			}
		}
		//when the loop completes it means the body is nullable
		if (k == productions[symbol].list[j].number) {
			first[0] = 1;
		}
	}

	return 0;
}

int isNullable(Production production) {
	int i;
	for (i = 0; i < production.number; i++) {
		if (production.list[i].number && (production.list[i].c[0] == '\0')) {
			return 1;
		}
	}
	return 0;
}

int getInput(int * variables, int * terminals, Production productions[256])
{
	int i;
	int j;
	char c;
	int numVariables;
	int numTerminals;
	int numProductions;
	char head;
	char body[50];//just an assumption for input length
	cout << "Variables: ";
	cin >> numVariables;
	for (i = 0; i < numVariables; i++) {
		cin >> c;
		variables[c] = 1;
	}
	cout << "Terminals: ";
	cin >> numTerminals;
	for (i = 0; i < numTerminals; i++) {
		cin >> c;
		terminals[c] = 1;
	}
	/*initialize production array*/
	for (i = 0; i < 256; i++) {
		productions[i].number = 0;
		productions[i].list= NULL;
	}
	cout << "Number of Productions: ";
	cin >> numProductions;
	for (i = 0; i < numProductions; i++) {
		cin >> head >> body;
		if (variables[head]) {
			productions[head].number++;
			productions[head].list = (Body *) realloc(productions[head].list, productions[head].number * sizeof(Body));
			productions[head].list[productions[head].number - 1].number = 0;
			productions[head].list[productions[head].number - 1].c = NULL;
			for (j = 0; j < strlen(body); j++) {
				productions[head].list[productions[head].number - 1].number++; 
				productions[head].list[productions[head].number - 1].c = (char *) realloc(productions[head].list[productions[head].number - 1].c, productions[head].list[productions[head].number - 1].number * sizeof(char));
				if (variables[body[j]] || terminals[body[j]]) {
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = body[j];
				} else {
					//this character is treated as a null character (which is neither in variables nor terminals)
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = '\0';
				}
			}
		}
	}
	return 0;
}