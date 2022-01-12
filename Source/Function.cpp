#include "Header.h"

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

vector<string> parse(string haystack, string seperator = ";", bool RemoveEmptyEntries = false)
{
	//string st = "hello new  world";
	size_t startPos = 0;
	vector<string> result;
	size_t Pos;
	size_t Pos2;
	while (true)
	{
		Pos = haystack.find(seperator, startPos);
		if (Pos != string::npos)
		{
			if (!RemoveEmptyEntries)
			{
				result.push_back(haystack.substr(startPos, Pos - startPos));
				startPos = Pos + seperator.length();
			}
			else
			{
				result.push_back(haystack.substr(startPos, Pos - startPos));
				startPos = Pos + seperator.length();
				Pos2 = haystack.find(seperator, startPos);
				while (true)
				{
					if (Pos2 - Pos == seperator.length())
					{
						Pos = Pos2;
						startPos = Pos + seperator.length();
						Pos2 = haystack.find(seperator, startPos);
					}
					else break;
				}
			}
		}
		else
			break;
	}
	result.push_back(haystack.substr(startPos, haystack.length() - Pos));

	return result;

}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ResetData() {
	MOVING = 'D';	// first, let passer move right
	SPEED = 1;		// first speed
	Y = { (WIDTH_CONSOLE - 2) / 2,HEIGH_CONSOLE - 1 }; // first position of passer 
	// Array of run car
	if (X == NULL) {
		X = new POINT * [MAX_CAR];
		for (int i = 0; i < MAX_CAR; i++)
			X[i] = new POINT[MAX_CAR_LENGTH];
		for (int i = 0; i < MAX_CAR; i++)
		{
			int temp = (rand() % (WIDTH_CONSOLE - MAX_CAR_LENGTH)) + 1;
			for (int j = 0; j < MAX_CAR_LENGTH; j++)
			{
				X[i][j].x = temp + j;
				X[i][j].y = 2 + i;
			}
		}
	}
}

void DrawBoard(int x, int y, int width, int height, int curPosX, int curPosY)
{
	SetColor(6);		// brown
	GotoXY(x, y); cout << 'X';
	for (int i = 1; i < width; i++)cout << 'X';
	cout << 'X';
	GotoXY(x, height + y); cout << 'X';
	for (int i = 1; i < width; i++)cout << 'X';
	cout << 'X';
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << 'X';
		GotoXY(x + width, i); cout << 'X';
	}
	SetColor(4);
	GotoXY(10, height + 2);
	cout << "MOVE:";
	SetColor(3);
	GotoXY(10, height + 3);
	cout << "W: up";
	GotoXY(10, height + 4);
	cout << "S: down";
	GotoXY(10, height + 5);
	cout << "A: left";
	GotoXY(10, height + 6);
	cout << "D: right";

	SetColor(4);
	GotoXY(26, height + 2);
	cout << "QUICK KEY:";
	SetColor(3);
	GotoXY(26, height + 3);
	cout << "L: Save";
	GotoXY(26, height + 4);
	cout << "P: Pause ";
	GotoXY(26, height + 5);
	cout << "Esc: Exit ";

	GotoXY(curPosX, curPosY);
}

void StartGame() {
	system("cls");
	ResetData();	// Creater original data
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // Draw screen game
	STATE = true;	// Allow for Thread run
}

void GabageCollect()
{
	for (int i = 0; i < MAX_CAR; i++)
		delete[] X[i];
	delete[] X;
}

void ExitGame(HANDLE t) {
	system("cls");
	TerminateThread(t, 0);
	GabageCollect();
}

void PauseGame(HANDLE t) {
	SuspendThread(t);
	SetColor(0);
	GotoXY(WIDTH_CONSOLE + 8, 6);
	cout << "--- PAUSED ---";
	GotoXY(WIDTH_CONSOLE + 2, 8);
	cout << "(1)Press any key to continue.";
	GotoXY(WIDTH_CONSOLE + 2, 9);
	cout << "(2)Press E to Exit game.";
	char temp = toupper(_getch());
	if (temp == 'E')
	{
		system("cls");
		GotoXY(50, 9);
		cout << "EXITING ....";
		Sleep(500);
		ByeBye();
	}
	else
	{
		GotoXY(WIDTH_CONSOLE + 8, 6);
		cout << "                    ";
		GotoXY(WIDTH_CONSOLE + 2, 8);
		cout << "                             ";
		GotoXY(WIDTH_CONSOLE + 2, 9);
		cout << "                          ";
	}
}

