#include "Nets.h"
extern komponent* last_RLC;
int zusammenfassung::parallel()
{
	komponent* root = last_RLC;
	komponent* second_root = last_RLC;
	komponent* temp = NULL;
	while (root != NULL )
	{
		first_matched = root;
		second_root = last_RLC;
		while (second_root != NULL )
		{
			Matched = second_root;
			second_root = second_root->next;
			if (first_matched == Matched)
			{
				temp = Matched;
				continue;
			}
			if (first_matched->NODE_1 == Matched->NODE_1 && first_matched->NODE_2 == Matched->NODE_2)
			{
				first_matched->Element = first_matched->Element + "||" + Matched->Element;
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
