#include "Zusammenfassen.h"

extern unsigned char k_index;
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

bool zusammenfassen::dreieck2stern()
{
	return false;
}

bool zusammenfassen::stern2dreieck()
{
	return false;
}

void zusammenfassen::operator()(komponent * last_RLC)
{
	Initialize_Adjacenzmatrix(last_RLC);
}

