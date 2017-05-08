#pragma once
#ifndef _NETS_
#define _NETS_

#include "Komponent.h"

class zusammenfassung
{
public:
	komponent* first_matched; // zu zusammenfassendes Element 
	komponent* Matched;  // entsprechendes Element mit passenden Knoten

	zusammenfassung():
		first_matched(NULL),Matched(NULL){}

	zusammenfassung(string from_knoten, string to_knoten);

	int parallel();
	int serie();
};
#endif // _NETS_