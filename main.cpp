#pragma warning(disable:4786)
#include "stdafx.h"
#include "header.h"

komponent* RLC;
system_1* sys_pointer;
komponent* scanner(void)
{
	FILE *inf;
	komponent* last_RLC;
	char fistr[100] = "in2.txt";
	// change from gets(fistr)
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		return NULL;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
		//obj.pr_tokentable();
	sys_pointer = obj.yyparse_and_get_Knoten();
	
	last_RLC = obj.yyparse_and_init_Netz();
	return last_RLC;
}

void print_kanten_tabelle(komponent* last_RLC)
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
	komponent* last_RLC;
	last_RLC = scanner();
	print_kanten_tabelle(last_RLC);
	vereinfachen Netz;
	Netz(last_RLC);
	print_kanten_tabelle(last_RLC);

	Knotenlist* node1 = Netz.setKnoten(last_RLC);
	Netz.display_node(node1);
	return 0;
}


