#pragma once
#ifndef _NETS_
#define _NETS_

#include "Komponent.h"
#include "knotenlist.h"

class vereinfachen:public Knotenlist
{
public:
	komponent* first_matched; // zu zusammenfassendes Element 
	komponent* Matched;  // entsprechendes Element mit passenden Knoten

	vereinfachen():
		first_matched(NULL),Matched(NULL){}



	/*Konstruktor der Klasse "vereinfachen". Die Funktion ruft die beiden Funktion "parallel" und "seriell"*/
	void operator ()(komponent* last_RLC);

	bool parallel(komponent* last_RLC); // in struction zusammenfassung
	bool seriell(komponent* last_RLC);	
};
#endif // _NETS_