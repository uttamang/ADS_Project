#pragma once
#ifndef _KOMPONENT_
#define _KOMPONENT_


#include <iostream>
#include <string>

using namespace std;

class komponent
{
public:
	string NODE_1 = ""; // 1.te Zeile in Kantentabelle	
	string NODE_2 = "";	// 2.te Zeile in Kantentabelle
	string Element = ""; // Kanten (Spalte in Kantentabelle)
	komponent* next = NULL; 
	int index = 0;
};



#endif // _KOMPONENT_