#include "Nets.h"
#include "global_var.h"
extern komponent* last_RLC;
extern system_1* sys_pointer;

void vereinfachen::operator()(komponent * last_RLC)
{
	while (parallel(last_RLC) || seriell(last_RLC));
	printf("\n\n 1te Version fertig!!! \n\n");
}
bool vereinfachen::parallel(komponent* last_RLC)
{
	komponent* root = last_RLC;
	komponent* second_root = last_RLC;
	komponent* temp = NULL;
	bool Node_changed = false; // Ist die Schaltung geändert, wird auf true gesetzt.
	while (root != NULL)
	{
		first_matched = root;
		second_root = last_RLC;
		while (second_root != NULL)
		{
			Matched = second_root;
			second_root = second_root->next;
			if (first_matched == Matched)
			{
				temp = Matched;
				continue;
			}
			if ((first_matched->NODE_1 == Matched->NODE_1 && first_matched->NODE_2 == Matched->NODE_2) || (first_matched->NODE_1 == Matched->NODE_2 && first_matched->NODE_2 == Matched->NODE_1))
			{
				first_matched-> Element = "(" + first_matched->Element + "||" + Matched->Element + ")";
				Node_changed = true;
				temp->next = Matched->next;
				delete Matched;
			}
			else
				temp = Matched;
		}
		root = root->next;
	}
	return Node_changed;
}
bool vereinfachen::seriell(komponent* last_RLC)
{
	int check_flag = 0;
	komponent* check_root = NULL;
	komponent* root = last_RLC;
	komponent* second_root = last_RLC;
	komponent* temp = NULL;
	bool Node_changed = false;
	while (root != NULL)
	{
		
		first_matched = root;
		second_root = last_RLC;
		while (second_root != NULL)
		{
			
			Matched = second_root;
			second_root = second_root->next;
			if (first_matched == Matched)
			{
				temp = Matched;
				continue;
			}

			if (first_matched->NODE_1 == Matched->NODE_1 && first_matched->NODE_2 != Matched->NODE_2) 
			{
				//printf("Fall 1 \n");
				check_flag = 0; 
				check_root = last_RLC;
				while (check_root != NULL)
				{					
					if (first_matched->NODE_1 == check_root->NODE_1 || first_matched->NODE_1 == check_root->NODE_2)
					{
						check_flag++;
					}
					check_root = check_root->next;
				}
				if (first_matched->NODE_1 == sys_pointer->INPUT || first_matched->NODE_1 == sys_pointer->OUTPUT || first_matched->NODE_1 == sys_pointer->GND) check_flag = 0;
				if (check_flag == 2)
				{
					Node_changed = true;
					first_matched->Element = "(" + first_matched->Element + "+" + Matched->Element + ")";
					first_matched->NODE_1 = first_matched->NODE_2;
					first_matched->NODE_2 = Matched->NODE_2;
					//printf("first_matches : %s \n", first_matched->Element);
					temp->next = Matched->next;
					delete Matched;
					check_flag = 0;
				}
			}
			else if (first_matched->NODE_1 == Matched->NODE_2 && first_matched->NODE_2 != Matched->NODE_1)
			{
				//printf("Fall 2 \n");
				check_flag = 0;
				check_root = last_RLC;
				while (check_root != NULL)
				{
					if (first_matched->NODE_1 == check_root->NODE_1 || first_matched->NODE_1 == check_root->NODE_2)
					{
						check_flag++;
					}
					check_root = check_root->next;
				}
				if (first_matched->NODE_1 == sys_pointer->INPUT || first_matched->NODE_1 == sys_pointer->OUTPUT || first_matched->NODE_1 == sys_pointer->GND) check_flag = 0;
				if (check_flag == 2)
				{
					Node_changed = true;
					first_matched->Element = "(" + first_matched->Element + "+" + Matched->Element + ")";
					first_matched->NODE_1 = first_matched->NODE_2;
					first_matched->NODE_2 = Matched->NODE_1;
					//printf("first_matches : %s \n", first_matched->Element);
					temp->next = Matched->next;
					delete Matched;
					check_flag = 0;
				}
			}
			else if (first_matched->NODE_2 == Matched->NODE_1 && first_matched->NODE_1 != Matched->NODE_2)
			{
				//printf("Fall 3 \n");
				check_flag = 0; 
				check_root = last_RLC;
				while (check_root != NULL)
				{
					if (first_matched->NODE_2 == check_root->NODE_1 || first_matched->NODE_2 == check_root->NODE_2)
					{
						check_flag++;
					}
					check_root = check_root->next;
				}
				if (first_matched->NODE_2 == sys_pointer->INPUT || first_matched->NODE_2 == sys_pointer->OUTPUT || first_matched->NODE_2 == sys_pointer->GND) check_flag = 0;
				if (check_flag == 2)
				{
					Node_changed = true;
					first_matched->Element = "(" + first_matched->Element + "+" + Matched->Element + ")";
					first_matched->NODE_1 = first_matched->NODE_1;
					first_matched->NODE_2 = Matched->NODE_2;
					//printf("first_matches : %s \n", first_matched->Element);
					temp->next = Matched->next;
					delete Matched;
					check_flag = 0;
				}
			}
			else if (first_matched->NODE_2 == Matched->NODE_2 && first_matched->NODE_1 != Matched->NODE_1)
			{
				//printf("Fall 4 \n");
				check_flag = 0;
				check_root = last_RLC;
				while (check_root != NULL)
				{
					if (first_matched->NODE_2 == check_root->NODE_1 || first_matched->NODE_2 == check_root->NODE_2)
					{
						check_flag++;
					}
					check_root = check_root->next;
				}
				if (first_matched->NODE_2 == sys_pointer->INPUT || first_matched->NODE_2 == sys_pointer->OUTPUT || first_matched->NODE_2 == sys_pointer->GND) check_flag = 0;
				if (check_flag == 2)
				{
					Node_changed = true;
					first_matched->Element = "(" + first_matched->Element + "+" + Matched->Element + ")";
					first_matched->NODE_1 = first_matched->NODE_1;
					first_matched->NODE_2 = Matched->NODE_1;
					//printf("first_matches : %s \n", first_matched->Element);
					temp->next = Matched->next;
					delete Matched;
					check_flag = 0;
				}
			}
			else
				temp = Matched;
		}
		root = root->next;
	}
	
	return Node_changed;
}


