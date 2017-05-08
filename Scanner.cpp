// K7scan0.cpp 
//
#include "Scanner.hpp"
void CParser::Load_tokenentry(string str,int index)
{
	IP_Token_table[str]=index;
	IP_revToken_table[index]=str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("ELEMENT_NAME",4);
	Load_tokenentry("INTEGER1",5);
	Load_tokenentry("NODE1", 7);
	Load_tokenentry("NODE2", 8);
	Load_tokenentry("ELEMENT", 9);
	int type = TYPE;
	Load_tokenentry("R", type++);
	Load_tokenentry("C", type++);
	Load_tokenentry("L", type++);

	int i = BESCHREIBUNG;
	Load_tokenentry("NETS", i++);
	Load_tokenentry("IN", i++);
	Load_tokenentry("OUT", i++);
	Load_tokenentry("CMN", i++);
	Load_tokenentry("INTERNAL", i++);

	int ii=TOKENSTART;
	Load_tokenentry("(",ii++);
	Load_tokenentry(")",ii++);
	Load_tokenentry(":",ii++);
	Load_tokenentry(";", ii++);
}
//------------------------------------------------------------------------

void CParser::pr_tokentable()
{
	
	typedef map<string,int>::const_iterator CI;
	const char* buf;

	printf( "Symbol Table ---------------------------------------------\n");

	for(CI p=IP_Token_table.begin(); p!=IP_Token_table.end(); ++p){
		buf = p->first.c_str();
		printf(" key:%s", buf);
		printf(" val:%d\n", p->second);;
	}
}
//------------------------------------------------------------------------

komponent*	CParser::yyparse_and_init_Netz()
{
	int tok;
	int index = 0;
	komponent* rt = NULL;
	komponent* first;
	/*
	*	Go parse things!
	*/
	while ((tok=yylex())!=0){
		printf("%d ",tok);
		if (tok == INTEGER1)
			printf("%s %d ", IP_revToken_table[tok].c_str(), yylval.i);
		else
			if (tok == ELEMENT_NAME)   // init datalist
			{
				printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
				first = new komponent;
				first->Element = yylval.s.c_str();
				first->index = index;
				first->next = rt;
				rt = first;
			}
			else
				if (tok >= BESCHREIBUNG)
					printf(" BESCHREIBUNG : %s %s", IP_revToken_table[tok].c_str(), yylval.s.c_str());
				else
					if (tok >= TOKENSTART)
						printf("%s ", IP_revToken_table[tok].c_str());
					else
						if (tok >= TYPE && tok <= 13)
							printf("TYPE: %s ", IP_revToken_table[tok].c_str());
						else
							if (tok == NODE1)
							{
								printf("%s : %s", IP_revToken_table[tok].c_str(), yylval.s.c_str());
								first->NODE_1 = yylval.s.c_str();
							}
							else
								if (tok == NODE2)
								{
									printf("%s : %s", IP_revToken_table[tok].c_str(), yylval.s.c_str());
									first->NODE_2 = yylval.s.c_str();
								}
								else
									printf("%c ",tok);
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
void CParser::InitParse(FILE *inp,FILE *err,FILE *lst)

{

	/*
	*	Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List  = lst;

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
  fprintf(IP_Error,"line %d: %s\n",IP_LineNumber,ers);
}
//------------------------------------------------------------------------

int CParser::IP_MatchToken(string &tok)
{
	int retval;
	if(	IP_Token_table.find(tok) != IP_Token_table.end()){
		retval = (IP_Token_table[tok]);
	}else{
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
	size_t pos;
	string temp_text;
	lexstate s;
	/*
	*	Keep on sucking up characters until we find something which
	*	explicitly forces us out of this function.
	*/
	for (s = L_START,yytext = ""; 1;){
		if (IP_LineNumber == 1)
			s = L_BESCHREIBUNG;

		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		switch (s){
		  //Starting state, look for something resembling a token.
			case L_START:
				if (isdigit(c)){
				  s = L_INT;
				}else if (isalpha(c) ){
						s = L_ELEMENT_NAME;
				}else if (isspace(c)){
							if (c == '\n'){
								IP_LineNumber += 1;
							}
							yytext = "";
				}else if (c == EOF){
							return ('\0');
				}
				else if (c == '(')
					s = L_NODE1;
				else if (c == ',')
					s = L_NODE2;
				else{						
						return (c);
				}
			break;

		  /*
		   *	Suck up the integer digits.
		   */
			case L_INT:
				if (isdigit(c)){
				  break;
				}else {
					Ungetc(c);
					yylval.s = yytext.substr(0,yytext.size()-1);
					yylval.i = atoi(yylval.s.c_str());
					return (INTEGER1);
				}
			break;

		  /*
		   *	Grab an identifier, see if the current context enables
		   *	it with a specific token value.
		   */
			
			case L_ELEMENT_NAME:
			   if (isalpha(c) || isdigit(c) || c == '_')
				  break;
				Ungetc(c);
				yytext = yytext.substr(0,yytext.size()-1);
				if (yytext == "NETS")
				{
					s = L_BESCHREIBUNG;
					yytext = "";
					break;
				}
				
				yylval.s = yytext;
				if (c = IP_MatchToken(yytext)){
					return (c);
				}else{
					return (ELEMENT_NAME);
				}
			
			case L_BESCHREIBUNG:
				if (isalpha(c) || c == ',')
					break;
				yytext = yytext.substr(0, yytext.size());
				if (yytext.find(':', 0)!= string::npos)
				{
					temp_text = yytext.substr(0, yytext.size()-1);
					yytext = "";
					break;
				}
				else
					if (yytext.find(';', 0) != string::npos)
						{
							yylval.s = temp_text;
							yytext = yytext.substr(0,yytext.size()-1);
						}
						else

							if (yytext == "NETS")
							{
								s = L_BESCHREIBUNG;
								yytext = "";
								break;
							}
								else
									if (isspace(c)) 
									{
										if (c == '\n') {
											IP_LineNumber += 1;
										}
										yytext = "";
										s = L_START;
										break;
									}
				return (IP_MatchToken(yytext));

			case L_NODE1:
				if (isalpha(c))
					break;
				Ungetc(c);
				yytext = yytext.substr(1, yytext.size() - 2);
				yylval.s = yytext;
				return (NODE1);

			case L_NODE2:
				if (isalpha(c))
					break;
				Ungetc(c);
				yytext = yytext.substr(1, yytext.size() - 2);
				yylval.s = yytext;
				return (NODE2);

			default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}
//------------------------------------------------------------------------
