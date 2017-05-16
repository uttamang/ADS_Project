#pragma once
#include <iostream>
#include <string>
#include "Komponent.h"
using namespace std;

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
	Knotenlist* next_knoten;
	elementlist* Ele_List;
	Knotenlist* Knotenlist::setKnoten(komponent* last_RLC);
	void display_node(Knotenlist* last_node);
};
