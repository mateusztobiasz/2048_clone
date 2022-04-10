// template.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "template.h"
#include <time.h>
#include <set>
#include <map>
#include <fstream>
#include <iostream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szChild[MAX_LOADSTRING];
HWND childWndPar[16];
HWND childWndSec[16];
HWND pointsPar;
HWND pointsSec;
int game[4][4] = { 0 };
HWND parWnd = NULL;
HWND secWnd = NULL;
std::map<int, std::tuple<int,int,int>> numbersColors;

int left = 0;
int right = 0;
int top = 0;
int bottom = 0;

int leftP = 0;
int rightP = 0;
int topP = 0;
int bottomP = 0;


RECT recL;

int goal = 2048;
int points = 0;
bool ifEnd = false;
bool ifReached = false;
bool ifMovePossible = true;







// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClassChild(HINSTANCE hInstance);
ATOM MyRegisterClass(HINSTANCE hInstance);
void MakeMove(char c, bool& b);
void ColorWindow(HWND h1,HWND h2, std::tuple<int, int, int> el);
void WriteNumber(HWND h1, HWND h2, int v,const TCHAR* t=nullptr);
std::pair<int, int> PutRandom2();
void MakeMap();
void UpdateWindows();
void UnCheckAllAndCheckRight(int v);
void WinOrLoseScreen(std::tuple<int,int,int> colors);
void MakeSemiTransparent(HWND hWnd,int war);
bool CheckForMoves();
bool ReadFile();
void WriteFile();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)

{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TEMPLATE, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_CHILD, szChild, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	MyRegisterClassChild(hInstance);
	MakeMap();
	

	srand(time(NULL));

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEMPLATE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		/*InvalidateRect(parWnd, NULL, TRUE);
		UpdateWindow(parWnd);*/
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
// 
// 
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEMPLATE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));






	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex1;

	wcex1.cbSize = sizeof(WNDCLASSEX);

	wcex1.style = CS_HREDRAW | CS_VREDRAW;
	wcex1.lpfnWndProc = WndProcChild;
	wcex1.cbClsExtra = 0;
	wcex1.cbWndExtra = 0;
	wcex1.hInstance = hInstance;
	wcex1.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEMPLATE));
	wcex1.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex1.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(204, 192, 174)));
	wcex1.lpszMenuName = MAKEINTRESOURCEW(IDC_TEMPLATE);
	wcex1.lpszClassName = szChild;
	wcex1.hIconSm = LoadIcon(wcex1.hInstance, MAKEINTRESOURCE(IDI_SMALL));






	return RegisterClassExW(&wcex1);
}

