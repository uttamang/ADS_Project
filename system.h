#pragma once
#ifndef _SYSTEM_1_
#define _SYSTEM_1_


#include <iostream>
#include <string>

using namespace std;
/******************************************************************************************************
*	Klasse system_1: Die Knotenn ,die sich um Eingang, Ausgang und Masse verhalten, werden gepeichert.
******************************************************************************************************/
class system_1
{
public:
	string INPUT = ""; 	
	string OUTPUT = "";	
	string GND = ""; 
	int index = 0;
};



#endif // _SYSTEM_1_