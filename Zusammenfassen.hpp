#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Komponent.h"

using namespace std;
typedef vector<vector<string> >  Ad_Mat;

/**************************************************************************************************
*	Klasse Darstellung: Hilfklasse zur Darstellung des Legends in GDE
***************************************************************************************************/
class Darstellung
{
public:
	string Nenner;
	string Zaehler;
};

/****************************************************************************************************
* Klasse zusammenfassen: Zur Zusammenfassung und Vereinfachung der Schaltung
****************************************************************************************************/
class zusammenfassen
{
public:
	Ad_Mat Adjacenzmatrix;
	map<string, int> node_table; // Knotentabelle
	map<int, string> rev_node_table; // umgekehrte Zuordnung von int und string der Knotentabelle
	map<string, Darstellung> legend; // Legendetabelle
	int max_read_node = 0;// maximale eingelesene Knoten
	int stern_index = 0; // Anzahl der Dreieck-Stern Transformation
	string H_Nenner; // Hauptnenner
	string H_Zaehler;// Hauptzaehler
	string H_R_in;
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