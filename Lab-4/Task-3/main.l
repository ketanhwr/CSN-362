%{
	#include <stdio.h>
	#include <stdlib.h>

	int wordcount = 0;
	int charactercount = 0;
	int linecount = 0;
%}

%%

\n				{linecount++; charactercount++;}
[A-Za-z0-9]+	{wordcount++; charactercount += yyleng;}
.				{charactercount++;}

%%

int yywrap(void)
{
	return 1;
}

int main(int argc, char **argv)
{
	yyin = fopen("input.txt", "r");
	yylex();

	printf("Word Count: %d\nCharacter Count: %d\nLine Count: %d\n", wordcount, charactercount, linecount);
}
