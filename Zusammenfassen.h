#pragma once

#include <vector>
#include <string>
#include <map>
#include "Komponent.h"
#include "system.h"

using namespace std;
typedef vector<vector<string> >  Ad_Mat;
//extern system_1* sys_pointer;


class zusammenfassen
{
public:
	Ad_Mat Adjacenzmatrix;
	map<string, int> node_table;
	map<int, string> rev_node_table;

	void Initialize_Adjacenzmatrix(komponent* last_RLC);
	void print_Adj();
	bool seriell();
	bool load_node_table(string node, int index);
	bool dreieck2stern();
	bool stern2dreieck();
	void zusammenfassen::insert_s2d(int node, int pina, int pinb, int pinc);
	void operator ()(komponent* last_RLC);
};