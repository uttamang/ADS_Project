#pragma once

#include <vector>
#include <string>
#include <map>
#include "Komponent.h"

using namespace std;
typedef vector<vector<string> >  Ad_Mat;

class zusammenfassen
{
public:
	Ad_Mat Adjacenzmatrix;
	map<string, int> node_table;
	map<int, string> rev_node_table;

	void Initialize_Adjacenzmatrix(komponent* last_RLC);
	bool seriell();
	bool load_node_table(string node, int index);
	bool dreieck2stern();
	bool stern2dreieck();
	void operator ()(komponent* last_RLC);
};