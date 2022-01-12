#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
// const
#define FILENAME "Data.txt"
#define MAX_CAR 17
#define MAX_CAR_LENGTH 40
#define MAX_SPEED 3
#define FIRST_SPEED 10
#define WIDTH_CONSOLE 88
#define HEIGH_CONSOLE 20
// global variable
// when use extern to declare global var in header.h, you must declare var again in main.cpp without extern
extern POINT** X;		// array MAX_CAR car
extern POINT Y;		// var represent passer
extern int cnt;	// var support when increasing speed of car
extern char MOVING;		// var determine way passer move
extern int SPEED;		// speed of cars (level)
extern bool STATE;		// status live/dead of passer
extern POINT alive[WIDTH_CONSOLE];	// Save coordinates of alive passer
extern int Num_alive;
extern bool flat;
extern int Round[MAX_CAR];	//Round Stop Delay _ 3 array for stop cars
extern int Stop[MAX_CAR];
extern int Delay[MAX_CAR];

void FixConsoleWindow();		// Co dinh man hinh (fixed screen)
void GotoXY(int x, int y);		// Move to (x,y)
void ResetData();				// Create data begin
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0);	// Draw board
void StartGame();				// Do the job before enter game
void GabageCollect();			// Delete garbage
void ExitGame(HANDLE t);		// Exit game
void PauseGame(HANDLE t);		// Pause game
void ProcessDead();				// Process when passer "dead"
void ProcessFinish(POINT& p);	// Process when passer "Live and Finish"
void DrawCars(const char* s);			// Draw cars
void DrawSticker(const POINT& p, const char* s);	// Draw paser
bool IsImpact(const POINT& p, int d);		// check whether accident
void MoveCars();				// set movement of cars
void EraseCars();				// Delete cars

// movement of passer
void MoveRight();				// Passer Move right
void MoveLeft();				// Passer move left
void MoveDown();				// Passer move down
void MoveUp();					// Passer move up
//-------------
void SubThread();	// run function

//------- add function ------------------
// -- WRITE --
										// Sua Code cua thay 5 hours
										// StopCar 1.5 hours
void GraphicDead(const POINT& p);		// Graphic when impact (done)	5mins
void ImpactAnother(POINT& p, int& Num);	// Process when passer impact passer before (done)	60 mins
void Menu();
void Notice(int& choice);				// Graphic of menu (done) 5 hours
void About();
void Rule();			
void ClearData();
void EnterGame(int& choice);
void PlayGame(int idx);
void SaveGame(string name, POINT& p);// Save Game
bool checkName(const string& name);
vector<string> parse(string haystack, string seperator, bool RemoveEmptyEntries);
void Pause(HANDLE t);
void SaveLoad();						// Save/Load game (done)	5 hours
void LoadData_Name(int& idx);
void updateData(int idx);
void ByeBye();
vector<string> parse(string haystack, string seperator, bool RemoveEmptyEntries);
// -- REFERENCE --

void SetColor(int ForgC);		// Change text color			//https://stackoverflow.com/questions/29574849/how-to-change-text-color-and-console-color-in-codeblocks
void TextColor(int x);			// Change text and background	//https://tranhanhuy.wordpress.com/2011/07/10/c-ham-mau-dung-cho-vc-6-0-textcolor/

// tam dung: viet trong ham MoveCars (update) cho chay trong vong Stop[i]-Delay[i] thi delay trong Delay[i]  Round[i] bien chay