#pragma warning(disable:4786)
#include "stdafx.h"
#include "header.h"



int main(int argc, char* argv[])
{
	FILE *inf;
	char fistr[100] = "in1.txt";
	// change from gets(fistr)
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		return 0;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	//	obj.pr_tokentable();
	obj.yyparse();
	printf("aaaa");
	char c; cin >> c;

	return 0;
}