void MakeMap() {

	std::tuple<int, int, int> tabRGB[11];

	tabRGB[0] = std::make_tuple(238, 228, 198);
	tabRGB[1] = std::make_tuple(239, 225, 218);
	tabRGB[2] = std::make_tuple(243, 179, 124);
	tabRGB[3] = std::make_tuple(246, 153, 100);
	tabRGB[4] = std::make_tuple(246, 125, 98);
	tabRGB[5] = std::make_tuple(247, 93, 60);
	tabRGB[6] = std::make_tuple(237, 206, 116);
	tabRGB[7] = std::make_tuple(239, 204, 98);
	tabRGB[8] = std::make_tuple(243, 201, 85);
	tabRGB[9] = std::make_tuple(238, 200, 72);
	tabRGB[10] = std::make_tuple(239, 192, 47);
	
	int key = 2;
	for (int i = 0; i < 11; i++)
	{
		numbersColors.insert(std::make_pair(key, tabRGB[i]));
		key *= 2;
	}
}
void MakeWindowCenetered(HWND hWnd) {
	int res_w = GetSystemMetrics(SM_CXSCREEN);
	int res_h = GetSystemMetrics(SM_CYSCREEN);

	RECT rc;

	GetWindowRect(hWnd, &rc);

	int win_width = rc.right - rc.left;
	int win_height = rc.bottom - rc.top;

	int center_x = (res_w - win_width) / 2;
	int center_y = (res_h - win_height) / 2;
	MoveWindow(hWnd, center_x, center_y, win_width, win_height, TRUE);
}
std::pair<int,int> PutRandom2() {
	bool notzero = false;
	while (notzero == false) {
		int i = rand() % 4;
		int j = rand() % 4;
		if (game[i][j] == 0) {
			notzero = true;
			game[i][j] = 2;
			return std::make_pair(i, j);
		}
	}
}
void MakeMove(char c, bool& ifReached) {

	std::pair<int, int> paraCor;
	int startI = 0, startJ = 0, stepI = 1, stepJ = 1;
	switch (c) {
	case 'w':
		paraCor.first = -1;
		paraCor.second = 0;
		break;
	case 'a':
		paraCor.first = 0;
		paraCor.second = -1;
		break;
	case 's':
		startI = 3;
		stepI = -1;
		paraCor.first = 1;
		paraCor.second = 0;
		break;
	case 'd':
		startJ = 3;
		stepJ = -1;
		paraCor.first = 0;
		paraCor.second = 1;
		break;
	}
	int alreadySummed[4][4] = { 0 };
	bool putRandom2 = false;
	bool canMove = false;
	do
	{
		canMove = false;
		for (int i = startI; i >= 0 && i < 4; i += stepI)
		{
			for (int j = startJ; j >= 0 && j < 4; j += stepJ)
			{
				int i_ = i + paraCor.first, j_ = j + paraCor.second;
				if (alreadySummed[i][j]!=1 && game[i][j] != 0 && !(i_ < 0 || i_ >= 4 || j_ < 0 || j_ >= 4 || game[i_][j_] != game[i][j] && game[i_][j_] != 0)) {
					putRandom2 = true;
					canMove = true;
					int tmp = game[i_][j_];
					game[i_][j_] += game[i][j];
					if (game[i_][j_] == goal) ifReached = true;
					if (game[i_][j_] == 2 * tmp) {
						points += tmp * 2;
						alreadySummed[i_][j_] = 1;
					}
					game[i][j] = 0;
				}

			}
		}
	} while (canMove);

	if (putRandom2) {
		PutRandom2();
	}


}

void ColorWindow(HWND winPar,HWND winSec,std::tuple<int,int,int> el) {
	auto hdcPar = GetDC(winPar);
	auto hdcSec = GetDC(winSec);
	RECT rcPar, rcSec;
	GetClientRect(winPar, &rcPar);
	GetClientRect(winSec, &rcSec);
	HBRUSH brush = CreateSolidBrush(RGB(std::get<0>(el), std::get<1>(el), std::get<2>(el)));
	FillRect(hdcPar, &rcPar, brush);
	FillRect(hdcSec, &rcSec, brush);
	DeleteObject(brush);
	ReleaseDC(winPar, hdcPar);
	ReleaseDC(winSec, hdcSec);
}
void WriteNumber(HWND winPar,HWND winSec, int number,const TCHAR* sl) {
	TCHAR s[256];
	if(!sl)_stprintf_s(s, 256, _T("%d"), number);
	else _stprintf_s(s, 256, _T("%s"), sl);
	auto hdcPar = GetDC(winPar);
	auto hdcSec = GetDC(winSec);
	RECT rcPar, rcSec;
	GetClientRect(winPar, &rcPar);
	GetClientRect(winSec, &rcSec);
	SetBkMode(hdcPar, TRANSPARENT);
	SetBkMode(hdcSec, TRANSPARENT);
	SetTextColor(hdcPar, RGB(255, 255, 255));
	SetTextColor(hdcSec, RGB(255, 255, 255));
	HFONT fontPar = CreateFont(
		- MulDiv(24, GetDeviceCaps(hdcPar, LOGPIXELSY), 72), // Height
		0, // Width
		 0, // Escapement
		 0, // Orientation
		 FW_BOLD, // Weight
		 false, // Italic
		 FALSE, // Underline
		
		 0, // StrikeOut
		 EASTEUROPE_CHARSET, // CharSet
		 OUT_DEFAULT_PRECIS, // OutPrecision
		 CLIP_DEFAULT_PRECIS, // ClipPrecision
		 DEFAULT_QUALITY, // Quality
		 DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
		 _T(" Arial ")); // Facename
	HFONT fontSec = CreateFont(
		-MulDiv(24, GetDeviceCaps(hdcSec, LOGPIXELSY), 72), // Height
		0, // Width
		0, // Escapement
		0, // Orientation
		FW_BOLD, // Weight
		false, // Italic
		FALSE, // Underline

		0, // StrikeOut
		EASTEUROPE_CHARSET, // CharSet
		OUT_DEFAULT_PRECIS, // OutPrecision
		CLIP_DEFAULT_PRECIS, // ClipPrecision
		DEFAULT_QUALITY, // Quality
		DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
		_T(" Arial ")); // Facename
	HFONT oldFontPar = (HFONT)SelectObject(hdcPar, fontPar);
	HFONT oldFontSec = (HFONT)SelectObject(hdcSec, fontSec);
	DrawText(hdcPar, s, (int)_tcslen(s), &rcPar,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DrawText(hdcSec, s, (int)_tcslen(s), &rcSec,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	DeleteObject(fontPar);
	DeleteObject(fontSec);
	ReleaseDC(winPar, hdcPar);
	ReleaseDC(winSec, hdcSec);
}
void UpdateWindows() {
	ColorWindow(pointsPar, pointsSec, std::make_tuple(204, 192, 174));
	WriteNumber(pointsPar, pointsSec, points);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			switch (game[i][j]) {
			case 0:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j],std::make_tuple(204, 192, 174));
				
				break;
			case 2:
				
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(2));
				WriteNumber(childWndPar[4 * i + j],childWndSec[4*i+j], 2);
				
				break;
			case 4:
				
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(4));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 4);
				
				break;
			case 8:
				
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(8));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 8);
				
				break;
			case 16:
				
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(16));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 16);
				
				break;
			case 32:
				
				
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(32));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 32);
				break;
			case 64:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(64));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 64);
				break;
			case 128:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(128));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 128);
				break;
			case 256:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(256));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 256);
				break;
			case 512:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(512));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 512);
				break;
			case 1024:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(1024));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 1024);
				break;
			case 2048:
				ColorWindow(childWndPar[4 * i + j], childWndSec[4 * i + j], numbersColors.at(2048));
				WriteNumber(childWndPar[4 * i + j], childWndSec[4 * i + j], 2048);
				break;
			
			}
			
			
		}
	}
}

