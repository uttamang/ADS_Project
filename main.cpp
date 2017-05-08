#pragma warning(disable:4786)
#include "stdafx.h"
#include "header.h"

komponent* RLC;
komponent* last_RLC;


void scanner(void)
{
	FILE *inf;
	char fistr[100] = "in1.txt";
	// change from gets(fistr)
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		return;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	//	obj.pr_tokentable();
	last_RLC = obj.yyparse_and_init_Netz();
}

void print_kanten_tabelle(void)
{
	komponent* rt = last_RLC;
	printf("NODE1| ");
	while (rt != NULL)
	{
		printf("%s ", rt->NODE_1.c_str());
		rt = rt->next;
	}
	printf("| \n");

	printf("NODE2| ");
	rt = last_RLC;
	while (rt != NULL)
	{
		printf("%s ", rt->NODE_2.c_str());
		rt = rt->next;
	}
	printf("| \n");

	rt = last_RLC;
	printf("Kante| ");
	while (rt != NULL)
	{
		printf("%s ", rt->Element.c_str());
		rt = rt->next;
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	scanner();
	print_kanten_tabelle();
	zusammenfassung Netz;
	Netz.parallel();

	printf("Nach Parallel \n \n");
	print_kanten_tabelle();
	return 0;
}


