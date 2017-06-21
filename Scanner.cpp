// K7scan0.cpp 
//
#include "stdafx.h"
#include "Scanner.hpp"
extern unsigned char k_index;

void CParser::Load_tokenentry(string str, int index)
{
	IP_Token_table[str] = index;
	IP_revToken_table[index] = str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("IDENTIFIER",4);
	Load_tokenentry("INTEGER1",5);

	int ii = TOKENSTART;
	Load_tokenentry("(", ii++);
	Load_tokenentry(")", ii++);
	Load_tokenentry(":", ii++);
	Load_tokenentry(";", ii++);
	Load_tokenentry(",", ii++);
	Load_tokenentry("NETS", ii++);
	Load_tokenentry("INTERNAL", ii++);
	Load_tokenentry("CMN",ii++);
	Load_tokenentry("OUT",ii++);
	Load_tokenentry("IN", ii++);
	Load_tokenentry("R", ii++);
	Load_tokenentry("C", ii++);
	Load_tokenentry("L", ii++);
}
//------------------------------------------------------------------------

void CParser::pr_tokentable()
{

	typedef map<string, int>::const_iterator CI;
	const char* buf;

	printf("Symbol Table ---------------------------------------------\n");

	for (CI p = IP_Token_table.begin(); p != IP_Token_table.end(); ++p) {
		buf = p->first.c_str();
		printf(" key:%s", buf);
		printf(" val:%d\n", p->second);;
	}
}
//------------------------------------------------------------------------
/*********************************************************************************************************
* Funktion: Die Funktion führt die lexikalische Analyse durch und charakterisiert das Ergebnis nach der 
*			Analyse. Laut dem Input der Form "(Elementname):(Elementtype)(Knoten1,Knoten2)" ist der Zähler
*			des Index
*			0 : Elementname
*			1 : Knoten1
*			2 : Knoten2
*			Die Elementtypen werden in Token-Tabelle eingetragen, sodass sie nicht mehr IDENTIFIER sind.
*			Die obigen Informationen sind in der Komponentlist der Klasse "komponent" gespeichert. Am Ende
*			der Funktion wird der Zeiger auf das letzte Element zurückgegeben.
*		
*			Am Anfang der Input-Datei sind die Information der INPUT, OUTPUT, GND Knoten. Diese werden in 
*			"netz_id" gespeichert. Sie sind in der Datei so beschrieben "NETS:a:IN;b:OUT;c:CMN;d,e:INTERNAL;"
*			Aus diesem Grund wird am Anfang des Funktionsstart diese Information entnommen. Die zugehörigen 
*			Zähler-Index sind
*			0: Input-Knote
*			1: Output-Knote
*			2: GND-Knote
*
*	Argument: 
*			system_1& netz_id: Variable zum Speichern der Input-, Output-, GND-Knoten.
*
*	Rückgabewert:
*			komponent* : Zeiger auf das letzte Element der Komponentlist.
*/
komponent*	CParser::yyparse_and_init_Netz(system_1& netz_id)
{
	int tok;
	char ID_Count = 0;
	komponent* rt = NULL;
	komponent* first = NULL;
	/*
	*	Go parse things!
	*/
	while ((tok = yylex()) != 0) {
		printf("%d ", tok);
		if (tok == INTEGER1) 
			printf("%s %d ", IP_revToken_table[tok].c_str(), yylval.i);
		else
			if (tok == IDENTIFIER && IP_LineNumber == 1)   // init datalist
			{
				printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());

				/*Die Beschreibung wird hier überprüft.*/
				if (k_index <= 2)
				{
					if (k_index <= 1)
					{
						k_index == 0 ? netz_id.INPUT = yylval.s : netz_id.OUTPUT = yylval.s;
					}
					else
						netz_id.GND = yylval.s;
				}
				k_index++;
			}
			else if(tok == IDENTIFIER && IP_LineNumber != 1 )
			{
				printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
				if (ID_Count == 0)
				{
					first = new komponent;
					first->Element = yylval.s.c_str();
					first->next = rt;
					rt = first;
					ID_Count++;
				}
				else if (ID_Count == 1)
				{
					first->NODE_1 = yylval.s.c_str();
					ID_Count++;
				}
				else
				{
					first->NODE_2 = yylval.s.c_str();
					ID_Count = 0;
				}
			}
		printf("\n");
	}

	// get the latest adresse
	return rt;

}
//------------------------------------------------------------------------

/*
*	Parse Initfile:
*
*	  This builds the context tree and then calls the real parser.
*	It is passed two file streams, the first is where the input comes
*	from; the second is where error messages get printed.
*/
void CParser::InitParse(FILE *inp, FILE *err, FILE *lst)

{

	/*
	*	Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List = lst;

	IP_LineNumber = 1;
	/*
	*	Define both the enabled token and keyword strings.
	*/
	IP_init_token_table();
}
//------------------------------------------------------------------------

/*
*	yyerror:
*
*	  Standard error reporter, it prints out the passed string
*	preceeded by the current filename and line number.
*/
void CParser::yyerror(char *ers)

{
	fprintf(IP_Error, "line %d: %s\n", IP_LineNumber, ers);
}
//------------------------------------------------------------------------

int CParser::IP_MatchToken(string &tok)
{
	int retval;
	if (IP_Token_table.find(tok) != IP_Token_table.end()) {
		retval = (IP_Token_table[tok]);
	}
	else {
		retval = (0);
	}
	return retval;
}

//------------------------------------------------------------------------

/*
*	yylex:
*
*/
int CParser::yylex()
{
	//Locals
	int c;
	lexstate s;
	/*
	*	Keep on sucking up characters until we find something which
	*	explicitly forces us out of this function.
	*/
	for (s = L_START, yytext = ""; 1;) {
		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START:
			if (isdigit(c)) {
				s = L_INT;
			}
			else if (isalpha(c)) {
				s = L_IDENT;
			}
			else if (isspace(c)) {
				if (c == '\n') {
					IP_LineNumber += 1;
				}
				yytext = "";
			}
			else if (c == EOF) {
				return ('\0');
			}
			else {
				return (c);
			}
			break;

			/*
			*	Suck up the integer digits.
			*/
		case L_INT:
			if (isdigit(c)) {
				break;
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.i = atoi(yylval.s.c_str());
				return (INTEGER1);
			}
			break;

			/*
			*	Grab an identifier, see if the current context enables
			*	it with a specific token value.
			*/

		case L_IDENT:
			if (isalpha(c) || isdigit(c) || c == '_')
				break;
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1);
			yylval.s = yytext;
			if (c = IP_MatchToken(yytext)) {
				return (c);
			}
			else {
				return (IDENTIFIER);
			}

		default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}
//------------------------------------------------------------------------