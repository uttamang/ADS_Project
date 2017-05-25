#include "Zusammenfassen.h"

extern unsigned char k_index;
extern system_1 *sys_pointer;
void zusammenfassen::Initialize_Adjacenzmatrix(komponent * last_RLC)
{
	unsigned char max_anzahl = k_index +1 ;// weil nur Komma gezählt wird.
	int index = 1;
	komponent* rt = last_RLC;
	vector<string> init; // Init the first element of Ad_Mat with empty string
	while (rt != NULL)
	{
		if (load_node_table(rt->NODE_1, index))
		{
			Adjacenzmatrix.push_back(init);
			Adjacenzmatrix[node_table[rt->NODE_1] - 1].assign(max_anzahl, "");
			index++;
		}
		if (load_node_table(rt->NODE_2, index))
		{
			Adjacenzmatrix.push_back(init);
			Adjacenzmatrix[node_table[rt->NODE_2] - 1].assign( max_anzahl, "");
			index++;
		}
		Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] = rt->Element;
		Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] = rt->Element;
		rt = rt->next;
	}
}
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
		if (rev_node_table[lp + 1] == sys_pointer->INPUT)
		{
			//cout << "Seriell gefunden aber die Knoten " << sys_pointer->INPUT << " war INPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer->OUTPUT)
		{
			//cout << "Seriell gefunden aber die Knoten " << sys_pointer->OUTPUT << " war OUTPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer->GND)
		{
			//cout << "Seriell gefunden aber die Knoten " << sys_pointer->GND << " war GND" << endl;
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
				Adjacenzmatrix[pin1][pin2] = "((" + ele1 + '+' + ele2 + ")||" + ele0 +")";
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
bool zusammenfassen::dreieck2stern()
{
	return false;
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
		if (rev_node_table[lp + 1] == sys_pointer->INPUT)
		{
			//cout << "Stern gefunden aber die Knoten " << sys_pointer->INPUT << " war INPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer->OUTPUT)
		{
			//cout << "Stern gefunden aber die Knoten " << sys_pointer->OUTPUT << " war OUTPUT" << endl;
			continue;
		}
		else if (rev_node_table[lp + 1] == sys_pointer->GND)
		{
			//cout << "Stern gefunden aber die Knoten " << sys_pointer->GND << " war GND" << endl;
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
	/*if (Adjacenzmatrix[pina][pinb] == "")
	{
		Adjacenzmatrix[pina][pinb] = 
			"(" + Adjacenzmatrix[node][pina] + "*" + Adjacenzmatrix[node][pinb] + "+"
			+ Adjacenzmatrix[node][pinb] + "*" + Adjacenzmatrix[node][pinc] + "+"
			+ Adjacenzmatrix[node][pinc] + "*" + Adjacenzmatrix[node][pina] + "+"
			+ ")/" + Adjacenzmatrix[node][pinc];
	}
	else
	{
		Adjacenzmatrix[pina][pinb] = 
			"((" + Adjacenzmatrix[node][pina] + "*" + Adjacenzmatrix[node][pinb] + " + "
			+ Adjacenzmatrix[node][pinb] + "*" + Adjacenzmatrix[node][pinc] + "+"
			+ Adjacenzmatrix[node][pinc] + "*" + Adjacenzmatrix[node][pina] + "+"
			+ ")/" + Adjacenzmatrix[node][pinc] +")||" + Adjacenzmatrix[pina][pinb]; ;
	}*/
	if (Adjacenzmatrix[pina][pinb] == "")
	{
		Adjacenzmatrix[pina][pinb] = "R" + rev_node_table[pina+1] + rev_node_table[node+1] + rev_node_table[pinb+1];
	}
	else
	{
		Adjacenzmatrix[pina][pinb] = "R" + rev_node_table[pina + 1] + rev_node_table[node + 1] + rev_node_table[pinb + 1] +"||" + Adjacenzmatrix[pina][pinb]; ;
	}
	Adjacenzmatrix[pinb][pina] = Adjacenzmatrix[pina][pinb]; // Symmetrie
	
}
void zusammenfassen::operator()(komponent * last_RLC)
{
	Initialize_Adjacenzmatrix(last_RLC);
	while (seriell() || stern2dreieck())
	{
		print_Adj();
	}
	cout << endl << "Keine Aenderung vorgenemmen => fertig !" << endl;
	print_Adj();
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
}
