#include "stdafx.h"
#include "Zusammenfassen.hpp"
#include "system.h"

extern unsigned char k_index; // Anzahl der gesamten Knoten
extern system_1 sys_pointer; // Globale Variable, die Input-, Output-, GND-Knoten enthalten

/***********************************************************************************************
* Funktion: Die Funktion initialisiert die Adjacenzmatrix mit Hilfe der Information in der Liste
*			"last_RLC". 
*			Zunächst wird die nxn Adjacenzmatrix mit "" initialisiert. n ist die Anzahl der gesamten
*			Knoten. Die Liste "last_RLC" wird von hinten an untersucht.
*			Es wird überprüft, ob zwischen zwei Knoten schon Elemente vorhanden sind. Wenn es der
*			Fall ist, werden die beiden Elemente mit der Operation "||" verknüpft.
* Argumente:
*
* komponent* last_RLC: Zeiger auf das letzte Element der Komponentlist.
*
* Rückgabewert: -
***********************************************************************************************/

void zusammenfassen::Initialize_Adjacenzmatrix(komponent * last_RLC)
{
	unsigned char max_anzahl = k_index +1 ;// weil nur Komma gezählt wird.
	int index = 1;
	komponent* rt = last_RLC;
	vector<string> init; // Init the first element of Ad_Mat with empty string
	init.assign(max_anzahl, "");
	Adjacenzmatrix.assign(max_anzahl, init);
	while (rt != NULL)
	{
		if (load_node_table(rt->NODE_1, index))//  Die Knote wird in einer Map-Tabelle eingetragen und dem Index zugeordnet.
		{									   // Wenn eingetragen, wird Index inkrementiert.
			index++;
		}
		if (load_node_table(rt->NODE_2, index))
		{
			index++;
		}
		if (Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] != "")// Überprüft, ob zwischen den beiden Knoten schon Elementname eingetragen wurde.
		{
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] += "||" + rt->Element;
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] = rt->Element;

		if (Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] != "")// Adjacenzmatrix ist symmetrisch, deswegen wird dasselbe Element nochmals in die transponierte Position eingetragen.
		{
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] += "||" + rt->Element;
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] = rt->Element;
		rt = rt->next;
	}
	max_read_node = index;
}

/**************************************************************************************************
* Funktion: Die Funktion trägt einen Knotennamen in die Nodetabelle ein. Gleichzeitig wird ihm das 
*			Index zugeordnet. Wenn eingetragen werden kann, wird true zurückgegeben.
*
* Argumente: 
* string node : Knotename
* int index   : Zugehöriges Index
*
* Rückgabewert: true , wenn der Name eingetragen werden kann.
*/
bool zusammenfassen::load_node_table(string node, int index)
{
	if (node_table[node] == NULL)
	{
		node_table[node] = index;
		rev_node_table[index] = node;
		return true;
	}
	else
		return false;
}

