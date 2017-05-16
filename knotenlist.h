#pragma once
#include <iostream>
#include <string>
#include "Komponent.h"
#include "system.h"
using namespace std;
//extern komponent* last_RLC;
//extern Knotenlist* last_node;
class elementlist {	
public:
	string ELE_NAME;								
	elementlist* next;
	void display_elementlist(elementlist* last_element);
};
class Knotenlist
{
public:
	string NODE = "";
	Knotenlist* next;
	elementlist* Ele_List;
	Knotenlist* Knotenlist::setKnoten(komponent* last_RLC);
	Knotenlist* Knotenlist::KnotenSortieren(Knotenlist* last_node, system_1* sys_pointer);
	void display_node(Knotenlist* last_node);
};
