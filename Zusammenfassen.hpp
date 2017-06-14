#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Komponent.h"

using namespace std;
typedef vector<vector<string> >  Ad_Mat;

class Darstellung
{
public:
	string Nenner;
	string Zaehler;
};

class zusammenfassen
{
public:
	Ad_Mat Adjacenzmatrix;
	map<string, int> node_table;
	map<int, string> rev_node_table;
	map<string, Darstellung> legend;
	int max_read_node = 0;
	int stern_index = 0;
	string H_Nenner;
	string H_Zaehler;

	void Initialize_Adjacenzmatrix(komponent* last_RLC);
	bool seriell();
	bool load_node_table(string node, int index);
	bool dreieck2stern();
	bool stern2dreieck();
	void operator ()(komponent* last_RLC);
	void print_matrix();
	void insert_s2d(int node, int pina, int pinb, int pinc);
	void print_Adj();
};
template <typename T>
string NumberToString(T number)
{
	ostringstream ss;
	ss << number;
	return ss.c_str();
}