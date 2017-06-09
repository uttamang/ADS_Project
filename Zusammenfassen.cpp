#include "stdafx.h"
#include "Zusammenfassen.hpp"
#include "system.h"

extern unsigned char k_index;
extern system_1 sys_pointer;
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
		if (load_node_table(rt->NODE_1, index))
		{
			index++;
		}
		if (load_node_table(rt->NODE_2, index))
		{
			index++;
		}
		if (Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] != "")
		{
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] += "||" + rt->Element;
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_1] - 1][node_table[rt->NODE_2] - 1] = rt->Element;

		if (Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] != "")
		{
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] += "||" + rt->Element;
		}
		else
			Adjacenzmatrix[node_table[rt->NODE_2] - 1][node_table[rt->NODE_1] - 1] = rt->Element;
		rt = rt->next;
	}
	max_read_node = index;
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

bool zusammenfassen::dreieck2stern()
{
	bool dreieck_vorhanden = false;
	vector<char> stern_position(3);
	for (char i = 0; i <= k_index && !dreieck_vorhanden; i++)
	{
		stern_position[0] = i;
		if ( i == node_table[sys_pointer.GND] - 1)
			continue;
		for (char ii = 0; ii <= k_index && !dreieck_vorhanden; ii++)
		{
			if ( ii == node_table[sys_pointer.GND] - 1)
				continue;
			if (Adjacenzmatrix[i][ii] != "")
			{
				stern_position[1] = ii;
				for (char i3 = 0; i3 <= k_index; i3++)
				{
					if (i3 == node_table[sys_pointer.GND] - 1)
						continue;
					if (Adjacenzmatrix[ii][i3] != "")
					{
						if (Adjacenzmatrix[i3][i] != "")
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
		for (char i = 0; i <= k_index; i++)
		{
			Adjacenzmatrix[i].push_back("");
		}
		k_index++;
		vector<string> init;
		init.assign(k_index + 1, "");
		Adjacenzmatrix.push_back(init);
		load_node_table("st", k_index+1);
		
		for (int i = 0; i < 3; i++)
		{
			Adjacenzmatrix[stern_position[i]][k_index] = Adjacenzmatrix[stern_position[i]][stern_position[1]] + Adjacenzmatrix[stern_position[i]][stern_position[2]] + "/(" + \
				Adjacenzmatrix[stern_position[0]][stern_position[1]] + "+" + Adjacenzmatrix[stern_position[0]][stern_position[2]] + "+" + Adjacenzmatrix[stern_position[1]][stern_position[2]] + ')';
			Adjacenzmatrix[k_index][stern_position[i]] = Adjacenzmatrix[stern_position[0]][stern_position[1]] + Adjacenzmatrix[stern_position[0]][stern_position[2]] + "/(" + \
				Adjacenzmatrix[stern_position[0]][stern_position[1]] + "+" + Adjacenzmatrix[stern_position[0]][stern_position[2]] + "+" + Adjacenzmatrix[stern_position[1]][stern_position[2]] + ')';
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
	while (seriell() || stern2dreieck() || dreieck2stern())
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

