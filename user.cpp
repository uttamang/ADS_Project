// User.cpp : header file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"
#include "graphics\graphicfunctions.h"
#include "header.h"
#include <iostream>
#include <fstream>
#include "math.h"

int ww = 1200;
int hh = ww / 2;
char fistr[100];
komponent* RLC;
system_1 sys_pointer;
unsigned char k_index = 0;
bool mouse_weiter_flag = false;
COLORREF Colref[] = { BLACK,RED,GREEN,BLUE,YELLOW,BROWN };
int Colind = 0;

#ifndef _USE_OLD_OSTREAMS
using namespace std;
#endif

#define _pi					3.1415926535				// Die Zahl Pi.
#define _180_durch_pi		57.2957795147				// = 180 * pi. Dient zur Laufzeitoptinierung.
#define _sinus(winkel)		sin((winkel) / _180_durch_pi)	// Funktion im Gradmass.
#define _cosinus(winkel)	cos((winkel) / _180_durch_pi)	// Funktion im Gradmass.

/************************************************************************************************************
* Funktinskoepfe
************************************************************************************************************/
komponent* scanner(void);
int Brueck_Darstellung(string Nenner, string Zaehler, int ww, int hh, int schrift);
int breite_berechnung(string tx, int schrift);
void print_inputfile();
void print_kanten_tabelle(komponent* last_RLC);
void Bereich_Darstellung();
void Schaltung_Darstellung(int x0, int y0, zusammenfassen Netz);
void Restart();
bool weiter();
void text_x(int x, int y, int size, int color, string tx);

/************************************************************************************************************
* main
************************************************************************************************************/
void user_main()
{
	while (1)
	{
		komponent* last_RLC;
		last_RLC = scanner();
		print_kanten_tabelle(last_RLC);
		zusammenfassen Netz1;
		Netz1(last_RLC);
		int schrift = ww / 50;
		set_windowpos(20, 40, ww, hh);
		while (1) {								// Endlosschleife
			set_drawarea(ww, hh);				// Setzen des Zeichenbereiches
			clrscr();
			Bereich_Darstellung();
			int y_U = hh / 6;
			int x_U = Brueck_Darstellung(Netz1.H_Nenner, Netz1.H_Zaehler, ww / 6, y_U, schrift);
			x_U -= 3 * 0.4*schrift;
			text_x(x_U, y_U - 0.5*schrift, schrift, BLUE, " = ");
			x_U -= 7 * 0.4*schrift;
			Brueck_Darstellung("U(" + sys_pointer.INPUT + "," + sys_pointer.GND + ")", "U(" + sys_pointer.OUTPUT + "," + sys_pointer.GND + ")", x_U, y_U, schrift);
			Schaltung_Darstellung(x_U, hh / 2 + 50, Netz1);
			print_inputfile();
			// Den "Restart"-Button malen und auf eine Aktivierung warten.
			if (weiter())
			{
				Netz1.clear_all(last_RLC);
				break;
			}
			if (StopProcess())break;
		}
	}
}

komponent* scanner(void)
{
	FILE *inf;
	komponent* last_RLC;
	cout << "Geben Sie bitte den Namen der Datei: ";
	cin >> fistr;
	cout << endl;

	// change from gets(fistr)
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		exit(0);
		return NULL;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	//obj.pr_tokentable();

	last_RLC = obj.yyparse_and_init_Netz(sys_pointer);
	fclose(inf);
	return last_RLC;
}
/************************************************************************************************************
* Funktion : Die Funktion stellt in der graphischen Oberflaeche eine Bruecke dar.
*
* Argument
* - string Nenner
* - string Zaehler
* - int x0 : Anfang Potion in horizontaler Koordinaten
* - int y0 : Anfang Potion in vertikaler Koordinaten
* - int schrit : Schriftsgroesse
*
* Rueckgabewert : Anfangspoition x0
************************************************************************************************************/
int Brueck_Darstellung(string Nenner, string Zaehler, int x0, int y0, int schrift)
{
	int tx_y, x_mitte;
	int line_x, ww_line, line_y;
	char* tx = new char[Zaehler.length() + 1];
	char* tx_n = new char[Nenner.length() + 1];
	int ww_nenner, ww_zaehler;

	strcpy(tx, Zaehler.c_str());
	strcpy(tx_n, Nenner.c_str());

	ww_nenner = breite_berechnung(Nenner, schrift); //Berite des Nenners wird berechnet
	ww_zaehler = breite_berechnung(Zaehler, schrift); // //Berite des Nenners wird berechnet

	if (ww_nenner < ww_zaehler) // Die laengere Breite wird genommen
	{
		ww_line = ww_zaehler;
	}
	else
	{
		ww_line = ww_nenner;
	}
	line(x0, y0 + 1, x0 + ww_line, y0 + 1, RED); // Linie wird dargestellt

	x_mitte = x0 + ww_line / 2;
	text(x_mitte - ww_nenner / 2, y0, schrift, BLUE, tx_n);



	text(x_mitte - ww_zaehler / 2, y0 - schrift, schrift, BLUE, tx);


	delete[] tx;
	delete[] tx_n;
	return x0;

}

