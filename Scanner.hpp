#pragma once

#include <string>
#include <map>
#include "Komponent.h"
#include "system.h"
#include "stdafx.h"
using namespace std;

#define	Getc(s)			getc(s)
#define	Ungetc(c)		ungetc(c,IP_Input)

/*
*	Lexical analyzer states.
*/
enum lexstate { L_START, L_INT, L_ELEMENT_NAME, L_BESCHREIBUNG, L_NODE1, L_NODE2 };

const int ELEMENT_NAME = 4;
const int INTEGER1 = 5;
const int BESCHREIBUNG = 500;
const int TOKENSTART = 300;
const int TYPE = 10;
const int NODE1 = 7;
const int NODE2 = 8;
/*const string INPUT = "a";
const string OUTPUT = "b";
const string GND = "c";*/



class CParser
{
public:

	string yytext;								//input buffer
	struct tyylval {								//value return
		string s;								//structure
		int i;
	}yylval;
	FILE *IP_Input;								//Input File
	FILE *IP_Error;								//Error Output
	FILE *IP_List;								//List Output
	int  IP_LineNumber;							//Line counter
	map<string, int> IP_Token_table;				//Tokendefinitions
	map<int, string> IP_revToken_table;			//reverse Tokendefinitions


	int CParser::yylex();						//lexial analyser
	void CParser::yyerror(char *ers);			//error reporter
	int CParser::IP_MatchToken(string &tok);	//checks the token
	void CParser::InitParse(FILE *inp, FILE *err, FILE *lst);
	komponent*	CParser::yyparse_and_init_Netz();		
	system_1 CParser::yyparse_and_get_Knoten();
	void CParser::pr_tokentable();				//test output for tokens
	void CParser::IP_init_token_table();		//loads the tokens
	void CParser::Load_tokenentry(string str, int index);//load one token
	CParser() { IP_LineNumber = 1; };	//Constructor
};
//------------------------------------------------------------------------
