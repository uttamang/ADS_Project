#pragma once
#include <iostream>
#include <string>
#include "Komponent.h"
using namespace std;
extern komponent* Last_RLC;

struct elementlist {							
	string ELE_NAME;								
	elementlist* next;
};
class Knotenlist
{
public:
	string NODE = "";
	Knotenlist* next_knoten;
	elementlist* Ele_List;
	Knotenlist* Knotenlist::setKnoten(komponent* last_RLC);
};
