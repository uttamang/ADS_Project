#include "knotenlist.h"
#include <map>
using namespace std;

/* This function initialize knotenlist*/

Knotenlist * Knotenlist::setKnoten(komponent * last_RLC)
{
	komponent* pointer = last_RLC;
	Knotenlist* temp = NULL;
	Knotenlist* rt = NULL;
	map<string, int> already_read_node_str;
	map<string, elementlist*> node_table;
	while (pointer != NULL)
	{
		if (already_read_node_str[pointer->NODE_1] == NULL)
		{
			node_table[pointer->NODE_1] = NULL;
			already_read_node_str[pointer->NODE_1] = 1;
			rt = new Knotenlist;
			rt->NODE = pointer->NODE_1;
			rt->Ele_List = new elementlist;
			rt->Ele_List->ELE_NAME = pointer->Element;

			rt->Ele_List->next = node_table[pointer->NODE_1];
			node_table[pointer->NODE_1] = rt->Ele_List;

			rt->next_knoten = temp;
			temp = rt;
		}
		else
		{
			elementlist* next_element = new elementlist;
			next_element->ELE_NAME = pointer->Element;

			next_element->next = node_table[pointer->NODE_1];
			node_table[pointer->NODE_1] = next_element;
		}
		if (already_read_node_str[pointer->NODE_2] == NULL)
		{
			node_table[pointer->NODE_2] = NULL;
			already_read_node_str[pointer->NODE_2] = 1;
			rt = new Knotenlist;
			rt->NODE = pointer->NODE_2;
			rt->Ele_List = new elementlist;
			rt->Ele_List->ELE_NAME = pointer->Element;

			rt->Ele_List->next = node_table[pointer->NODE_2];
			node_table[pointer->NODE_2] = rt->Ele_List;

			rt->next_knoten = temp;
			temp = rt;
		}
		else
		{
			elementlist* next_element = new elementlist;
			next_element->ELE_NAME = pointer->Element;

			next_element->next = node_table[pointer->NODE_2];
			node_table[pointer->NODE_2] = next_element;
		}
		pointer = pointer->next;
	}

	/*turn the last pointer of elementlist to each node*/
	Knotenlist* ptr_node = temp;
	while (ptr_node != NULL)
	{
		ptr_node->Ele_List = node_table[ptr_node->NODE];
		ptr_node = ptr_node->next_knoten;
	}
	return temp;
}

void Knotenlist::display_node(Knotenlist * last_node)
{
	printf("\n");
	if (last_node != NULL)
	{
		printf("\n");
		printf("Nodename: %s \n", last_node->NODE.c_str());
		printf("Elementname: \n");
		last_node->Ele_List->display_elementlist(last_node->Ele_List);
		display_node(last_node->next_knoten);
	}
}

void elementlist::display_elementlist(elementlist * last_element)
{
	if (last_element != NULL)
	{
		display_elementlist(last_element->next);
		printf(" %s", last_element->ELE_NAME.c_str());
	}
}