void Pause(HANDLE t)
{
	SuspendThread(t);
}

void ProcessDead() {
	STATE = 0;
	GraphicDead(Y);
	SetColor(4);
	GotoXY(25, HEIGH_CONSOLE + 8);
	printf("!!!  Dead, type Y to continue or anykey to exit  !!!");
}
void GraphicDead(const POINT& Y) {
	//Y.x Y.y
	SetColor(5);		// set magenta color
	GotoXY(Y.x, Y.y);
	cout << "*";
	GotoXY(Y.x - 2, Y.y);
	cout << "==";
	GotoXY(Y.x + 1, Y.y);
	cout << "==";
	GotoXY(Y.x, Y.y + 1);
	cout << "|";
	GotoXY(Y.x, Y.y - 1);
	cout << "|";
	GotoXY(Y.x - 2, Y.y + 1);
	cout << "//";
	GotoXY(Y.x - 2, Y.y - 1);
	cout << "\\\\";
	GotoXY(Y.x + 1, Y.y - 1);
	cout << "//";
	GotoXY(Y.x + 1, Y.y + 1);
	cout << "\\\\";
	SetColor(15);		// set white color
}

void SetColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void TextColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void ImpactAnother(POINT& p, int& Num)
{
	alive[++Num].x = p.x;			// alive[1] is first position of passer alive
	alive[Num].y = p.y;
	for (int i = 1; i < Num; i++)	// if passer impact alive passer -> passer dead
		if (p.x == alive[i].x && p.y == alive[i].y)
			ProcessDead();
}

void ProcessFinish(POINT& p, int& Num) {
	ImpactAnother(p, Num);
	SPEED == MAX_SPEED ? SPEED = 1 : SPEED++;
	p = { (WIDTH_CONSOLE - 2) / 2,HEIGH_CONSOLE - 1 }; // first position of passer
	MOVING = 'D'; // First let passer move right
}

void DrawCars(const char* s)
{
	SetColor(0);		// Black
	for (int i = 0; i < MAX_CAR; i++) {
		for (int j = 0; j < MAX_CAR_LENGTH; j++)
		{
			GotoXY(X[i][j].x, X[i][j].y);
			cout << s;
		}
	}
}

void DrawSticker(const POINT& p, const char* s) {
	SetColor(3);		// Cyan
	GotoXY(p.x, p.y);
	cout << s;
}

bool IsImpact(const POINT& p, int d)
{
	if (d == 1 || d == 19)return false;
	for (int i = 0; i < MAX_CAR_LENGTH; i++)
	{
		if (p.x == X[d - 2][i].x && p.y == X[d - 2][i].y) return true;
	}
	return false;
}

void MoveCars() {
	for (int i = 1; i < MAX_CAR; i += 2)	// xe le
	{
		Round[i]++;
		if (Round[i] < Delay[i])
			continue;
		else
		{
			cnt = 0;
			do {
				cnt++;
				for (int j = 0; j < MAX_CAR_LENGTH - 1; j++) {
					X[i][j] = X[i][j + 1];
				}
				X[i][MAX_CAR_LENGTH - 1].x + 1 == WIDTH_CONSOLE ? X[i][MAX_CAR_LENGTH - 1].x = 1 : X[i][MAX_CAR_LENGTH - 1].x++; // check whether car touch screen
			} while (cnt < SPEED);
			if (Round[i] == Stop[i]) Round[i] = 0;
		}
	}

	for (int i = 0; i < MAX_CAR; i += 2)
	{
		Round[i]++;
		if (Round[i] < Delay[i])
			continue;
		else
		{
			cnt = 0;
			do {
				cnt++;
				for (int j = MAX_CAR_LENGTH - 1; j > 0; j--)
					X[i][j] = X[i][j - 1];
				X[i][0].x - 1 == 0 ? X[i][0].x = WIDTH_CONSOLE - 1 : X[i][0].x--; // check whether car touch screen
			} while (cnt < SPEED);
			if (Round[i] == Stop[i]) Round[i] = 0;
		}
	}
}

