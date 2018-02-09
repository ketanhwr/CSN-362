%%

[a-zA-Z][a-zA-Z0-9]+[^a-zA-Z0-9 \n\t]	{printf("Accepted!");}
.*										{printf("Doesn't accept!");}

%%

int yywrap(void)
{
	return 1;
}
