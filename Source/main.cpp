#include "Header.h"

// ---- declare global var ----
POINT** X;		// array MAX_CAR car
POINT Y;		// var represent passer
int cnt = 0;	// var support when increasing speed of car
char MOVING;		// var determine way passer move
int SPEED;		// speed of cars (level)
bool STATE = true;		// status live/dead of passer
POINT alive[WIDTH_CONSOLE];
int Num_alive = 0;
bool flat = false;
int Round[MAX_CAR] = { 0 };
int Stop[MAX_CAR];
int Delay[MAX_CAR];
//--------------------------------


void main()
{
	FixConsoleWindow();
	srand(time(NULL));
	for (int i = 0; i < MAX_CAR; i++)
	{
		Stop[i] = rand() % 15 + 45;
		Delay[i] = rand() % 15 + 10;
	}
	Menu();
}