void EraseCars()
{
	for (int i = 0; i < MAX_CAR; i += 2) {
		cnt = 0;
		do {
			GotoXY(X[i][MAX_CAR_LENGTH - 1 - cnt].x, X[i][MAX_CAR_LENGTH - 1 - cnt].y);
			printf(" ");
			cnt++;
		} while (cnt < SPEED);
	}
	for (int i = 1; i < MAX_CAR; i += 2) {
		cnt = 0;
		do {
			GotoXY(X[i][0 + cnt].x, X[i][0 + cnt].y);
			printf(" ");
			cnt++;
		} while (cnt < SPEED);
	}
}

void MoveRight() {
	if (Y.x < WIDTH_CONSOLE - 1)
	{
		DrawSticker(Y, " ");
		Y.x++;
		DrawSticker(Y, "Y");
	}
}

void MoveLeft() {
	if (Y.x > 1) {
		DrawSticker(Y, " ");
		Y.x--;
		DrawSticker(Y, "Y");
	}
}

void MoveDown() {
	if (Y.y < HEIGH_CONSOLE - 1)
	{
		DrawSticker(Y, " ");
		Y.y++;
		DrawSticker(Y, "Y");
	}
}

void MoveUp() {
	if (Y.y > 1) {
		DrawSticker(Y, " ");
		Y.y--;
		DrawSticker(Y, "Y");
	}
}

void EnterGame(int& choice)
{
	do
	{
		system("cls");
		SetColor(4);		// Red color
		GotoXY(44, 5);
		printf("CSC10002 - PROGRAMMING TECHNIQUES");
		GotoXY(51, 6);
		cout << "GAME: CROSS THE ROAD";
		SetColor(3);		// Cyan color
		GotoXY(54, 8);
		cout << "(1) New Game";
		GotoXY(54, 9);
		cout << "(2) Continue Game";
		GotoXY(54, 10);
		cout << "(3) Clear Data";
		GotoXY(54, 11);
		cout << "(4) Menu";
		SetColor(4);
		GotoXY(54, 16);
		cout << "Choose: ";
		cin >> choice;
	} while (choice > 4 || choice < 1);
	switch (choice)
	{
	case (1):
		PlayGame(0);
		break;
	case (2):
		SaveLoad();
		break;
	case (3):
		ClearData();
		break;
	case (4):
		Menu();
		break;
	}
}

void ClearData()
{
	fstream f;
	int choice;
	f.open(FILENAME, ios::out);
	f.close();
	GotoXY(49, 18);
	SetColor(5);
	cout << "!! Data has been clean !!";
	SetColor(7);
	GotoXY(47, 19);
	system("pause");
	SetColor(0);
	EnterGame(choice);
}

void Notice(int& choice)
{
	do
	{
		system("cls");
		SetColor(4);		// Red color
		GotoXY(44, 5);
		printf("CSC10002 - PROGRAMMING TECHNIQUES");
		GotoXY(51, 6);
		cout << "GAME: CROSS THE ROAD";
		SetColor(3);		// Cyan color
		GotoXY(54, 8);
		cout << "(1) Start game";
		GotoXY(54, 9);
		cout << "(2) Rule";
		GotoXY(54, 10);
		cout << "(3) About";
		GotoXY(54, 11);
		cout << "(4) Quit Game";
		SetColor(4);		// Red color
		GotoXY(51, 16);
		cout << "Choose: ";
		cin >> choice;
	} while (choice < 1 || choice>4);
}

