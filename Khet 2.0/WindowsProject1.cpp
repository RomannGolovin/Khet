// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"
#include <Windows.h>
#include <vector>
#include <map>
#include <string>

#define MAX_LOADSTRING 100

static HINSTANCE hInst; // текущий экземпляр
static WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
static WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна
static HBITMAP hBitmap = NULL; //Указатель для массива изображений

enum Side {
	SIDE_RED,
	SIDE_WHITE,
	SIDE_NONE
};

enum KhetFigureType {
	FIGURE_TYPE_ANUBIS,
	FIGURE_TYPE_PHARAOH,
	FIGURE_TYPE_PYRAMID,
	FIGURE_TYPE_SCARAB,
	FIGURE_TYPE_SPHINX,
	FIGURE_TYPE_NONE
};

enum Direction {
	DIRECTION_DOWN = 0,
	DIRECTION_LEFT,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_NONE
};

class KhetFigure {
public:
	KhetFigureType type;
	Direction rotation;
	Side side;

	KhetFigure(KhetFigureType type = KhetFigureType::FIGURE_TYPE_NONE, Direction rotation = Direction::DIRECTION_NONE,
		Side side = Side::SIDE_NONE) : type(type), rotation(rotation), side(side) {}

	bool operator<(const KhetFigure &other) const {
		if (type != other.type) return other.type > type;
		if (rotation != other.rotation) return other.rotation > rotation;
		return other.side > side;
	}
};

static std::vector<std::vector<KhetFigure>> field;
static std::map<KhetFigure, HBITMAP> bitmaps;
static Side current_side = SIDE_RED;
static std::vector<std::pair<Direction, std::pair<int, int>>> laser;
static Side winner = SIDE_NONE;

