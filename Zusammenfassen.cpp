#include "stdafx.h"
#include "Zusammenfassen.hpp"
#include "system.h"
#include <iomanip> 
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
	unsigned char max_anzahl = k_index + 1;// weil nur Komma gezaehlt wird.
	int index = 1;
	string ele0;
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
		if (Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] != "")// ueberprueft, ob zwischen den beiden Knoten schon Elementname eingetragen wurde.
		{
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] = "(" + Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] + "||" + rt->Element + ")";
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] = rt->Element;

		if (Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] != "")// Adjacenzmatrix ist symmetrisch, deswegen wird dasselbe Element nochmals in die transponierte Position eingetragen.
		{
			ele0 = Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1];
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] ="(" + Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1]  + "||" + rt->Element + ")";
			cout << "------------------------------------------------------------------------------------------------------" << endl; 
			cout << "Parallele Zusammenfassung " << endl;
			cout << " - An den Knoten (" + rt->NODE_1 + "," + rt->NODE_2 + ") ist das Element " << ele0 << " schon vorhanden" << endl;
			cout << " - Ergebbnis an den Knoten (" + rt->NODE_1 + "," + rt->NODE_2 + ") : " + Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] << endl;
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] = rt->Element;
		rt = rt->next;
	}
	max_read_node = index;
}

/**************************************************************************************************
* Funktion: Die Funktion traegt einen Knotennamen in die Nodetabelle ein. Gleichzeitig wird ihm das
*			Index zugeordnet. Wenn eingetragen werden kann, wird true zurueckgegeben.
*
* Argumente:
* string node : Knotename
* int index   : Zugehoeriges Index
*
* Rueckgabewert: true , wenn der Name eingetragen werden kann.
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
* Funktion: Es wird Dreieck-Stern Transformation durchgefuehrt. Die Funktion prueft, ob es zwischen drei
*			Knoten eine Schleife existiert. Zunaechst wird zwischen zwei Knoten untersucht, wenn ein Element
*			dort eingetragen wird, wird die erste Position(1) gemerkt und von dem zweiten Knoten(2) wird weiter gesucht, ob
*			dieses Knote Verbindung mit anderem Knote(3) hat. Falls zwischen Knoten (2) und (3) ein Element eingetragen wurde,
*			wird dann zwischen Knoten (3) und (1) den Eintrag ermittelt. Wenn Eintrag vorhanden ist, wird Flag "Dreieck_Vorhanden"
*			true gesetzt.
*				Wenn ein Flag "dreieck_vorhanden" true gesetzt wird, wird ein weiteres Knote hinzugefuegt und als "st(i)" genannt.
*			Zur besseren uebersicht wird ein Legend erstellt. Die "Darstellung"-Objekt speichert den Nenner und Zaehler.
*			Spaeter wird ihm eine Abkuerzung der Form "R{i,j}" zugeordnet. Zuletzt wird dann die ehemalige Elemente
*			zwischen drei Knoten geloescht.
* kein Argument
* Rueckgabewert: true, wenn Dreieck-Stern Transformation m�glich ist.
********************************************************************************************************************************/
bool zusammenfassen::dreieck2stern()
{
	bool dreieck_vorhanden = false;
	vector<char> stern_position(3);
	for (char i = 0; i <= k_index && !dreieck_vorhanden; i++)
	{
		stern_position[0] = i;
		if (i == node_table[sys_pointer.GND] - 1) // wird ueberspringt, falls das Knote ein GND ist.
			continue;
		for (char ii = 0; ii <= k_index && !dreieck_vorhanden; ii++)
		{
			if (ii == node_table[sys_pointer.GND] - 1)// wird ueberspringt, falls das Knote ein GND ist.
				continue;
			if (Adjacenzmatrix[i][ii] != "") // Zwischen (1)(2) Eintrag vorhanden?
			{
				stern_position[1] = ii;
				for (char i3 = 0; i3 <= k_index; i3++)
				{
					if (i3 == node_table[sys_pointer.GND] - 1)// wird ueberspringt, falls das Knote ein GND ist.
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
		for (char i = 0; i <= k_index; i++)// Adjacenzmatrix (x-dimension) um 1 vergroessert.
		{
			Adjacenzmatrix[i].push_back("");
		}
		k_index++;
		//------------------------------------------------------//
		// Wegen Symmetrie, wird Adjacenzmatrix (y-dimension) um 1 vergroessert.
		vector<string> init;
		init.assign(k_index + 1, "");
		Adjacenzmatrix.push_back(init);
		load_node_table("st" + to_string(k_index), k_index + 1);// Hier muss noch Index fuer st rein

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
		cout << endl;
		cout << "----------------------------------------------------------------------------------" << endl;
		cout << "     Dreieck-Stern Transformation an den Knoten:" << endl;
		cout << "     " + rev_node_table[stern_position[0] + 1] + ',' + rev_node_table[stern_position[1] + 1] + ',' + rev_node_table[stern_position[2] + 1] << endl;
		cout << "Ergebnis:" << endl;
		for (int i = 0; i < 3; i++)
		{
			cout << "Knoten: " << rev_node_table[stern_position[i] + 1] + ',' << rev_node_table[k_index + 1] << ": ";
			cout << Adjacenzmatrix[stern_position[i]][k_index] + "=";
			cout << legend[Adjacenzmatrix[stern_position[i] ][k_index ]].Zaehler + '/' << legend[Adjacenzmatrix[stern_position[i]][k_index]].Nenner << endl;
		}
	}

	return dreieck_vorhanden;
}
/*********************************************************************************************************************************
* Funktion: Die Funktion gibt die aktuelle Adjacenzmatrix aus.
* kein Argument
* Rueckgabewert: keine
********************************************************************************************************************************/
void zusammenfassen::print_matrix()
{
	for (int i = 0; i <= k_index + 1; i++) // print node-name
		cout << rev_node_table[i] << "              ";
	cout << endl;
	for (char i = 0; i <= k_index; i++)
	{
		if (i <= max_read_node)
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
/*********************************************************************************************************************************
* Funktion: Es wird serielle Zusammenfassung durchgefuehrt. Die Funktion prueft, ob in einer Knoten genau 2 Elementen vorhanden ist.
*			Wenn das der Fall ist , wird die ueblichen 2 Knoten untersucht.
*				
* kein Argument
* Rueckgabewert: true, wenn  serielle Zusammenfassung durchgefuehrt ist.
********************************************************************************************************************************/
bool zusammenfassen::seriell()
{
	bool seriell_vorhanden = false;
	string ele0;
	string ele1;
	string ele2;
	int Knoten_mitte, Knoten1, Knoten2, element_counter; 
	
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++) // durchlauf für jede Knoten in Adjacenzmatrix
	{
		element_counter = 0;
		// Prueft, ob die aktuelle Knoten GND,INPUT oder OUTPUT ist. 
		//Wenn das der Fall ist , wird nichts zusammengefasst und der zeiger geht zur naechsten Knoten
		if (rev_node_table[lp + 1] == sys_pointer.INPUT) // ueberprueft, ob die Knoten INPUT ist.
		{
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.OUTPUT) // ueberprueft, ob die Knoten OUTPUT ist.
		{
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.GND)// ueberprueft, ob die Knoten GND ist.
		{
			continue;
		}
		//Es wird hier Elemet gezaehlt und Knoten gemerkt.
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++) 
		{
			Knoten_mitte = lp;
			if (Adjacenzmatrix[lp][fp] != "")
			{
				if (element_counter == 0)	//erste Element, die mit der Knoten0 und Knoten1 verbunden ist.
				{
					Knoten1 = fp;			//erste Knoten merken, die durch die Element mit der Knoten0 verbunden ist.
				}
				Knoten2 = fp;				//letzte Knoten merken, die durch die Element mit der Knoten0 verbunden ist.
				element_counter++;			//letzte Element, die mit der Knoten0 und Knoten2 verbunden ist.
				;
			}
		}
		if (element_counter == 2) // Prueft, ob an der aktuellen Knoten gerade 2 Elementen vorhanden sind.
		{
			seriell_vorhanden = true; // Rueckgabewert wird gesetzt.
			cout << "------------------------------------------------------------------------------------------------------" << endl;
			cout << "Sreielle Zusammenfassung" << endl;
			cout << " - An der Knoten : (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten_mitte + 1] << "," << rev_node_table[Knoten2 + 1] << ")" << endl;
			ele1 = Adjacenzmatrix[Knoten_mitte][Knoten1];
			ele2 = Adjacenzmatrix[Knoten_mitte][Knoten2];
			ele0 = Adjacenzmatrix[Knoten1][Knoten2];
			if (Adjacenzmatrix[Knoten1][Knoten2] == "") // Prueft, ob bereits ein Element zwischen der Knoten1 und Knoten2  vorhanden ist.
			{
				Adjacenzmatrix[Knoten1][Knoten2] = ele1 + '+' + ele2; 
				cout << " - Ergebnis an den Knoten (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << ") : " << Adjacenzmatrix[Knoten1][Knoten2] << endl;
			}
			else
			{
				Adjacenzmatrix[Knoten1][Knoten2] = " ((" + ele1 + '+' + ele2 + ")||" + ele0 + ") "; //Parallele Zusammenfassung wird benoetigt
				cout << " - An den Knoten (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << ") ist das Element " << ele0 << " schon vorhanden " << endl <<
					"   => Parallel zusammenfassung" << endl <<
					"      Ergebnis an den Knoten (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << ") : " << Adjacenzmatrix[Knoten1][Knoten2] << endl;
			}
			Adjacenzmatrix[Knoten2][Knoten1] = Adjacenzmatrix[Knoten1][Knoten2]; // Symmetrie
			// alte elemente werden geloescht
			Adjacenzmatrix[Knoten_mitte][Knoten1] = "";
			Adjacenzmatrix[Knoten_mitte][Knoten2] = "";
			Adjacenzmatrix[Knoten1][Knoten_mitte] = "";
			Adjacenzmatrix[Knoten2][Knoten_mitte] = "";
			
		}
	}
	return seriell_vorhanden;
}