void About()
{
	{
		system("cls");
		SetColor(4);
		GotoXY(44, 5);
		printf("CSC10002 - PROGRAMMING TECHNIQUES");
		GotoXY(51, 6);
		cout << "GAME: CROSS THE ROAD";
		GotoXY(40, 8); SetColor(4);
		printf("Lecture:");
		GotoXY(44, 9); SetColor(3);
		printf("- Truong Toan Thinh");
		GotoXY(44, 10);
		printf("- Tran Anh Duy");
		GotoXY(44, 11);
		printf("- Mai Anh Tuan");
		GotoXY(40, 12); SetColor(4);	//Red
		printf("Code by: ");
		GotoXY(44, 13); SetColor(3);	//Cyan	
		printf("- 19127392: To Gia Hao");
		GotoXY(44, 14);
		printf("- 19127471: Luong Kiem Minh");
		GotoXY(44, 15);
		printf("- 19127525: Nguyen Thanh Quan (Leader)");
		GotoXY(44, 16);
		printf("- 19127613: Phan Dinh Tuan");
		GotoXY(40, 19); SetColor(4);	//Red
		printf("HCMUS _ Ho Chi Minh University of Sience.");
		SetColor(7);	//light gray
		GotoXY(45, 20);
		system("pause");
	}
}

void Rule()
{
	system("cls");
	SetColor(4);		// Red color
	GotoXY(44, 5);
	printf("CSC10002 - PROGRAMMING TECHNIQUES");
	GotoXY(51, 6);
	cout << "GAME: CROSS THE ROAD";
	GotoXY(40, 8);
	cout << "	MOVE:";
	SetColor(3);
	GotoXY(40, 9);
	cout << "	W: up";
	GotoXY(40, 10);
	cout << "	S: down";
	GotoXY(40, 11);
	cout << "	A: left";
	GotoXY(40, 12);
	cout << "	D: right";
	SetColor(4);
	GotoXY(60, 8);
	cout << "	QUICK KEY:";
	SetColor(3);
	GotoXY(60, 9);
	cout << "	L: Save game";
	GotoXY(60, 10);
	cout << "	P: Pause game";
	GotoXY(30, 14); SetColor(4);
	cout << "	RULE:";
	GotoXY(30, 15); SetColor(3);
	cout << "	Control Passer(Y) to cross the road";
	GotoXY(30, 16);
	cout << "	If you impact the car, you will be die.";
	GotoXY(30, 17);
	cout << "	If you impact the passers(Y) have complete before, you will be die.";
	GotoXY(30, 18);
	cout << "	When you die. You can play again or exit the game";
	GotoXY(30, 19);
	cout << "	While playing game, you can stop or save the game you are playing";
	GotoXY(30, 20);
	cout << "	You can play a new game or continue the game has been saved";
	GotoXY(50, 22); SetColor(4);
	cout << "   ---- GOOD LUCK ----";
	GotoXY(45, 23); SetColor(7);
	system("pause");
}

void SaveGame(string name, POINT& p)
{
	fstream f;
	// Ghi: name - toa do Passer Y - speed - so nguoi choi pass - toa do cac passer Y da qua - ve xe...
	f.open(FILENAME, ios::out | ios::app);
	f << name << ";" << p.x << ";" << p.y;		// name - toa do Y
	f << ";" << SPEED;						// speed
	f << ";" << Num_alive;						// so nguoi choi pass
	for (int i = 1; i <= Num_alive; i++)
		f << ";" << alive[i].x << ";" << alive[i].y;	// toa do cac passer Y da qua

	for (int i = 0; i < MAX_CAR; i++)			// cars data
		if (i % 2 == 0)
			f << ";" << X[i][MAX_CAR_LENGTH - 1].x << ";" << X[i][MAX_CAR_LENGTH - 1].y;
		else
			f << ";" << X[i][0].x << ";" << X[i][0].y;	// la toa do duoi

	f << endl;
	f.close();
}

void LoadData_Name(int& idx)
{
	fstream f;
	string line;
	f.open(FILENAME, ios::in);
	idx = 0;
	getline(f, line);
	while (!f.eof() && line != "")
	{
		int i = 0;
		GotoXY(54, 10 + idx);
		cout << ++idx << ". ";
		while (line[i] != ';')
		{
			if (line[i] != ';')
				cout << line[i++];
		}
		cout << "\n";
		getline(f, line);
	}
	f.close();
}

bool checkName(const string& name)
{
	fstream f;
	string line;
	f.open(FILENAME, ios::in);
	getline(f, line);
	while (!f.eof() && line != "")
	{
		vector <string> str = parse(line);
		if (str[0] == name) return false;
		getline(f, line);
	}
	f.close();
	return true;
}

