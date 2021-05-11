// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"
#include <Windowsx.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
static HINSTANCE hInst;                                // текущий экземпляр
static WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
static WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
static HBITMAP hBitmap = NULL;

class ChessPiece {
	int pieceId; //0..10, 0-blank, odd - white pieces
	int rot; //rot=0(up)..3(left), should be 0 if id=0
public:
	ChessPiece(int id, int rotation) : pieceId(id), rot(rotation) {}
	int getId() { return pieceId; }
	int getRot() { return rot; }
	void setRot(int r) { rot = r; }
	void setId(int idd) { pieceId = idd; }
	int isValidMove(int fromx, int fromy, int tox, int toy)
	{
		return (abs(tox - fromx) == 1 && abs(toy - fromy) <= 1) || (abs(tox - fromx) <= 1 && abs(toy - fromy) == 1);
	}
	int laser(int k)
	{
		switch (k) {//0..3-направления, 5-уничтожение фигуры, 6-конец отображения, 7-конец игры
		case 0:
			if (pieceId == 0)
			{
				return k; break;
			}
			if (pieceId == 1 || pieceId == 2)
			{
				switch (rot)
				{
				case 0:return 6; break;//конец отображения
				case 1:return 5; break;//delete
				case 2:return 5; break;//delete
				case 3:return 5; break;//delete
				}
			}
			if (pieceId == 3 || pieceId == 4)
			{
				return 7; break;//game over
			}
			if (pieceId == 5 || pieceId == 6)
			{
				switch (rot)
				{
				case 0:return 5; break;//delete
				case 1:return 1; break;
				case 2:return 3; break;
				case 3:return 5; break;//delete
				}
			}
			if (pieceId == 7 || pieceId == 8)
			{
				switch (rot)
				{
				case 0:return 3; break;
				case 1:return 1; break;
				case 2:return 3; break;
				case 3:return 1; break;
				}
			}
			if (pieceId == 9 || pieceId == 10)
			{
				return 6; break;//конец отображения
			}
			break;
		case 1:
			if (pieceId == 0)
			{
				return k; break;
			}
			if (pieceId == 1 || pieceId == 2)
			{
				switch (rot)
				{
				case 0:return 5; break;//delete
				case 1:return 6; break;//конец отображения
				case 2:return 5; break;//delete
				case 3:return 5; break;//delete
				}
			}
			if (pieceId == 3 || pieceId == 4)
			{
				return 7; break;//game over
			}
			if (pieceId == 5 || pieceId == 6)
			{
				switch (rot)
				{
				case 0:return 5; break;//delete
				case 1:return 5; break;//delete
				case 2:return 2; break;
				case 3:return 0; break;
				}
			}
			if (pieceId == 7 || pieceId == 8)
			{
				switch (rot)
				{
				case 0:return 2; break;
				case 1:return 0; break;
				case 2:return 2; break;
				case 3:return 0; break;
				}
			}
			if (pieceId == 9 || pieceId == 10)
			{
				return 6; break;//конец отображения
			}
			break;
		case 2:
			if (pieceId == 0)
			{
				return k; break;
			}
			if (pieceId == 1 || pieceId == 2)
			{
				switch (rot)
				{
				case 0:return 5; break;//delete
				case 1:return 5; break;//delete
				case 2:return 6; break;//конец отображения
				case 3:return 5; break;//delete
				}
			}
			if (pieceId == 3 || pieceId == 4)
			{
				return 7; break;//game over
			}
			if (pieceId == 5 || pieceId == 6)
			{
				switch (rot)
				{
				case 0:return 1; break;
				case 1:return 5; break;//delete
				case 2:return 5; break;//delete
				case 3:return 3; break;
				}
			}
			if (pieceId == 7 || pieceId == 8)
			{
				switch (rot)
				{
				case 0:return 1; break;
				case 1:return 3; break;
				case 2:return 1; break;
				case 3:return 3; break;
				}
			}
			if (pieceId == 9 || pieceId == 10)
			{
				return 6; break;//конец отображения
			}
			break;
		case 3:
			if (pieceId == 0)
			{
				return k; break;
			}
			if (pieceId == 1 || pieceId == 2)
			{
				switch (rot)
				{
				case 0:return 5; break;//delete
				case 1:return 5; break;//delete
				case 2:return 5; break;//delete
				case 3:return 6; break;//конец отображения
				}
			}
			if (pieceId == 3 || pieceId == 4)
			{
				break; return 7; break;//game over
			}
			if (pieceId == 5 || pieceId == 6)
			{
				switch (rot)
				{
				case 0:return 0; break;
				case 1:return 2; break;
				case 2:return 5; break;//delete
				case 3:return 5; break;//delete
				}
			}
			if (pieceId == 7 || pieceId == 8)
			{
				switch (rot)
				{
				case 0:return 0; break;
				case 1:return 2; break;
				case 2:return 0; break;
				case 3:return 2; break;
				}
			}
			if (pieceId == 9 || pieceId == 10)
			{
				return 6; break;//конец отображения
			}
		}
	}
};
void laser()
{

}
int lasermap[10][8];
//for (int i = 0; i < 9; i++) {
	//for (int j = 0; j < 7; j++) {
	//	lasermap[i][j] = 0;
	//}
