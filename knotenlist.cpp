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

			rt->next = temp;
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

			rt->next = temp;
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

	//turn the last pointer of elementlist to each node
	Knotenlist* ptr_node = temp;
	while (ptr_node != NULL)
	{
		ptr_node->Ele_List = node_table[ptr_node->NODE];
		ptr_node = ptr_node->next;
	}
	return temp;
}

void Knotenlist::display_node(Knotenlist * last_node)
{
	//printf("\n");
	if (last_node != NULL)
	{
		printf("\n");
		printf("%s : ", last_node->NODE.c_str());
		//printf("Elementname: \n");
		last_node->Ele_List->display_elementlist(last_node->Ele_List);
		display_node(last_node->next);
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
Knotenlist* Knotenlist::KnotenSortieren(Knotenlist* last_node, system_1* sys_pointer) { // new write Knotenlist* next_knoten; push and pop !!

	Knotenlist* node1 = last_node;
	Knotenlist* last_node1 = NULL;
	Knotenlist* node0 = NULL; // CMD IN .. .. .. OUT
	Knotenlist* node_output = NULL;
	Knotenlist* node_input = NULL;
	Knotenlist* new_node = NULL;
	system_1* sys1 = sys_pointer;
	elementlist* ele0 = NULL;
	elementlist* ele1 = NULL;
	int num_nodes = 0;
	int jj = 0;
	printf("\nnach dem Sortierungsverfahren");
	while (node1 != NULL) {
		num_nodes++;
		last_node1 = node1;
		node1 = node1->next;
	}
	last_node1->next = last_node;
	node1 = last_node;

	int ii = num_nodes;
	last_node1 = node1;
	node1 = node1->next;
	while (node1->NODE != sys1->OUTPUT)
	{
		last_node1 = node1;
		node1 = node1->next;
	}
	node_output = new Knotenlist;
	node_output->NODE = node1->NODE;
	node_output->Ele_List = node1->Ele_List;
	node_output->next = NULL;
	last_node1->next = node1->next;
	node1 = node1->next;
	/*--------------------------------------*/

	while (node1->NODE != sys1->INPUT)
	{
		last_node1 = node1;
		node1 = node1->next;
	}
	node_input = new Knotenlist;
	node_input->NODE = node1->NODE;
	node_input->Ele_List = node1->Ele_List;
	node_input->next = NULL;
	last_node1->next = node1->next;
	node1 = node1->next;
	/*--------------------------------------*/
	while (node1->NODE != sys1->GND)
	{
		last_node1 = node1;
		node1 = node1->next;
	}
	new_node = new Knotenlist;
	new_node->NODE = node1->NODE;
	new_node->Ele_List = node1->Ele_List;
	new_node->next = NULL;
	node_input->next = new_node;
	last_node1->next = node1->next;
	node1 = node1->next;
	ii = ii - 3;
	if (ii == 0)
	{
		node_output->next = node_input;
		return node_output;
	}
	/*--------------------------------------*/
	node0 = node_output;

	while (ii>1)
	{
		// beginn init
		jj = 0;
		ele0 = node0->Ele_List;
		ele1 = node1->Ele_List;
		// end init
		while (ele0->ELE_NAME != ele1->ELE_NAME)
		{
			ele1 = ele1->next;
			if (ele1 == NULL)
			{
				last_node1 = node1;
				node1 = node1->next;
				ele1 = node1->Ele_List;
				jj++; //erste ele0 nicht gleich

				if (jj == ii)
				{
					if (ele0 == NULL)
					{
						jj = ii;
					}
					else
					{
						ele0 = ele0->next;
						jj = 0;
					}
				}
			}
		} // end while
		new_node = new Knotenlist;
		new_node->NODE = node1->NODE;
		new_node->Ele_List = node1->Ele_List;
		new_node->next = NULL;
		node0->next = new_node;
		last_node1->next = node1->next;
		node0 = new_node;
		node1 = node1->next;
		ii--;
	}
	ii--;
	new_node = new Knotenlist;
	new_node->NODE = last_node1->NODE;
	new_node->Ele_List = last_node1->Ele_List;
	new_node->next = node_input;
	node0->next = new_node;
	return node_output;
}