void UnCheckAllAndCheckRight(int v) {
	CheckMenuItem(GetMenu(parWnd), ID_GOAL_8, MF_UNCHECKED);
	CheckMenuItem(GetMenu(parWnd), ID_GOAL_16, MF_UNCHECKED);
	CheckMenuItem(GetMenu(parWnd), ID_GOAL_64, MF_UNCHECKED);
	CheckMenuItem(GetMenu(parWnd), ID_GOAL_2048, MF_UNCHECKED);

	CheckMenuItem(GetMenu(secWnd), ID_GOAL_8, MF_UNCHECKED);
	CheckMenuItem(GetMenu(secWnd), ID_GOAL_16, MF_UNCHECKED);
	CheckMenuItem(GetMenu(secWnd), ID_GOAL_64, MF_UNCHECKED);
	CheckMenuItem(GetMenu(secWnd), ID_GOAL_2048, MF_UNCHECKED);

	switch (v) {
	case 8:
		CheckMenuItem(GetMenu(parWnd), ID_GOAL_8, MF_CHECKED);
		CheckMenuItem(GetMenu(secWnd), ID_GOAL_8, MF_CHECKED);
		break;
	case 16:
		CheckMenuItem(GetMenu(parWnd), ID_GOAL_16, MF_CHECKED);
		CheckMenuItem(GetMenu(secWnd), ID_GOAL_16, MF_CHECKED);
		break;
	case 64:
		CheckMenuItem(GetMenu(parWnd), ID_GOAL_64, MF_CHECKED);
		CheckMenuItem(GetMenu(secWnd), ID_GOAL_64, MF_CHECKED);
		break;
	case 2048:
		CheckMenuItem(GetMenu(parWnd), ID_GOAL_2048, MF_CHECKED);
		CheckMenuItem(GetMenu(secWnd), ID_GOAL_2048, MF_CHECKED);
		break;


	}
}