//}

//int x1 = 0; int y1 = 0; int r = 0;

//if (moveNumber % 2 == 0) {
	//r = board[0][0]->getRot();
//}
//if (moveNumber % 2 == 1) {
	//x1 = 9;
	//y1 = 7;
	//r = board[9][7]->getRot();
//}
//while (r < 4)
//{
	//switch (r) :
		//case 0:x1--; if (x1 > -1) { r = board[x1][y1]->laser(r); } break;
		//case 1:y1++; if (y1 < 10) { r = board[x1][y1]->laser(r); } break;
		//case 2:x1++; if (x1 < 10) { r = board[x1][y1]->laser(r); } break;
		//case 3:y1--; if (y1 > -1) { r = board[x1][y1]->laser(r); } break;
		//case 5:board[x1][y1]->setId(0); break;
		//case 6:break;
		//case 7:break;
		//создать oldr, потом создать двумерный массив 10*8, где в зависимости от значений old r и r будут храниться нужные рисунки лазера

//}

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
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

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главног


//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
HBITMAP hBitmaps[1 + 5 * 2][4];//виды и положения фигур
//static HBITMAP ChPieceBoard[10][8];//отрисовывает поле
ChessPiece *board[10][8];//какая фигура хранится в определенной клетке
bool fromSelected = false;
int fromx, fromy;//valid when fromSelected
int moveNumber = 1;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	int x, y;
	int tox, toy;

	switch (message)
    {
	case WM_CREATE:
		//DWORD err2;

				hBitmaps[0][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\test.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[0][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\test.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[0][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\test.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[0][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\test.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

				hBitmaps[1][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubRed0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[1][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubRed1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[1][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubRed2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[1][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubRed3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[2][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubWhite0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[2][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubWhite1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[2][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubWhite2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[2][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\AnubWhite3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[3][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharRed0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[3][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharRed1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[3][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharRed2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[3][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharRed3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[4][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharWhite0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[4][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharWhite1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[4][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharWhite2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[4][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PharWhite3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[5][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrRed0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[5][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrRed1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[5][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrRed2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[5][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrRed3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[6][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrWhite0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[6][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrWhite1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[6][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrWhite2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[6][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\PyrWhite3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				
				hBitmaps[7][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarRed0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[7][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarRed1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[7][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarRed2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[7][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarRed3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

				hBitmaps[8][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarWhite0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[8][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarWhite1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[8][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarWhite2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[8][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\ScarWhite3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

				hBitmaps[9][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxRed0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[9][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxRed1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[9][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxRed2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[9][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxRed3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

				hBitmaps[10][0] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxWhite0.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[10][1] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxWhite1.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[10][2] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxWhite2.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				hBitmaps[10][3] = (HBITMAP)LoadImageA(hInst, "c:\\res\\SfinxWhite3.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

				for (x = 0; x < 10; x++) {
					for (y = 0; y < 8; y++) {
						board[x][y] = new ChessPiece(0, 0);
					}
				}
				board[0][0] = new ChessPiece(9, 0);
				board[0][3] = new ChessPiece(5, 0);
				board[0][4] = new ChessPiece(5, 1);
				board[2][1] = new ChessPiece(5, 2);
				board[2][3] = new ChessPiece(6, 2);
				board[2][4] = new ChessPiece(6, 3);
				board[2][7] = new ChessPiece(6, 3);
				board[3][2] = new ChessPiece(6, 3);
				board[3][7] = new ChessPiece(2, 2);
				board[4][0] = new ChessPiece(1, 0);
				board[4][3] = new ChessPiece(7, 2);
				board[4][4] = new ChessPiece(8, 1);
				board[4][7] = new ChessPiece(4, 2);
				board[5][0] = new ChessPiece(3, 0);
				board[5][3] = new ChessPiece(7, 3);
				board[5][4] = new ChessPiece(8, 0);
				board[5][7] = new ChessPiece(2, 2);
				board[6][0] = new ChessPiece(1, 0);
				board[6][5] = new ChessPiece(5, 1);
				board[7][0] = new ChessPiece(5, 1);
				board[7][3] = new ChessPiece(5, 1);
				board[7][4] = new ChessPiece(5, 0);
				board[7][6] = new ChessPiece(6, 0);
				board[9][3] = new ChessPiece(6, 3);
				board[9][4] = new ChessPiece(6, 2);
				board[9][7] = new ChessPiece(10, 2);

				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 7; j++) {
						lasermap[i][j] = 0;
					}
				}
		//err2 = GetLastError();
		break;
		//#define WM_NCLBUTTONDOWN                0x00A1
		//#define WM_NCLBUTTONUP                  0x00A2
		//#define WM_NCLBUTTONDBLCLK              0x00A3
		//#define WM_NCRBUTTONDOWN                0x00A4
		//#define WM_NCRBUTTONUP                  0x00A5
		//#define WM_NCRBUTTONDBLCLK              0x00A6
		//#define WM_NCMBUTTONDOWN                0x00A7
		//#define WM_NCMBUTTONUP                  0x00A8
		//#define WM_NCMBUTTONDBLCLK              0x00A9

		//#define WM_LBUTTONDOWN                  0x0201
		//#define WM_LBUTTONUP                    0x0202
		//#define WM_LBUTTONDBLCLK                0x0203
		//#define WM_RBUTTONDOWN                  0x0204
		//#define WM_RBUTTONUP                    0x0205
		//#define WM_RBUTTONDBLCLK                0x0206
		//#define WM_MBUTTONDOWN                  0x0207
		//#define WM_MBUTTONUP                    0x0208
		//#define WM_MBUTTONDBLCLK                0x0209
	case WM_PAINT:
		PAINTSTRUCT     ps;
		HDC             hdc;
		BITMAP          bitmap;
		BITMAP			AnubRed;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;
		RECT			clientRect;
		RECT			textRect;
		HRGN			bgRgn;
		HBRUSH			hBrush;
		HPEN			hPen;

		int x, y;

		hdc = BeginPaint(hWnd, &ps);

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmaps[0][0]);

		GetObject(hBitmaps[0][0], sizeof(bitmap), &bitmap);
		for (x = 0; x < 10; x++) {
			for (y = 0; y < 8; y++) {
				BitBlt(hdc, bitmap.bmWidth*x, bitmap.bmHeight*y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
			}
		}
		

		hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 255, 255));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0)));
		for (x= 0; x < 10; x++)
		{
			for (y = 0; y < 8; y++)
			{
				oldBitmap = SelectObject(hdcMem, hBitmaps[board[x][y]->getId()][board[x][y]->getRot()]);
				BitBlt(hdc, x * 100, y * 100, 100, 100, hdcMem, 0, 0, SRCCOPY);
				SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
				Rectangle(hdc, x*100, y*100+50, x*100+100, y*100+55);
			}
		}

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		break;
	case  WM_LBUTTONDOWN:
		if (!fromSelected) {
			fromx = LOWORD(lParam) / 100;
			fromy = HIWORD(lParam) / 100;
			if ((fromx >= 0) && (fromx <= 9) && (fromy >= 0) && (fromy <= 7)) { fromSelected = true; break; }
		}
		if (fromSelected) {
			tox = LOWORD(lParam) / 100;
			toy = HIWORD(lParam) / 100;
			if (tox >= 0 && tox <= 9 && toy >= 0 && toy <= 7
				&& fromx == tox && fromy == toy && (moveNumber % 2 != board[fromx][fromy]->getId() % 2)
				&& board[fromx][fromy]->getId() > 0) {
				int a = board[fromx][fromy]->getRot();
				switch (fromx)
				{
				case 0:if (fromy == 0 && board[fromx][fromy]->getRot() == 2)
				{
					board[fromx][fromy]->setRot(1);
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
					moveNumber++;
					fromSelected = false;
				}; break;
				case 9:if (fromy == 7 && board[fromx][fromy]->getRot() == 0)
				{
					board[fromx][fromy]->setRot(3);
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
					moveNumber++;
					fromSelected = false;
				}; break;
				default:board[fromx][fromy]->setRot((a + 3) % 4);
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
					moveNumber++;
					fromSelected = false;
				}
				//функция лазера
				break;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		if (fromSelected) {
			tox = LOWORD(lParam) / 100;
			toy = HIWORD(lParam) / 100;
			if (tox >= 0 && tox <= 9 && toy >= 0 && toy <= 7
				&& (moveNumber%2!=board[fromx][fromy]->getId()%2)
				&& (fromx != 0 || fromy != 0)
				&& board[fromx][fromy]->getId()>0) {
				if (fromx == tox && fromy == toy) {
					int a = board[fromx][fromy]->getRot();
					board[fromx][fromy]->setRot((a + 1) % 4);
					//функция лазера
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
					moveNumber++;
					fromSelected = false;
					break;
				}
				else if (board[fromx][fromy]->isValidMove(fromx, fromy, tox, toy)) {
					ChessPiece *tmp = board[tox][toy];
					board[tox][toy] = board[fromx][fromy];
					board[fromx][fromy] = tmp;
					moveNumber++;
					//функция лазера
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
				}
			}
			fromSelected = false;
		}
		break;
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
