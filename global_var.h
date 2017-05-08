#define	Getc(s)			getc(s)
#define	Ungetc(c)		ungetc(c,IP_Input)

#include "Komponent.h"

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