/*********************************************************************************************************************************
* Funktion: Es wird Stern-Dreieck Transformation durchgefuehrt. Die Funktion prueft, ob es an einer Knoten
*			eine Sternschaltung existiert (genau 3 Elementen vorhanden). Zu naechst wird die mittlere Knoten
*			untersucht und gemwerkt. Dir uebliche 3 Knoten werden weiterhin gemerkt.
*				Die "Darstellung"-Objekt speichert den Nenner und Zaehler.
*			Spareer wird ihm eine Abkuerzung der Form "R{i,j}" zugeordnet. Zuletzt wird dann die ehemalige Elemente
*			in der Sternschaltung geloescht.
* kein Argument
* Rueckgabewert: true, wenn Stern-Dreieck Transformation moeglich ist.
* Adjacenzmatrix wird überschrieben
********************************************************************************************************************************/
bool zusammenfassen::stern2dreieck()
{
	bool stern_vorhanden = false;
	string ele0;
	string ele1;
	string ele2;
	string ele3;
	int Knoten0, Knoten_a, Knoten_b, Knoten_c, element_counter; //Knoten0 ist die mettlere Knoten in der Stern
	// Prueft, ob die aktuelle Knoten GND,INPUT oder OUTPUT ist. 
	//Wenn das der Fall ist ,wird michts unternommen und der zeiger geht zur naechsten Knoten
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++)
	{
		element_counter = 0;
		if (rev_node_table[lp + 1] == sys_pointer.INPUT)// ueberprueft, ob die Knoten INPUT ist.
		{
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.OUTPUT)// ueberprueft, ob die Knoten OUTPUT ist.
		{
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.GND)// ueberprueft, ob die Knoten GND ist.
		{
			continue;
		}
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++)
		{
			Knoten0 = lp;
			if (Adjacenzmatrix[lp][fp] != "") //ueberprueft, ob ein Element mit der aktuellen Knoten verbunden ist. (ja = //Anzahl der Elemente erhoehen)
			{
				if (element_counter == 0) // erstes Element merken
				{
					Knoten_a = fp;	//knoten  a merken
				}
				else
					if (element_counter == 1)
					{
						Knoten_b = fp; //knoten b merken
					}
					else
						Knoten_c = fp; //knoten c merken
				element_counter++; //Anzahl der Elemente erhoehen
			}
		}
		if (element_counter == 3) // Prueft, ob die Bedingung fuer die Transformation erfuellt wird.
		{
			stern_vorhanden = true; //Rueckgabewert setzen
			cout << "------------------------------------------------------------------------------------------------------" << endl;
			cout << "Stern-Dreieck-Umwandlung" << endl;
			cout << " - An der Knoten : (" << rev_node_table[Knoten0 + 1] << "," << rev_node_table[Knoten_a + 1] << "," << rev_node_table[Knoten_b + 1] << "," << rev_node_table[Knoten_c + 1] << ")" << endl;
			//neue Element einfuegen
			insert_s2d(Knoten0, Knoten_a, Knoten_b, Knoten_c); 
			insert_s2d(Knoten0, Knoten_c, Knoten_a, Knoten_b);
			insert_s2d(Knoten0, Knoten_b, Knoten_c, Knoten_a);
			//ehemalige Elemente werden geloescht
			Adjacenzmatrix[Knoten0][Knoten_a] = "";
			Adjacenzmatrix[Knoten0][Knoten_b] = "";
			Adjacenzmatrix[Knoten0][Knoten_c] = "";
			Adjacenzmatrix[Knoten_a][Knoten0] = "";
			Adjacenzmatrix[Knoten_b][Knoten0] = "";
			Adjacenzmatrix[Knoten_c][Knoten0] = "";
		}
	}
	return stern_vorhanden;
}