/*********************************************************************************************************************************
* Funktion: Es wird Dreieck-Stern Transformation durchgeführt. Die Funktion prüft, ob es zwischen drei 
*			Knoten eine Schleife existiert. Zunächst wird zwischen zwei Knoten untersucht, wenn ein Element
*			dort eingetragen wird, wird die erste Position(1) gemerkt und von dem zweiten Knoten(2) wird weiter gesucht, ob
*			dieses Knote Verbindung mit anderem Knote(3) hat. Falls zwischen Knoten (2) und (3) ein Element eingetragen wurde,
*			wird dann zwischen Knoten (3) und (1) den Eintrag ermittelt. Wenn Eintrag vorhanden ist, wird Flag "Dreieck_Vorhanden" 
*			true gesetzt.
*				Wenn ein Flag "dreieck_vorhanden" true gesetzt wird, wird ein weiteres Knote hinzugefügt und als "st(i)" genannt.
*			Zur besseren Übersicht wird ein Legend erstellt. Die "Darstellung"-Objekt speichert den Nenner und Zähler. 
*			Später wird ihm eine Abkürzung der Form "R{i,j}" zugeordnet. Zuletzt wird dann die ehemalige Elemente
*			zwischen drei Knoten gelöscht.
* kein Argument
* Rückgabewert: true, wenn Dreieck-Stern Transformation möglich ist.
********************************************************************************************************************************/
bool zusammenfassen::dreieck2stern()
{
	bool dreieck_vorhanden = false;
	vector<char> stern_position(3);
	for (char i = 0; i <= k_index && !dreieck_vorhanden; i++)
	{
		stern_position[0] = i;
		if ( i == node_table[sys_pointer.GND] - 1) // wird überspringt, falls das Knote ein GND ist.
			continue;
		for (char ii = 0; ii <= k_index && !dreieck_vorhanden; ii++)
		{
			if ( ii == node_table[sys_pointer.GND] - 1)// wird überspringt, falls das Knote ein GND ist.
				continue;
			if (Adjacenzmatrix[i][ii] != "") // Zwischen (1)(2) Eintrag vorhanden?
			{
				stern_position[1] = ii;
				for (char i3 = 0; i3 <= k_index; i3++)
				{
					if (i3 == node_table[sys_pointer.GND] - 1)// wird überspringt, falls das Knote ein GND ist.
						continue;
					if (Adjacenzmatrix[ii][i3] != "")// Zwischen (2)(3) Eintrag vorhanden?
					{
						if (Adjacenzmatrix[i3][i] != "")// Zwischen (3)(1) Eintrag vorhanden?
						{
							stern_position[2] = i3;
							dreieck_vorhanden = true;
							break;
						}
					}
				}
			}
		}
	}
	if (dreieck_vorhanden)
	{
		for (char i = 0; i <= k_index; i++)// Adjacenzmatrix (x-dimension) um 1 vergrößert.
		{
			Adjacenzmatrix[i].push_back("");
		}
		k_index++;
		//------------------------------------------------------//
		// Wegen Symmetrie, wird Adjacenzmatrix (y-dimension) um 1 vergrößert.
		vector<string> init;
		init.assign(k_index + 1, ""); 
		Adjacenzmatrix.push_back(init);
		load_node_table("st"+ to_string(k_index), k_index+1);// Hier muss noch Index für st rein

		//-----------------------------------------------------//
		Darstellung B;
		char i2 = 1, i3 = 2;
		
		for (int i = 0; i < 3; i++)
		{
			B.Zaehler = Adjacenzmatrix[stern_position[i]][stern_position[i2]] + '*' + Adjacenzmatrix[stern_position[i]][stern_position[i3]];
			B.Nenner = Adjacenzmatrix[stern_position[0]][stern_position[1]] + "+" + Adjacenzmatrix[stern_position[0]][stern_position[2]] + "+" + Adjacenzmatrix[stern_position[1]][stern_position[2]];
			legend["R{" + rev_node_table[stern_position[i] + 1] + ", " + rev_node_table[k_index + 1] + "}"] = B;

			Adjacenzmatrix[stern_position[i]][k_index] = "R{" + rev_node_table[stern_position[i] + 1] + ", " + rev_node_table[k_index + 1] + "}";
			Adjacenzmatrix[k_index][stern_position[i]] = "R{" + rev_node_table[stern_position[i] + 1] + ", " + rev_node_table[k_index + 1] + "}";
			i2++;
			i3++;
			if (i2 > 2)
				i2 = 0;
			if (i3 > 2)
				i3 = 0;
		}
		for (int i = 0; i < 3; i++) // delete node
		{
			Adjacenzmatrix[stern_position[i]][stern_position[0]] = "";
			Adjacenzmatrix[stern_position[i]][stern_position[1]] = "";
			Adjacenzmatrix[stern_position[i]][stern_position[2]] = "";
		}
	}

	return dreieck_vorhanden;
}

void zusammenfassen::print_matrix()
{
	for (int i = 0; i <= k_index + 1; i++) // print node-name
		cout << rev_node_table[i] << "              ";
	cout << endl;
	for (char i = 0; i <= k_index; i++)
	{
		if(i <= max_read_node)
			cout << rev_node_table[i + 1] << "              ";
		for (char ii = 0; ii <= k_index; ii++)
		{
			if (Adjacenzmatrix[i][ii] == "")
				cout << "              ";
			else
				cout << Adjacenzmatrix[i][ii] << "             ";
		}
		cout << endl;
	}
}
 	
