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
			cout << "--------------------------------------------------------------------------" << endl;
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
				Adjacenzmatrix[Knoten1][Knoten2] = " ((" + ele1 + '+' + ele2 + ")||" + ele0 + ") "; //pararelle Zusammenfassung wird benoetigt
				cout << " - An den Knoten (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << ") ist das Element " << ele0 << " schon vorhanden " << endl <<
					"   => Parallel zusammenfassung" << endl <<
					"      neues Ergebnis an den Knoten (" << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << ") : " << Adjacenzmatrix[Knoten1][Knoten2] << endl;
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
********************************************************************************************************************************/
bool zusammenfassen::stern2dreieck()
{
	bool stern_vorhanden = false;
	string ele0;
	string ele1;
	string ele2;
	string ele3;
	int Knoten0, Knoten1, Knoten2, Knoten3, element_counter;
	// Prueft, ob die aktuelle Knoten GND,INPUT oder OUTPUT ist. 
	//Wenn das der Fall ist ,wird michts unternommen und der zeiger geht zur naechsten Knoten
	for (int lp = 0; lp < Adjacenzmatrix.size(); lp++)
	{
		element_counter = 0;
		if (rev_node_table[lp + 1] == sys_pointer.INPUT)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.INPUT << " war INPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.OUTPUT)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.OUTPUT << " war OUTPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer.GND)
		{
			//cout << "Stern gefunden aber die Knoten " <<sys_pointer.GND << " war GND" << endl;
			continue;
		}
		for (int fp = 0; fp < Adjacenzmatrix[lp].size(); fp++)
		{
			Knoten0 = lp;
			if (Adjacenzmatrix[lp][fp] != "")
			{
				if (element_counter == 0)
				{
					Knoten1 = fp;
				}
				else
					if (element_counter == 1)
					{
						Knoten2 = fp;
					}
					else
						Knoten3 = fp;
				element_counter++;
				;
			}
		}
		if (element_counter == 3) // Prueft, ob die Bedingung fuer die Transformation erfuellt wird.
		{
			stern_vorhanden = true;
			cout << "--------------------------------------------------------------------------" << endl;
			cout << "Stern-Dreieck-Umwandlung" << endl;
			cout << " - An der Knoten : (" << rev_node_table[Knoten0 + 1] << "," << rev_node_table[Knoten1 + 1] << "," << rev_node_table[Knoten2 + 1] << "," << rev_node_table[Knoten3 + 1] << ")" << endl;
			//neue Element einfuegen
			insert_s2d(Knoten0, Knoten1, Knoten2, Knoten3);
			insert_s2d(Knoten0, Knoten3, Knoten1, Knoten2);
			insert_s2d(Knoten0, Knoten2, Knoten3, Knoten1);
			//ehemalige Elemente werden geloescht
			Adjacenzmatrix[Knoten0][Knoten1] = "";
			Adjacenzmatrix[Knoten0][Knoten2] = "";
			Adjacenzmatrix[Knoten0][Knoten3] = "";
			Adjacenzmatrix[Knoten1][Knoten0] = "";
			Adjacenzmatrix[Knoten2][Knoten0] = "";
			Adjacenzmatrix[Knoten3][Knoten0] = "";
		}
	}
	return stern_vorhanden;
}
/*********************************************************************************************************************************
* Funktion: Es wird ein neue Element in Adlacenzmatrix eingefuegt.
*		Die "Darstellung"-Objekt speichert den Nenner und Zaehler.
*		Spaater wird ihm eine Abkuerzung der Form "R{i,j}" zugeordnet.
*
* Argument : 4 Knoten eingeben um Element aus der Adlacenzmatrix zu erhalten.
*	- mid_node
*	- pina
*	- pinb
*	- pinc
*
* Rueckgabewert: keine.
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
		Adjacenzmatrix[pina][pinb] = "R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}';
		cout << " - Ergebbnis an den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") : " << Adjacenzmatrix[pina][pinb] << endl;
		cout << "     R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}'  << " = ("<< B.Zaehler + ")/(" << B.Nenner << ')'<<endl;
	}
	else
	{
		Adjacenzmatrix[pina][pinb] = "R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}' + "||" + Adjacenzmatrix[pina][pinb]; 
		cout << " - An den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") ist das Element " << ele0 << " schon vorhanden" <<endl;
		cout << "   => Pararelle Zusammenfassung " << endl;
		cout << "      Ergebbnis an den Knoten (" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + ") : " << Adjacenzmatrix[pina][pinb] << endl;
		cout << "        R{" + rev_node_table[pina + 1] + ',' + rev_node_table[pinb + 1] + '}' << " = (" << B.Zaehler + ")/(" << B.Nenner << ')' << endl;
	}
	Adjacenzmatrix[pinb][pina] = Adjacenzmatrix[pina][pinb]; // Symmetrie

}
/********************************************************************************************************************************
<<<<<<< HEAD
*Funktion: Dieser Operator ruft die Funktionen "seriell->stern2dreieck->dreieck2stern" der Reihefolge nach auf. Wenn Änderungen in
*		   der Schaltung vorgenommen werden, fängt die Zusammenfassungsoperation wieder von vorne an.
* Inputsargument: 
				komponent* last_RLC : Zeiger auf letztes Element in der Komponentlist.
* Rückgabewert  : -
=======
*Funktion: Dieser Operator ruft die Funktionen "seriell->stern2dreieck->dreieck2stern" der Reihefolge nach auf. Wenn Ã„nderungen in
*		   der Schaltung vorgenommen werden, fÃ¤ngt die Zusammenfassungsoperation wieder von vorne an.
* Inputsargument: 
				komponent* last_RLC : Zeiger auf letztes Element in der Komponentlist.
* RÃ¼ckgabewert  : -
>>>>>>> origin/master
*********************************************************************************************************************************/
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