/*********************************************************************************************************************************
* Funktion: 
*		zwischen pina und pinb wird ein neues Elemet hunzugefuegt.
*		Das Ergebnis wird in der Form R{pina,pinb} abgekuerzt.
*		Es wird ein Objekt "Darstellung" erzeugt, um den Nenner und Zaehler zuspeichen.
*		Das neue Element wird in Adlacenzmatrix eingefuegt.
*		Die "Darstellung"-Objekt speichert den Nenner und Zaehler.
*
* Argument : alle 4 Knoten muss in der Funktion bekannt sein, um alle Widerstaende erhalten zu koennen.
*	- mid_node : mittlere Knoten in der Sternschaltung
*	- pina : zwischen pina und pinb wird ein Resultat erzeugt.
*	- pinb
*	- pinc
*
* Rueckgabewert: keine.
* Adjacenzmatrix wird überschrieben
********************************************************************************************************************************/
void zusammenfassen::insert_s2d(int mid_node, int pina, int pinb, int pinc) {
	Darstellung B;
	string ele0;
	ele0 = Adjacenzmatrix[pina][pinb];
	B.Zaehler = Adjacenzmatrix[mid_node][pina] + "*" + Adjacenzmatrix[mid_node][pinb] + "+"
		+ Adjacenzmatrix[mid_node][pinb] + "*" + Adjacenzmatrix[mid_node][pinc] + "+"
		+ Adjacenzmatrix[mid_node][pinc] + "*" + Adjacenzmatrix[mid_node][pina];
	B.Nenner = Adjacenzmatrix[mid_node][pinc];
	legend["R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}'] = B;

	if (Adjacenzmatrix[pina][pinb] == "") // Prueft, ob bereits ein Element zwischen der Knoten pina und Knoten pinb  vorhanden ist.
	{
		Adjacenzmatrix[pina][pinb] = "R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}'; //Abkuerzung in der Adjacenzmatrix gespeichert
		cout << " - Ergebbnis an den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") : " << Adjacenzmatrix[pina][pinb] << endl;
		cout << "     R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}'  << " = ("<< B.Zaehler + ")/(" << B.Nenner << ')'<<endl;
	}
	else
	{
		Adjacenzmatrix[pina][pinb] = "(R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}' + "||" + Adjacenzmatrix[pina][pinb] + ")";  //Abkuerzung in der Adjacenzmatrix gespeichert und parallel mit dem vorhandenen Element geschaltet
		cout << " - An den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") ist das Element " << ele0 << " schon vorhanden" <<endl;
		cout << "   => Parallele Zusammenfassung " << endl;
		cout << "      Ergebbnis an den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") : " << Adjacenzmatrix[pina][pinb] << endl;
		cout << "        R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}' << " = (" << B.Zaehler + ")/(" << B.Nenner << ')' << endl;
	}
	Adjacenzmatrix[pinb][pina] = Adjacenzmatrix[pina][pinb]; // Symmetrie

}
/*********************************************************************************************************************************
* Funktion: Durchfuehrung aller Art der Zusammenfassungen das Ergebnis wird in Console gezeigt.
*		Am Ende wird das Resultat der Uebertragungsfunktion und die Liste der Ankuerzung ausgegeben.
*
* Rueckgabewert: keine.
* Adjacenzmatrix wird überschrieben
********************************************************************************************************************************/
void zusammenfassen::operator()(komponent * last_RLC)
{
	Initialize_Adjacenzmatrix(last_RLC);
	while (seriell() || stern2dreieck() || dreieck2stern())
	{
		;
	//	print_Adj();
	}
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	H_Nenner = Adjacenzmatrix[node_table[sys_pointer.INPUT] - 1][node_table[sys_pointer.OUTPUT] - 1] + " + " + Adjacenzmatrix[node_table[sys_pointer.OUTPUT] - 1][node_table[sys_pointer.GND] - 1];
	H_Zaehler = Adjacenzmatrix[node_table[sys_pointer.OUTPUT] - 1][node_table[sys_pointer.GND] - 1];
	H_R_in = Adjacenzmatrix[node_table[sys_pointer.INPUT] - 1][node_table[sys_pointer.OUTPUT] - 1];
	cout << "Uebertragungsfunktion" << endl;
	cout << "Zaehler: " << H_Zaehler << endl;
	cout << "Nenner: " << H_Nenner << endl << endl;
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	print_legend();
}
/*********************************************************************************************************************************
* Funktion: Die Adjacenzmatrix wird ausgegeben
*
* Rueckgabewert: keine.
* nach dem Aufruf des Funktion wird kein Variabel ueberschriben
********************************************************************************************************************************/
void zusammenfassen::print_Adj()
{
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	cout << "Adjacenzmatrix " << endl;
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++)
	{
		cout << rev_node_table[lp + 1] << "  ";
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
}