/************************************************************************************************************
* Funktion : Die Funktion berechnet die Breite der Zeichenskette
*		Buchstaben und Zeichen haben unterschiedliche Breite.
*		Um die Bruecke optimal darzustellen, wird die passende Breite berechnet.
*		In der Funktion wird die Zeichenskette in 2 Gruppe geteilt. Naemlich Buchstaben und Zeichen.
*
* Rueckgabewert : Berechnette Breite der Zeichenskette
************************************************************************************************************/
int breite_berechnung(string tx, int schrift)
{

	char* text = new char[tx.length() + 1];
	int kk;
	int N_Buchstabe = 0;
	int N_Zeichen;
	int breite;
	strcpy(text, tx.c_str());
	char *pch;
	pch = strtok(text, " ,.-+*()[]|{}"); //Bestimmte Zeichen wird betrachtet.
	while (pch != NULL)
	{
		kk = 0;
		while (*(pch + kk) != '\0')
		{
			N_Buchstabe++; //Buchstaben wird gezaehlt
			kk++;
		}
		pch = strtok(NULL, " ,.-+*()[]|{}");
	}
	N_Zeichen = tx.length() - N_Buchstabe; // Die Anzahl der Zeichen wird berechnet
										   //cout << "Anzahl der Buchstaben :" << N_Buchstabe << "Zeichen" << N_Zeichen;
	breite = N_Buchstabe*schrift*0.51 + N_Zeichen*schrift*0.225; //gewisse Breite wird zu den Buchstaben und Zeichen zugeordnet.
																 //cout << " Breite :" << breite << endl;
	return breite;
}
/************************************************************************************************************
* Funktion : Der Inhalte der eingegebenen Datei wird in Console ausgegeben.
*
* Rueckgabewert : keine
************************************************************************************************************/
void print_inputfile()
{
	cout << endl << "------------------------------------------------------------------------------------------------------" << endl;
	cout << "text der eingegebenen Datei : \"" << fistr << '"' << endl;
	string line;
	ifstream myfile(fistr);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			//line = atoi(line.c_str());
			cout << line << endl;
		}
		myfile.close();
	}
	else cout << "cannot open the file modulated_signal.txt" << endl;
}

/************************************************************************************************************
* Funktion : Die Kantentabelle wird in Console ausgegeben.
*
* Rueckgabewert : komponent * last RLC
************************************************************************************************************/
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

/************************************************************************************************************
* Funktion : Die graphische Oberflaeche wird vorbereitet
*
* Rueckgabewert : keine
************************************************************************************************************/
void Bereich_Darstellung()
{
	text_x(20, 15, 32, BLUE, "�bertragungsfunktion");
	rectangle(10, 10, ww - 10, hh / 3, BLUE, -1);
	text_x(20, hh / 3 - 25, 18, BLUE, "Legende der Wiederst�nde in Console-Fenster");
	text_x(20, hh / 3 + 15, 32, BLUE, "zusammengefasste Schaltung");
	rectangle(10, hh / 3 + 10, ww - 10, hh - 10, BLUE, -1);
}

/************************************************************************************************************
* Funktion : Die Endresultat der zusammengefassten Schaltung wird dargestellt.
*		Die zwei Widerstaende wird geschrieben. Die Eingangs- und Ausgangsknoten
*		werden gezeichnet.
*
* Argumente :
*  - int x0 : Anfangspotition in x-Achse
*  - int y0 : Anfangspotition in yx-Achse
*  - Objekt zusammenfassen : Die Widerstaende werden von dem Objekt geholt
* Rueckgabewert : keine
************************************************************************************************************/
void Schaltung_Darstellung(int x0, int y0, zusammenfassen Netz)
{
	int Z_ww = 50;
	int Z_hh = 10;
	int x, y;
	x = x0;
	y = y0;
	text_x(x - 15, y - 20, 32, BLUE, sys_pointer.INPUT.c_str());
	line(x, y, x + 75, y, BLUE);
	x += 75;
	text_x(x, y - 50, 24, BLUE, Netz.H_R_in.c_str());
	rectangle(x, y - Z_hh, x + Z_ww, y + Z_hh, BLUE, -1);
	x += Z_ww;
	line(x, y, x + 75, y, BLUE);
	x += 75;
	text_x(x + 5, y - 20, 32, BLUE, sys_pointer.OUTPUT.c_str());
	line(x, y, x, y + 75, BLUE);
	y += 75;
	text_x(x + 25, y + 10, 24, BLUE, Netz.H_Zaehler.c_str());
	rectangle(x - Z_hh, y, x + Z_hh, y + Z_ww, BLUE, -1);
	y += Z_ww;
	line(x, y, x, y + 75, BLUE);
	y += 75;
	text_x(x - 5, y - 5, 32, BLUE, sys_pointer.GND.c_str());
	line(x - Z_hh, y, x + Z_hh, y, BLUE);
}
void Restart()
{
	int b, h, x, y;

	get_drawarea(&b, &h);

	textbox(b - 120, h - 40, b - 5, h - 5, 18, BLUE, GREY, GREY, SINGLE_LINE | VCENTER_ALIGN | CENTER_ALIGN, ("Restart"));
	updatescr();

	while (
		!((mouseclick(&x, &y) == 1) &&
		((x > b - 120) && (x < b - 5)) &&
			((y > h - 40) && (y < h - 5))
			)) {
		printf(".");
		if (StopProcess())break;
	};

	printf("######################################\n\n");
	clrscr();
	printf("######################################\n\n");
}
bool weiter()
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
		if (StopProcess())break;
		return true;
	};

}

/************************************************************************************************************
* Funktion : Die Funktion stellt eine Textdarstellung dar. Mit der Funktion laesst sich es vermeiden,
* jedes mal string zu kopieren, die Anzahl der Zeichen zu berechnen.
*
* Argumente :
* Rueckgabewert : keine
************************************************************************************************************/
void text_x(int x, int y, int size, int color, string tx)
{
	char* tx_n = new char[tx.length() + 1];
	strcpy(tx_n, tx.c_str());
	text(x, y, size, color, tx_n);
}