void WinOrLoseScreen(std::tuple<int, int, int> colors) {
	RECT rcPar, rcSec;
	GetClientRect(parWnd, &rcPar);
	GetClientRect(secWnd, &rcSec);	

	
	auto hdcPar = GetDC(parWnd);
	auto hdcSec = GetDC(secWnd);
	auto hdcBitPar = CreateCompatibleDC(hdcPar);
	auto hdcBitSec = CreateCompatibleDC(hdcSec);

	auto bitmapPar = CreateCompatibleBitmap(hdcPar, rcPar.right - rcPar.left, rcPar.bottom - rcPar.top);
	auto bitmapSec = CreateCompatibleBitmap(hdcSec,rcSec.right-rcSec.left,rcSec.bottom-rcSec.top);

	auto oldBitPar = (HBITMAP)SelectObject(hdcBitPar, bitmapPar);
	auto oldBitSec = (HBITMAP)SelectObject(hdcBitSec, bitmapSec);

	auto brush = CreateSolidBrush(RGB(std::get<0>(colors), std::get<1>(colors), std::get<2>(colors)));

	FillRect(hdcBitPar, &rcPar, brush);
	FillRect(hdcBitSec, &rcSec, brush);

	DeleteObject(brush);

	BLENDFUNCTION fcn;

	fcn.AlphaFormat = 0;
	fcn.BlendFlags = 0;
	fcn.BlendOp = AC_SRC_OVER;
	fcn.SourceConstantAlpha = 136;
	AlphaBlend(hdcPar, rcPar.left, rcPar.top, rcPar.right - rcPar.left, rcPar.bottom - rcPar.top, hdcBitPar,
		rcPar.left, rcPar.top, rcPar.right - rcPar.left, rcPar.bottom - rcPar.top, fcn);
	AlphaBlend(hdcSec, rcSec.left, rcSec.top, rcSec.right - rcSec.left, rcSec.bottom - rcSec.top, hdcBitSec,
		rcSec.left, rcSec.top, rcSec.right - rcSec.left, rcSec.bottom - rcSec.top, fcn);

	DeleteObject(oldBitPar);
	DeleteObject(oldBitSec);
	ReleaseDC(parWnd, hdcPar);
	ReleaseDC(secWnd, hdcSec);
	DeleteDC(hdcBitPar);
	DeleteDC(hdcBitSec);

	
}

bool CheckForMoves() {
	
	for (int i = 0; i < 4; i++)
	{
		int j_ = 1;
		for (int j = 0; j < 4; j++)
		{
			if (game[i][j] == 0) return true;
			if (j_ < 4 && game[i][j] == game[i][j_]) return true;
			j_++;
		}
	}

	
	for (int j = 0; j < 4; j++)
	{
		int i_ = 1;
		for (int i = 0;  i< 4; i++)
		{
			
			if (i_ < 4 && game[i][j] == game[i_][j]) return true;
			i_++;
		}
	}

	return false;

}

void MakeSemiTransparent(HWND hWnd, int war) {
	// kod na polprzezroczysty ekran

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	SetLayeredWindowAttributes(hWnd, 0, (255 * war) / 100, LWA_ALPHA);
}