static void LoadBitmap(std::string name, KhetFigureType type, Side side) {
	std::string tok = name + (side == SIDE_RED ? "Red" : (side == SIDE_WHITE ? "White" : ""));
	bool a = KhetFigure(type, DIRECTION_DOWN, side) < KhetFigure(type, DIRECTION_LEFT, side);
	bool b = KhetFigure(type, DIRECTION_LEFT, side) < KhetFigure(type, DIRECTION_DOWN, side);
	bitmaps[KhetFigure(type, DIRECTION_DOWN, side)] =
		(HBITMAP)LoadImageA(hInst, (tok + "0.bmp").c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	bitmaps[KhetFigure(type, DIRECTION_LEFT, side)] =
		(HBITMAP)LoadImageA(hInst, (tok + "1.bmp").c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	bitmaps[KhetFigure(type, DIRECTION_UP, side)] =
		(HBITMAP)LoadImageA(hInst, (tok + "2.bmp").c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
	bitmaps[KhetFigure(type, DIRECTION_RIGHT, side)] =
		(HBITMAP)LoadImageA(hInst, (tok + "3.bmp").c_str(), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
}

static void LoadBitmaps() {
	bitmaps[KhetFigure(FIGURE_TYPE_NONE, DIRECTION_NONE, SIDE_NONE)] =
		(HBITMAP)LoadImageA(hInst, "Test.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

	LoadBitmap("Anub", FIGURE_TYPE_ANUBIS, SIDE_RED);
	LoadBitmap("Phar", FIGURE_TYPE_PHARAOH, SIDE_RED);
	LoadBitmap("Pyr", FIGURE_TYPE_PYRAMID, SIDE_RED);
	LoadBitmap("Scar", FIGURE_TYPE_SCARAB, SIDE_RED);
	LoadBitmap("Sfinx", FIGURE_TYPE_SPHINX, SIDE_RED);

	LoadBitmap("Anub", FIGURE_TYPE_ANUBIS, SIDE_WHITE);
	LoadBitmap("Phar", FIGURE_TYPE_PHARAOH, SIDE_WHITE);
	LoadBitmap("Pyr", FIGURE_TYPE_PYRAMID, SIDE_WHITE);
	LoadBitmap("Scar", FIGURE_TYPE_SCARAB, SIDE_WHITE);
	LoadBitmap("Sfinx", FIGURE_TYPE_SPHINX, SIDE_WHITE);
}

static void InitializeField() {
	field.resize(10);
	for (int i = 0; i < field.size(); ++i) {
		field[i].resize(8);
	}
}

static std::pair<int, int> DirectionToCoords(Direction direction) {
	switch (direction) {
		case DIRECTION_UP: return std::pair<int, int>(0, -1);
		case DIRECTION_RIGHT: return std::pair<int, int>(1, 0);
		case DIRECTION_DOWN: return std::pair<int, int>(0, 1);
		case DIRECTION_LEFT: return std::pair<int, int>(-1, 0);
	}
	return std::pair<int, int>(0, 0);
}

// Returns pair of cells travelled and cells to remove
static std::pair<std::vector<std::pair<Direction, std::pair<int, int>>>, std::vector<std::pair<int, int>>> ConstructLaser() {
	std::vector<std::pair<Direction, std::pair<int, int>>> travelled;
	std::vector<std::pair<int, int>> removed;
	int x, y;
	Direction laser_direction;

	if (current_side == SIDE_RED) {
		std::pair<int, int> d = DirectionToCoords(field[0][0].rotation);
		x = d.first;
		y = d.second;
		laser_direction = field[0][0].rotation;
	} else {
		std::pair<int, int> d = DirectionToCoords(field[9][7].rotation);
		x = d.first + 9;
		y = d.second + 7;
		laser_direction = field[9][7].rotation;
	}

	while (x >= 0 && x < 10 && y >= 0 && y < 10) {
		travelled.push_back(std::pair<Direction, std::pair<int, int>>(laser_direction, std::pair<int, int>(x, y)));
		std::pair<int, int> d = DirectionToCoords(laser_direction);
		int next_x = x + d.first;
		int next_y = y + d.second;
		if (next_x < 0 || next_x >= 10 || next_y < 0 || next_y >= 8) break;
		KhetFigure next = field[next_x][next_y];

		if (next.type != FIGURE_TYPE_NONE) {
			Direction laser_local_direction = (Direction)((4 + laser_direction - next.rotation) % 4);
			if (next.type == FIGURE_TYPE_PHARAOH) {
				removed.push_back(std::pair<int, int>(next_x, next_y));
				break;
			} else if (next.type == FIGURE_TYPE_PYRAMID && 
				(laser_local_direction == DIRECTION_UP || laser_local_direction == DIRECTION_RIGHT)) {
				removed.push_back(std::pair<int, int>(next_x, next_y));
				break;
			} else if (next.type == FIGURE_TYPE_ANUBIS) {
				if (laser_local_direction != DIRECTION_UP)
					removed.push_back(std::pair<int, int>(next_x, next_y));
				break;
			} else if (next.type == FIGURE_TYPE_SPHINX) {
				break;
			}
			switch (laser_local_direction) {
				case DIRECTION_DOWN: laser_direction = (Direction)((4 + laser_direction - 1) % 4); break;
				case DIRECTION_LEFT: laser_direction = (Direction)((4 + laser_direction + 1) % 4); break;
				case DIRECTION_UP: laser_direction = (Direction)((4 + laser_direction - 1) % 4); break;
				case DIRECTION_RIGHT: laser_direction = (Direction)((4 + laser_direction + 1) % 4); break;
			}
		}

		x = next_x;
		y = next_y;
	}

	return std::pair<std::vector<std::pair<Direction, std::pair<int, int>>>, std::vector<std::pair<int, int>>>(travelled, removed);
}

// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
// ФУНКЦИЯ: MyRegisterClass()
//
// ЦЕЛЬ: Регистрирует класс окна.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	RECT window_rect;
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 1000;
	window_rect.bottom = 800;
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, true);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
		CW_USEDEFAULT, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


int from_x = -1, from_y = -1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_CREATE:
		LoadBitmaps();
		InitializeField();
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 8; y++) {
				field[x][y] = KhetFigure();
			}
		}
		field[0][0] = KhetFigure(FIGURE_TYPE_SPHINX, DIRECTION_DOWN, SIDE_RED);
		field[0][3] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_DOWN, SIDE_RED);
		field[0][4] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_LEFT, SIDE_RED);
		field[2][1] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_UP, SIDE_RED);
		field[7][0] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_LEFT, SIDE_RED);
		field[7][3] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_LEFT, SIDE_RED);
		field[7][4] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_DOWN, SIDE_RED);
		field[6][5] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_LEFT, SIDE_RED);
		field[4][3] = KhetFigure(FIGURE_TYPE_SCARAB, DIRECTION_UP, SIDE_RED);
		field[5][3] = KhetFigure(FIGURE_TYPE_SCARAB, DIRECTION_RIGHT, SIDE_RED);
		field[5][0] = KhetFigure(FIGURE_TYPE_PHARAOH, DIRECTION_DOWN, SIDE_RED);
		field[4][0] = KhetFigure(FIGURE_TYPE_ANUBIS, DIRECTION_DOWN, SIDE_RED);
		field[6][0] = KhetFigure(FIGURE_TYPE_ANUBIS, DIRECTION_DOWN, SIDE_RED);

		field[9][7] = KhetFigure(FIGURE_TYPE_SPHINX, DIRECTION_UP, SIDE_WHITE);
		field[9][4] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_UP, SIDE_WHITE);
		field[9][3] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_RIGHT, SIDE_WHITE);
		field[7][6] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_DOWN, SIDE_WHITE);
		field[2][7] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_RIGHT, SIDE_WHITE);
		field[2][4] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_RIGHT, SIDE_WHITE);
		field[2][3] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_UP, SIDE_WHITE);
		field[3][2] = KhetFigure(FIGURE_TYPE_PYRAMID, DIRECTION_RIGHT, SIDE_WHITE);
		field[5][4] = KhetFigure(FIGURE_TYPE_SCARAB, DIRECTION_DOWN, SIDE_WHITE);
		field[4][4] = KhetFigure(FIGURE_TYPE_SCARAB, DIRECTION_LEFT, SIDE_WHITE);
		field[4][7] = KhetFigure(FIGURE_TYPE_PHARAOH, DIRECTION_UP, SIDE_WHITE);
		field[5][7] = KhetFigure(FIGURE_TYPE_ANUBIS, DIRECTION_UP, SIDE_WHITE);
		field[3][7] = KhetFigure(FIGURE_TYPE_ANUBIS, DIRECTION_UP, SIDE_WHITE);
		break;

	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc;
		HDC w_hdc;
		BITMAP bitmap;
		BITMAP AnubRed;
		HDC hdcMem;
		HGDIOBJ oldBitmap;
		RECT clientRect;
		RECT textRect;
		HRGN bgRgn;
		HBRUSH hBrush;
		HPEN hPen;

		int x, y;


		w_hdc = BeginPaint(hWnd, &ps);
		hdc = CreateCompatibleDC(w_hdc);
		BITMAP structBitmapHeader;
		HGDIOBJ hBitmap = GetCurrentObject(w_hdc, OBJ_BITMAP);
		GetObject(hBitmap, sizeof(BITMAP), &structBitmapHeader);
		HBITMAP bmp = CreateCompatibleBitmap(w_hdc, structBitmapHeader.bmWidth, structBitmapHeader.bmHeight);
		SelectObject(hdc, bmp);

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, bitmaps[KhetFigure()]);

		GetObject(bitmaps[KhetFigure()], sizeof(bitmap), &bitmap);
		for (x = 0; x < 10; x++) {
			for (y = 0; y < 8; y++) {
				BitBlt(hdc, bitmap.bmWidth * x, bitmap.bmHeight * y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
			}
		}


		hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 255, 255));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0)));
		for (int x = 0; x < 10; ++x) {
			for (int y = 0; y < 8; ++y) {
				oldBitmap = SelectObject(hdcMem, bitmaps[field[x][y]]);
				BitBlt(hdc, x * 100, y * 100, 100, 100, hdcMem, 0, 0, SRCCOPY);
			}
		}

		for (int i = 0; i < laser.size(); ++i) {
			std::pair<Direction, std::pair<int, int>> r = laser[i];
			int x = r.second.first;
			int y = r.second.second;
			SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
			if (i - 1 >= 0 && r.first != laser[i - 1].first) {
				if ((laser[i - 1].first == DIRECTION_DOWN && r.first == DIRECTION_RIGHT) ||
					(laser[i - 1].first == DIRECTION_LEFT && r.first == DIRECTION_UP)) {
					Rectangle(hdc, x * 100 + 50, y * 100, x * 100 + 55, y * 100 + 50);
					Rectangle(hdc, x * 100 + 50, y * 100 + 50, x * 100 + 100, y * 100 + 55);
				}
				if ((laser[i - 1].first == DIRECTION_DOWN && r.first == DIRECTION_LEFT) ||
					(laser[i - 1].first == DIRECTION_RIGHT && r.first == DIRECTION_UP)) {
					Rectangle(hdc, x * 100 + 50, y * 100, x * 100 + 55, y * 100 + 50);
					Rectangle(hdc, x * 100, y * 100 + 50, x * 100 + 50, y * 100 + 55);
				}
				if ((laser[i - 1].first == DIRECTION_UP && r.first == DIRECTION_RIGHT) ||
					(laser[i - 1].first == DIRECTION_LEFT && r.first == DIRECTION_DOWN)) {
					Rectangle(hdc, x * 100 + 50, y * 100 + 50, x * 100 + 55, y * 100 + 100);
					Rectangle(hdc, x * 100 + 50, y * 100 + 50, x * 100 + 100, y * 100 + 55);
				}
				if ((laser[i - 1].first == DIRECTION_UP && r.first == DIRECTION_LEFT) ||
					(laser[i - 1].first == DIRECTION_RIGHT && r.first == DIRECTION_DOWN)) {
					Rectangle(hdc, x * 100 + 50, y * 100 + 50, x * 100 + 55, y * 100 + 100);
					Rectangle(hdc, x * 100, y * 100 + 50, x * 100 + 50, y * 100 + 55);
				}
			}
			else if (r.first == DIRECTION_DOWN || r.first == DIRECTION_UP) {
				Rectangle(hdc, x * 100 + 50, y * 100, x * 100 + 55, y * 100 + 100);
			}
			else if (r.first == DIRECTION_RIGHT || r.first == DIRECTION_LEFT) {
				Rectangle(hdc, x * 100, y * 100 + 50, x * 100 + 100, y * 100 + 55);
			}
		}

		if (winner != SIDE_NONE) {
			const char *text = (winner == SIDE_RED) ? "RED WON" : "WHITE WON";
			RECT r = { 230, 100 * 3, 0, 0 };
			SetTextColor(hdc, RGB(255, 255, 100));
			SetBkMode(hdc, TRANSPARENT);
			HFONT hFontOld = (HFONT)SelectObject(hdc, CreateFont(128, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Consolas")));
			DrawTextA(hdc, text, -1, &r, DT_SINGLELINE | DT_NOCLIP);
		}

		BitBlt(w_hdc, 0, 0, structBitmapHeader.bmWidth, structBitmapHeader.bmHeight, hdc, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN: {
		if (from_x < 0) {
			from_x = LOWORD(lParam) / 100;
			from_y = HIWORD(lParam) / 100;
			if ((from_x < 0) || (from_x > 9) || (from_y < 0) || (from_y > 7) || field[from_x][from_y].side != current_side) {
				from_x = -1;
				from_y = -1;
			}
			break;
		}

		int to_x = LOWORD(lParam) / 100;
		int to_y = HIWORD(lParam) / 100;
		if (to_x < 0 || to_x > 9 || to_x < 0 || to_y > 7) {
			from_x = -1;
			from_y = -1;
			break;
		}
		if (from_x == to_x && from_y == to_y) {
			if (message == WM_LBUTTONDOWN) {
				// Rotate left
				switch (field[from_x][from_y].rotation) {
					case DIRECTION_UP: field[from_x][from_y].rotation = DIRECTION_LEFT; break;
					case DIRECTION_RIGHT: field[from_x][from_y].rotation = DIRECTION_UP; break;
					case DIRECTION_DOWN: field[from_x][from_y].rotation = DIRECTION_RIGHT; break;
					case DIRECTION_LEFT: field[from_x][from_y].rotation = DIRECTION_DOWN; break;
				}
			} else {
				// Rotate right
				switch (field[from_x][from_y].rotation) {
					case DIRECTION_UP: field[from_x][from_y].rotation = DIRECTION_RIGHT; break;
					case DIRECTION_RIGHT: field[from_x][from_y].rotation = DIRECTION_DOWN; break;
					case DIRECTION_DOWN: field[from_x][from_y].rotation = DIRECTION_LEFT; break;
					case DIRECTION_LEFT: field[from_x][from_y].rotation = DIRECTION_UP; break;
				}
			}
		} else {
			if (std::abs(from_x - to_x) > 1 || std::abs(from_y - to_y) > 1) break;
			KhetFigure current = field[from_x][from_y];
			KhetFigure next = field[to_x][to_y];
			if (current.type == FIGURE_TYPE_SPHINX || 
				next.type == FIGURE_TYPE_SPHINX || next.type == FIGURE_TYPE_SCARAB || next.type == FIGURE_TYPE_PHARAOH ||
				(next.type != FIGURE_TYPE_NONE && current.type != FIGURE_TYPE_SCARAB)) {
				from_x = -1;
				from_y = -1;
				break;
			}
			field[from_x][from_y] = field[to_x][to_y];
			field[to_x][to_y] = current;	
		}
		from_x = -1;
		from_y = -1;
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		Sleep(300);

		std::pair<std::vector<std::pair<Direction, std::pair<int, int>>>, std::vector<std::pair<int, int>>> r = ConstructLaser();
		laser = r.first;

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		Sleep(300);
		
		for (std::pair<int, int> p : r.second) {
			if (field[p.first][p.second].type == FIGURE_TYPE_PHARAOH) {
				winner = (field[p.first][p.second].side == SIDE_RED) ? SIDE_WHITE : SIDE_RED;
			}
			field[p.first][p.second] = KhetFigure();
		}

		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		Sleep(300);

		laser.clear();
		InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);

		current_side = current_side == SIDE_RED ? SIDE_WHITE : SIDE_RED;

		break;
	}
	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);


	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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