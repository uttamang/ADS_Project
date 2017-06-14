// User.cpp : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"
#include "graphics\graphicfunctions.h"
#include "header.h"

#include <iostream>
#include <fstream>


komponent* RLC;
system_1 sys_pointer;
unsigned char k_index = 0;
bool mouse_weiter_flag = false;

void print_kanten_tabelle(komponent* last_RLC);
komponent* scanner(void);

#ifndef _USE_OLD_OSTREAMS
using namespace std;
#endif



#include "math.h"

#define _pi					3.1415926535				// Die Zahl Pi.
#define _180_durch_pi		57.2957795147				// = 180 * pi. Dient zur Laufzeitoptinierung.
#define _sinus(winkel)		sin((winkel) / _180_durch_pi)	// Funktion im Gradmass.
#define _cosinus(winkel)	cos((winkel) / _180_durch_pi)	// Funktion im Gradmass.

COLORREF Colref[]={BLACK,RED,GREEN,BLUE,YELLOW,BROWN};
int Colind=0;
int Brueck_Darstellung(string Nenner, string Zaehler, int ww, int hh, int schrift);
void Bereich_Darstellung();
void Restart()
{
	int b, h, x, y;

	get_drawarea(&b, &h);

	textbox(b - 120, h - 40, b - 5, h - 5, 18, BLUE, GREY, GREY, SINGLE_LINE | VCENTER_ALIGN | CENTER_ALIGN, ("Restart"));
	updatescr();

	while ( 
		!((mouseclick(&x,&y) == 1) &&
		  ((x > b-120) && (x < b-5)) &&
		  ((y > h-40)  &&	(y < h-5))
		 )) {
		printf(".");
		if(StopProcess())break;
	};

	printf("######################################\n\n");
	clrscr();
	printf("######################################\n\n");
}
void weiter();
void Zeichnen_Schaltung(Ad_Mat netz, int a0, int a1);
void legend(zusammenfassen& l, int schrift);
void text_x(int x, int y, int size, int color, string tx);


void user_main()
{
	komponent* last_RLC;
	last_RLC = scanner();
	print_kanten_tabelle(last_RLC);
	zusammenfassen Netz1;
	Netz1(last_RLC);

	int ww = 1400, hh = 900;
	int schrift = 24;
	char* tx = "1234567890";
	
	set_windowpos(0, 0, ww, hh);
	while (1) {								// Endlosschleife
		set_drawarea(ww, hh);				// Setzen des Zeichenbereiches
		clrscr();
		Bereich_Darstellung();
		legend(Netz1, schrift);
		/*text(750,200,24,BLUE,".");
		text(755, 200, 24, BLUE,".");
		text(760, 200, 24, BLUE, ".");
		text(765, 200, 24, BLUE, ".");
		text(770, 200, 24, BLUE, ".");
		text(775, 200, 24, BLUE, ".");
		text(780, 200, 24, BLUE, ".");
		text(750, 224, 24, BLUE, "AA");
		text(774, 224, 24, BLUE, "A");*/
		Brueck_Darstellung( Netz1.H_Nenner, Netz1.H_Zaehler, 1000, 800, schrift);
							// Den "Restart"-Button malen und auf eine Aktivierung warten.
		weiter();
		if (StopProcess())break;

	}
}

int Brueck_Darstellung(string Nenner, string Zaehler, int ww, int hh, int schrift)
{
	int tx_y, tx_x;
	int temp;
	char* tx = new char[Zaehler.length() + 1];
	char* tx_n = new char[Nenner.length() + 1];
	strcpy(tx, Zaehler.c_str());
	strcpy(tx_n, Nenner.c_str());

	tx_x = ww / 2 - Nenner.length()*schrift*0.9 /2;
	tx_y = hh / 2 + schrift;
	text(tx_x, tx_y, schrift, BLUE, tx_n);
	temp = tx_x;

	line(tx_x, tx_y, tx_x + schrift*0.9 * Nenner.length()/2, tx_y, RED);

	tx_x = ww / 2 - Zaehler.length()*schrift/2;
	tx_y = hh / 2;
	text(tx_x, tx_y, schrift, BLUE, tx);

	delete[] tx;
	delete[] tx_n;
	return temp;

}
void Bereich_Darstellung()
{
	rectangle(10,10,500,800,BLUE,-1);
	rectangle(510, 10, 1310, 400, BLUE, -1);
	rectangle(510, 410, 1310, 800, BLUE, -1);
}
komponent* scanner(void)
{
	FILE *inf;
	komponent* last_RLC;
	char fistr[100];
	cout << "Geben Sie bitte den Namen der Datei: ";
	cin >> fistr;
	cout << endl;

	// change from gets(fistr)
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		return NULL;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	//obj.pr_tokentable();

	last_RLC = obj.yyparse_and_init_Netz(sys_pointer);
	return last_RLC;
}
void print_kanten_tabelle(komponent* last_RLC)
{
	komponent* rt = last_RLC;
	printf("NODE1| ");
	while (rt != NULL)
	{
		printf("%s ", rt->NODE_1.c_str());
		rt = rt->next;
	}
	printf("| \n");

	printf("NODE2| ");
	rt = last_RLC;
	while (rt != NULL)
	{
		printf("%s ", rt->NODE_2.c_str());
		rt = rt->next;
	}
	printf("| \n");

	rt = last_RLC;
	printf("Kante| ");
	while (rt != NULL)
	{
		printf("%s ", rt->Element.c_str());
		rt = rt->next;
	}
	printf("\n");
}
void weiter()
{
	int b, h, x, y;

	get_drawarea(&b, &h);

	textbox(b - 400, h - 40, b - 300, h - 5, 18, BLUE, GREY, GREY, SINGLE_LINE | VCENTER_ALIGN | CENTER_ALIGN, ("weiter"));
	updatescr();

	while (
		!((mouseclick(&x, &y) == 1) &&
		((x > b - 120) && (x < b - 5)) &&
			((y > h - 40) && (y < h - 5))
			)) 
	{
		printf(".");
		mouse_weiter_flag = true;
		if (StopProcess())break;
	};

}
void Zeichnen_Schaltung(Ad_Mat netz, int a0, int a1)
{
	int index = 0;
	float angle = 0;
	for (int i = 0; i < netz.size(); i++)
	{
		for (int ii = 0; ii < netz.size(); ii++)
		{
			if (netz[i][ii] != "")
			{
				index++;
			}
		}
	}
}
void legend(zusammenfassen& l, int schrift)
{
	int y;
	y = 20;
	int x_x;
	text_x(15,y,50,BLUE,"LEGENDE");
	for (auto& x : l.legend) {
		y += 5*schrift;
		x_x = Brueck_Darstellung(x.second.Nenner, x.second.Zaehler, 500 + x.first.size()+schrift*0.5, y, schrift);
		text_x(x_x - x.first.size()*schrift*0.9/2  ,y/2+0.5*schrift,schrift,BLUE,x.first + " = ");
	}
}
void text_x(int x, int y , int size, int color , string tx)
{
	char* tx_n = new char[tx.length() + 1];
	strcpy(tx_n, tx.c_str());
	text(x,y,size,color,tx_n);
}