bool zusammenfassen::seriell()
{
	bool rt = false;
	string ele0;
	string ele1;
	string ele2;
	int node, pin1, pin2, seriell_check;
	cout << endl << "Serielle Zusammenfassung" << endl;
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++)
	{
		seriell_check = 0;
		if (rev_node_table[lp + 1] == sys_pointer.INPUT)
		{
			//cout << "Seriell gefunden aber die Knoten " <<sys_pointer.INPUT << " war INPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.OUTPUT)
		{
			//cout << "Seriell gefunden aber die Knoten " <<sys_pointer.OUTPUT << " war OUTPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.GND)
		{
			//cout << "Seriell gefunden aber die Knoten " <<sys_pointer.GND << " war GND" << endl;
			continue;
		}
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++)
		{
			node = lp;
			if (Adjacenzmatrix[lp][fp] != "")
			{
				if (seriell_check == 0)
				{
					pin1 = fp;
				}
				pin2 = fp;
				seriell_check++;
				;
			}
		}
		if (seriell_check == 2)
		{
			rt = true;
			cout << "Seriell gefunden !!! Knoten : " << rev_node_table[node+1] << " zwischen " << rev_node_table[pin1+1] << " und " << rev_node_table[pin2+1] << endl;
			ele1 = Adjacenzmatrix[node][pin1];
			ele2 = Adjacenzmatrix[node][pin2];
			ele0 = Adjacenzmatrix[pin1][pin2];
			if (Adjacenzmatrix[pin1][pin2] == "")
			{
				Adjacenzmatrix[pin1][pin2] = ele1 + '+' + ele2;
			}
			else
			{
				Adjacenzmatrix[pin1][pin2] = " ((" + ele1 + '+' + ele2 + ")||" + ele0 +") ";
			}
			Adjacenzmatrix[pin2][pin1] = Adjacenzmatrix[pin1][pin2]; // Symmetrie
			// alte elemente werden geloescht
			Adjacenzmatrix[node][pin1] = "";
			Adjacenzmatrix[node][pin2] = "";
			Adjacenzmatrix[pin1][node] = "";
			Adjacenzmatrix[pin2][node] = "";
		}
	}
	return rt;
}
bool zusammenfassen::stern2dreieck()
{
	bool rt = false;
	string ele0;
	string ele1;
	string ele2;
	string ele3;
	int node, pin1, pin2,pin3, seriell_check;
	cout << endl << "Stern-Dreieck Umwaldlung Zusammenfassung" << endl;
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++)
	{
		seriell_check = 0;
		if (rev_node_table[lp + 1] ==sys_pointer.INPUT)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.INPUT << " war INPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] ==sys_pointer.OUTPUT)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.OUTPUT << " war OUTPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] ==sys_pointer.GND)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.GND << " war GND" << endl;
			continue;
		}
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++)
		{
			node = lp;
			if (Adjacenzmatrix[lp][fp] != "")
			{
				if (seriell_check == 0)
				{
					pin1 = fp;
				}else
					if (seriell_check == 1)
					{
						pin2 = fp;
					}
					else
				pin3 = fp;
				seriell_check++;
				;
			}
		}
		if (seriell_check == 3)
		{
			rt = true;
			cout << "Stern gefunden !!! Knoten : " << rev_node_table[node + 1] << " zwischen " << rev_node_table[pin1 + 1] << "," << rev_node_table[pin2 + 1] << " und " << rev_node_table[pin3 + 1] << endl;
			insert_s2d(node, pin1, pin2,pin3);
			insert_s2d(node, pin3, pin1,pin2);
			insert_s2d(node, pin2, pin3,pin1);
			// alte elemente werden geloescht
			Adjacenzmatrix[node][pin1] = "";
			Adjacenzmatrix[node][pin2] = "";
			Adjacenzmatrix[node][pin3] = "";
			Adjacenzmatrix[pin1][node] = "";
			Adjacenzmatrix[pin2][node] = "";
			Adjacenzmatrix[pin3][node] = "";
		}
	}
	return rt;
}
void zusammenfassen::insert_s2d(int node,int pina,int pinb, int pinc) {
	Darstellung B;

	B.Zaehler = Adjacenzmatrix[node][pina] + "*" + Adjacenzmatrix[node][pinb] + "+"
			+ Adjacenzmatrix[node][pinb] + "*" + Adjacenzmatrix[node][pinc] + "+"
			+ Adjacenzmatrix[node][pinc] + "*" + Adjacenzmatrix[node][pina];
	B.Nenner = Adjacenzmatrix[node][pinc];
	legend["R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}'] = B;
	
	if (Adjacenzmatrix[pina][pinb] == "")
	{
		Adjacenzmatrix[pina][pinb] = "R{" + rev_node_table[pina+1] + ',' + rev_node_table[pinb+1]+ '}';
	}
	else
	{
		Adjacenzmatrix[pina][pinb] = "R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1]+ '}' +"||" + Adjacenzmatrix[pina][pinb]; ;
	}
	Adjacenzmatrix[pinb][pina] = Adjacenzmatrix[pina][pinb]; // Symmetrie
	
}
void zusammenfassen::operator()(komponent * last_RLC)
{
	Initialize_Adjacenzmatrix(last_RLC);
	while (seriell()||stern2dreieck() ||dreieck2stern())
	{
		print_Adj();
	}
	cout << endl << "Keine Aenderung vorgenemmen => fertig !" << endl;
	print_Adj();
	H_Nenner = Adjacenzmatrix[node_table[sys_pointer.INPUT] - 1][node_table[sys_pointer.OUTPUT] - 1] + " + " + Adjacenzmatrix[node_table[sys_pointer.OUTPUT] - 1][node_table[sys_pointer.GND] - 1];
	H_Zaehler = Adjacenzmatrix[node_table[sys_pointer.OUTPUT] - 1][node_table[sys_pointer.GND] - 1];
	cout << "Zaehler: " << H_Zaehler << endl;
	cout << "Nenner: " << H_Nenner << endl;
}
void zusammenfassen::print_Adj()
{
	cout << "Adjacenzmatrix " << endl;
	for (int lp = 0 ; lp < Adjacenzmatrix.size();lp++)
	{
		cout << rev_node_table[lp+1] << "  ";
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++)
		{			
			if (Adjacenzmatrix[lp][fp] != "")
			{
				cout << Adjacenzmatrix[lp][fp] << "  ";
			}
			else
				cout << "--  ";
		}
		cout << endl;
	}
	for (auto& x : legend) {
		cout << x.first << ": " << x.second.Zaehler << ": "<< x.second.Nenner << endl;
	}
}