void zusammenfassen::clear_all(komponent * last_RLC)
{
	komponent* rt = last_RLC;
	komponent* fi;
	while (rt != NULL)
	{
		fi = rt->next;
		delete rt;
		rt = fi;
	}
}

zusammenfassen::~zusammenfassen()
{
	node_table.erase(node_table.begin(), node_table.end());
	rev_node_table.erase(rev_node_table.begin(), rev_node_table.end());
	legend.erase(legend.begin(), legend.end());
	for(int i = 0 ; i < k_index ; i++)
		Adjacenzmatrix[i].erase(Adjacenzmatrix[i].begin(), Adjacenzmatrix[i].end());
	Adjacenzmatrix.erase(Adjacenzmatrix.begin(), Adjacenzmatrix.end());
	H_Nenner.clear();
	H_Zaehler.clear();
	H_R_in.clear();
	cout << "delete complete" << endl;
}

/*********************************************************************************************************************************
* Funktion: Legende wird ausgegeben
*
* Rueckgabewert: keine.
* nach dem Aufruf des Funktion wird kein Variabel ueberschriben
********************************************************************************************************************************/
void zusammenfassen::print_legend()
{
	int kk = 0;
	cout << "Legende" << endl;
	for (auto& x : legend)
	{
		cout << "          " << x.second.Zaehler << endl;
		kk = x.second.Nenner.size();
		if (x.second.Nenner.size() < x.second.Zaehler.size())
		{
			kk = x.second.Zaehler.size();
		}
		cout << x.first << " =  ";
		for (int nn = 0; nn < kk; nn++)
		{
			cout << "-";
		}
		cout << endl;
		cout << "          ";
		for (int nn = 0; nn < kk / 2 - x.second.Nenner.size() / 2; nn++)
		{
			cout << " ";
		}
		cout << x.second.Nenner << endl << endl;
	}
}