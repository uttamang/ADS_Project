#include "Nets.h"
#include "global_var.h"
extern komponent* last_RLC;
extern system_1* sys_pointer;
int zusammenfassung::parallel()
{
	komponent* root = last_RLC;
	komponent* second_root = last_RLC;
	komponent* temp = NULL;
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
				temp->next = Matched->next;
				delete Matched;
			}
			else
				temp = Matched;
		}
		root = root->next;
	}
	return 0;
}
int zusammenfassung::seriell()
{
	int check_flag = 0;
	komponent* check_root = NULL;
	komponent* root = last_RLC;
	komponent* second_root = last_RLC;
	komponent* temp = NULL;
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
	
	return 0;
}