void SaveLoad()
{
	int idx, choice;
	do
	{
		system("cls");
		SetColor(4);		// Red color
		GotoXY(44, 5);
		printf("CSC10002 - PROGRAMMING TECHNIQUES");
		GotoXY(51, 6);
		cout << "GAME: CROSS THE ROAD";
		GotoXY(54, 8);
		cout << "-- LOAD GAME --";
		SetColor(3);		// Cyan color
		LoadData_Name(idx);		// load game
		if (idx != 0)
		{
			SetColor(4);
			GotoXY(54, 12 + idx);
			cout << "Choose: ";
			cin >> choice;
		}
		else
		{
			GotoXY(50, 12);
			cout << "No data has been saved";
			SetColor(7);
			GotoXY(46, 13);
			system("pause");
			SetColor(0);
			EnterGame(choice);
			break;
		}
	} while (choice > idx || choice < 1);

	PlayGame(idx);
}

void updateData(int idx) {
	fstream f;
	string line;
	f.open(FILENAME, ios::in);
	int i = 1;
	getline(f, line);
	while (!f.eof() && line != "" && i != idx)
	{
		++i;
		getline(f, line);
	}
	vector<string> infor = parse(line);
	f.close();
	cout << endl;
	//-----------------------------
	SPEED = atoi(infor[3].c_str());		// Load first speed
	Y = { atoi(infor[1].c_str()),atoi(infor[2].c_str()) }; // Load first position of passer 
	Num_alive = atoi(infor[4].c_str());
	for (int i = 1; i <= Num_alive; i++)
	{
		alive[i].x = atoi(infor[3 + i * 2].c_str());		// Load alive passer
		alive[i].y = atoi(infor[4 + i * 2].c_str());
	}
	for (int i = 0; i < MAX_CAR; i++)			// Load tail of cars
		if (Num_alive == 0)
			if (i % 2 == 0)
			{																	// num alive =0
				X[i][MAX_CAR_LENGTH - 1].x = atoi(infor[i * 2 + 5].c_str());	//5	9
				X[i][MAX_CAR_LENGTH - 1].y = atoi(infor[i * 2 + 6].c_str());	//6	10
			}
			else
			{
				X[i][0].x = atoi(infor[2 * i + 1 + 4].c_str());					//7	11
				X[i][0].y = atoi(infor[2 * i + 1 + 5].c_str());					//8	12
			}
		else
			if (i % 2 == 0)
			{																	// num alive =2
				X[i][MAX_CAR_LENGTH - 1].x = atoi(infor[i * 2 + 5 + Num_alive * 2].c_str());	//9		13
				X[i][MAX_CAR_LENGTH - 1].y = atoi(infor[i * 2 + 6 + Num_alive * 2].c_str());	//10	14
			}
			else
			{
				X[i][0].x = atoi(infor[2 * i + 1 + 4 + Num_alive * 2].c_str());					//11	15
				X[i][0].y = atoi(infor[2 * i + 1 + 5 + Num_alive * 2].c_str());					//12	16
			}

	for (int i = 0; i < MAX_CAR; i++)
	{
		for (int j = 1; j < MAX_CAR_LENGTH - 1; j++)
		{
			if (i % 2 == 0)
			{
				X[i][MAX_CAR_LENGTH - 1 - j].x = X[i][MAX_CAR_LENGTH - j].x - 1;		// xe chan
				if (X[i][MAX_CAR_LENGTH - 1 - j].x == 0)
					X[i][MAX_CAR_LENGTH - 1 - j].x = WIDTH_CONSOLE - 1;
				X[i][MAX_CAR_LENGTH - 1 - j].y = X[i][MAX_CAR_LENGTH - 1].y;
			}
			else
			{
				X[i][j].x = X[i][j - 1].x + 1;
				if (X[i][0 + j].x == WIDTH_CONSOLE)
					X[i][0 + j].x = 1;
				X[i][0 + j].y = X[i][0].y;
			}
		}
		if (i % 2 == 0)
			X[i][0].x = X[i][1].x;
		else
			X[i][MAX_CAR_LENGTH - 1].x = X[i][MAX_CAR_LENGTH - 2].x;
	}
	flat = true;
}

