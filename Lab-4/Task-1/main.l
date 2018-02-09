%{
	#include <stdlib.h>
	#include <stdio.h>

	int lineno = 0;
	FILE *output;
%}

%%

\n		{}
.*		{lineno++; fprintf(output, "%2d: %s\n", lineno, yytext);}

%%

int yywrap(void)
{
	return 1;
}

int main(int argc, char **argv)
{
	yyin = fopen("input.txt", "r");
	output = fopen("output.txt", "w");

	yylex();

	fclose(yyin);
	fclose(output);
}