bool ReadFile() {
	std::ifstream  stream;
	stream.open("2048.ini", std::ios::in);
	if (!stream.is_open()) {
		return false;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::string number;
			stream >> number;
			game[i][j] = atoi(number.c_str());
		}
	}
	std::string tmp;
	stream >> tmp;
	points = atoi(tmp.c_str());

	stream >> tmp;
	goal = atoi(tmp.c_str());

	stream >> tmp;

	if (atoi(tmp.c_str()) == 1) ifReached = true;
	else ifReached = false;

	stream >> tmp;

	if (atoi(tmp.c_str()) == 1) ifMovePossible = true;
	else ifMovePossible = false;

	


	stream.close();

	UpdateWindows();
	UnCheckAllAndCheckRight(goal);

	if (ifReached) {
		WinOrLoseScreen(std::make_tuple(66, 245, 69));
		WriteNumber(parWnd, secWnd, 2, _T("WIN!!"));
		ifEnd = true;
	}
	if (!ifMovePossible) {
		WinOrLoseScreen(std::make_tuple(252, 40, 69));
		WriteNumber(parWnd, secWnd, 2, _T("LOSE!!"));
		ifEnd = true;
	}

	return true;
}
void WriteFile() {
	std::ofstream  stream;
	stream.open("2048.ini", std::ios::out | std::ios::trunc);

	if (!stream.is_open()) {
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stream << game[i][j] << " ";
		}
	}
	stream << std::endl;

	stream << points << std::endl;

	stream << goal << std::endl;

	if (ifReached) stream << 1 << std::endl;
	else stream << 0 << std::endl;

	if (ifMovePossible) stream << 1 << std::endl;
	else stream << 0 << std::endl;

	stream.close();
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	RECT rc = { 0,0,4 * 60 + 5 * 10,4 * 60 + 5 * 10+80 };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
		100, 100, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	HWND hWnd1 = CreateWindowEx(WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_VISIBLE,
		1000, 100, rc.right - rc.left, rc.bottom - rc.top, hWnd, nullptr, hInstance, nullptr);
	
	RECT rce, rces;
	GetWindowRect(hWnd, &rce);
	left = rce.left;
	right = rce.right;
	top = rce.top;
	bottom = rce.bottom;

	leftP = rce.left;
	rightP = rce.right;
	topP = rce.top;
	bottomP = rce.bottom;
	GetWindowRect(hWnd1, &rces);

	recL = rces;


	parWnd = hWnd;
	secWnd = hWnd1;

	CheckMenuItem(GetMenu(parWnd), ID_GOAL_2048, MF_CHECKED);
	CheckMenuItem(GetMenu(secWnd), ID_GOAL_2048, MF_CHECKED);

	
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWnd1, nCmdShow);
	
	UpdateWindow(hWnd);
	UpdateWindow(hWnd1);

	

	



	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static std::pair<int, int> para;
	
	switch (message)
	{

	case WM_TIMER:
	{
	
			if (wParam == 3) {
				
				if (ReadFile()) {
					KillTimer(hWnd, 3);
					break;
				}
				
				ColorWindow(childWndPar[para.first * 4 + para.second],childWndSec[para.first * 4 + para.second],numbersColors.at(2));
				WriteNumber(childWndPar[para.first * 4 + para.second],childWndSec[para.first * 4 + para.second],2);
				WriteNumber(pointsPar, pointsSec, points);
				KillTimer(hWnd, 3);
			}
			
		
		
	}
	break;

	case WM_CREATE:
	{
		static bool visited = false;
		
		if (!visited) {
			para = PutRandom2();
		}
		
		int index = 0;
		int index1 = 0;
		auto region1 = CreateRoundRectRgn(0, 0, 3 * 10 + 4 * 60, 70, 10, 10);
		if (!visited) {
			
			pointsPar = CreateWindowEx(0, szChild, (LPCWSTR)NULL, WS_CHILDWINDOW, 10 , 10, 3*10+4*60,
				70,
				hWnd,
				NULL,
				hInst,
				NULL);
			SetWindowRgn(pointsPar, region1, TRUE);
			ShowWindow(pointsPar, SW_SHOW);
			UpdateWindow(pointsPar);
		}
		else{
			pointsSec = CreateWindowEx(0, szChild, (LPCWSTR)NULL, WS_CHILDWINDOW, 10, 10, 3 * 10 + 4 * 60,
				70,
				hWnd,
				NULL,
				hInst,
				NULL);
			SetWindowRgn(pointsSec, region1, TRUE);
			ShowWindow(pointsSec, SW_SHOW);
			UpdateWindow(pointsSec);
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				auto region = CreateRoundRectRgn(0, 0, 60, 60, 10, 10);
				if (!visited) {
					
					
					
					childWndPar[index] = CreateWindowEx(0, szChild, (LPCWSTR)NULL, WS_CHILDWINDOW, 10 + 70 * j, 90 + 70 * i, 60,
						60,
						hWnd,
						NULL,
						hInst,
						NULL);
					SetWindowRgn(childWndPar[index], region, TRUE);
					
					ShowWindow(childWndPar[index], SW_SHOW);
					UpdateWindow(childWndPar[index]);
					
					
					index++;
					
				}
				else if(visited){
					
					
					childWndSec[index1] = CreateWindowEx(0, szChild, (LPCWSTR)NULL, WS_CHILDWINDOW, 10 + 70 * j, 90 + 70 * i, 60,
						60,
						hWnd,
						NULL,
						hInst,
						NULL);
					SetWindowRgn(childWndSec[index1], region, TRUE);
					ShowWindow(childWndSec[index1], SW_SHOW);
					UpdateWindow(childWndSec[index1]);
					DeleteObject(region);
					index1++;
					
				}




				
			}
		}

		DeleteObject(region1);
		if (visited) {
			SetTimer(hWnd, 3, 100, NULL); SetTimer(hWnd, 4, 100, NULL);
		}
		
		visited = true;
	
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		
		switch (wmId)
		{
		case ID_GOAL_8:
			UnCheckAllAndCheckRight(8);
			goal = 8;
			break;
		case ID_GOAL_16:
			UnCheckAllAndCheckRight(16);
			goal = 16;
			break;
		case ID_GOAL_64:
			UnCheckAllAndCheckRight(64);
			goal = 64;
			break;
		case ID_GOAL_2048:
			UnCheckAllAndCheckRight(2048);
			goal = 2048;
			break;
		case ID_NEW:
			ifReached = false;
			ifEnd = false;
			ifMovePossible = true;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					game[i][j] = 0;
				}
			}
			points = 0;
			PutRandom2();
			ColorWindow(parWnd, secWnd, std::make_tuple(250, 247, 238));
			UpdateWindows();
			
				break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		
		if (ifEnd) break;
		switch (wParam) {
			case 0x57: // w
			{
				
				MakeMove('w',ifReached);
				
				
			}
			break;
			case 0x41: { // a
				MakeMove('a',ifReached);
				
			}
			break;
			case 0x53: { // s
				
				MakeMove('s',ifReached);
				
				
			}
			break;
			case 0x44:{ // d
				MakeMove('d',ifReached);
				
			}
			break;

		}
		ifMovePossible =CheckForMoves();
		UpdateWindows();
		if (ifReached) {
			WinOrLoseScreen(std::make_tuple(66, 245, 69));
			WriteNumber(parWnd, secWnd, 2, _T("WIN!!"));
			ifEnd = true;
		}
		if (!ifMovePossible) {
			WinOrLoseScreen(std::make_tuple(252, 40, 69));
			WriteNumber(parWnd, secWnd, 2, _T("LOSE!!"));
			ifEnd = true;
			//ifMovePossible = true;
		}
		

	}
	break;
	case WM_DESTROY:
		WriteFile();
		PostQuitMessage(0);
		break;
	case WM_SIZING:
	{


		RECT* rec = (RECT*)lParam;

		rec->left = left;
		rec->right = right;
		rec->bottom = bottom;
		rec->top = top;
	}
	
	case WM_MOVE:
	{
		
		RECT rc;
		
		GetWindowRect(hWnd, &rc);
		left = rc.left;
		right = rc.right;
		top = rc.top;
		bottom = rc.bottom;
		
		
		const int correct = 50;

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		// poruszanie siê wzglêdem œrodka ekranu

		if (hWnd == parWnd) {
			MoveWindow(secWnd, width - (int)LOWORD(lParam) - (rightP - leftP), height - (int)HIWORD(lParam) - (bottomP - topP), recL.right - recL.left, recL.bottom - recL.top, TRUE);
			
		}
		else if (hWnd == secWnd) {
			MoveWindow(parWnd, width - (int)LOWORD(lParam) - (recL.right - recL.left), height - (int)HIWORD(lParam) - (recL.bottom - recL.top),
				rightP - leftP, bottomP - topP, TRUE);

			int xPar = width - (int)LOWORD(lParam) - (recL.right - recL.left);
			int yPar = height - (int)HIWORD(lParam) - (recL.bottom - recL.top);
			RECT clRec;
			GetClientRect(hWnd, &clRec);
			if (xPar + clRec.right > rc.left && rc.left+clRec.right>=xPar &&
				yPar + clRec.bottom +correct >= rc.top &&
				 rc.top+clRec.bottom+correct >=yPar
				)
			{
				MakeSemiTransparent(hWnd,50);
				
			}
			else {
				MakeSemiTransparent(hWnd, 100);
			}
			UpdateWindow(hWnd);

		}

		

		




	}
	break;
	
		

	default:
		

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 2, 1000, NULL);
		break;
	case WM_TIMER:


		//UpdateWindow(childWnd);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	/*case WM_DESTROY:
		PostQuitMessage(0);
		break;*/
	case WM_CHAR:
		break;
	case WM_KEYDOWN:


		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