void ByeBye()
{
	system("cls");
	GotoXY(50, 9);
	SetColor(5);
	cout << "EXITING ....";
	Sleep(500);
	system("cls");
	SetColor(4);		// Red color
	GotoXY(44, 5);
	printf("CSC10002 - PROGRAMMING TECHNIQUES");
	GotoXY(51, 6);
	cout << "GAME: CROSS THE ROAD";
	GotoXY(49, 9);
	SetColor(4);
	cout << "  *******     *******";
	GotoXY(49, 10);
	cout << " *       *   *       *";
	GotoXY(49, 11);
	cout << "*         ***         *";
	GotoXY(49, 12);
	cout << " *                   *";
	GotoXY(49, 13);
	cout << "  *";
	SetColor(3);
	GotoXY(53, 13);
	cout << " SEE YOU AGAIN ";
	GotoXY(68, 13);
	SetColor(4);
	cout << " *";
	GotoXY(49, 14);
	cout << "    *             *";
	GotoXY(49, 15);
	cout << "      *         *";
	GotoXY(49, 16);
	cout << "        *     *";
	GotoXY(49, 17);
	cout << "          ***";
	GabageCollect();
	GotoXY(49, 30);
	exit(0);
}

void PlayGame(int idx)
{
	char temp;
	StartGame();

	if (idx != 0)
	{
		updateData(idx);
		DrawCars("*");
	}
	thread t1(SubThread);
	for (int i = 1; i <= Num_alive; i++)
	{
		POINT a;
		a.x = alive[i].x;
		a.y = alive[i].y;
		DrawSticker(a, "Y");
	}
	if (flat)
	{
		PauseGame(t1.native_handle());
		flat = false;
	}
	while (true)
	{
		temp = toupper(_getch());
		if (STATE)
		{
			if (temp == 27) {
				Pause(t1.native_handle());
				system("cls");
				GotoXY(50, 9);
				cout << "EXITING ....";
				Sleep(500);
				ByeBye();
			}
			else if (temp == 'P') {
				PauseGame(t1.native_handle());
			}
			else if (temp == 'L')
			{
				Pause(t1.native_handle());
				string name;
				SetColor(0);
				do
				{
					GotoXY(40, HEIGH_CONSOLE + 3);
					cout << "                                                    ";
					GotoXY(50, HEIGH_CONSOLE + 4);
					cout << "                                                    ";
					SetColor(4);
					GotoXY(50, HEIGH_CONSOLE + 2);
					cout << "Enter name:                                                                      ";
					SetColor(3);
					GotoXY(62, HEIGH_CONSOLE + 2);
					cin >> name;
					if (!checkName(name))
					{
						GotoXY(40, HEIGH_CONSOLE + 3);
						SetColor(4);
						cout << "This name is coincide, Please enter another name !!!";
						SetColor(7);
						GotoXY(50, HEIGH_CONSOLE + 4);
						system("pause");
					}
				} while (!checkName(name));
				SaveGame(name, Y);
				PauseGame(t1.native_handle());
			}
			else {
				ResumeThread((HANDLE)t1.native_handle());
				if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S')
					MOVING = temp;
			}

		}
		else
		{
			if (temp == 'Y') StartGame();
			else
				ByeBye();
		}

	}
}

void Menu()
{
	int choice;
	TextColor(252);
	do
	{
		Notice(choice);
		switch (choice)
		{
		case (1):
			EnterGame(choice);
			break;
		case (2):
			Rule();
			break;
		case (3):
			About();
			break;
		}
	} while (choice != 4);
	if (choice == 4) ByeBye();
}

void SubThread()
{
	while (true) {
		if (STATE) // if passer alive
		{
			switch (MOVING) // check moving var
			{
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			MOVING = ' ';// temporary dont allow move, wait to receive keyboard in main function
			EraseCars();
			MoveCars();
			DrawCars("*");
			if (IsImpact(Y, Y.y))
				ProcessDead(); // check whether accident
			if (Y.y == 1)
				ProcessFinish(Y, Num_alive); // check whether finish
			Sleep(FIRST_SPEED);	//sleep base on SPEED
		}
	}
}
