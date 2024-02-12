#include <windows.h>
#include <tchar.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hDialog, hStatic2, clickedStatic;
HINSTANCE hInst1;
const int LEFT = 230, TOP = 450, WIDTH = 380, HEIGHT = 50;
int count = 0;


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
    hInst1 = hInst;
    MSG msg;
    hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
    ShowWindow(hDialog, nCmdShow);
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
    static int strx, stry;
    static HWND mystatic;
    static TCHAR str1[20];
    static RECT rect;
    int x, y, width, height; 

    switch (mes)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        PostQuitMessage(0);
        return TRUE;

    case WM_MOUSEMOVE:
        wsprintf(str1, TEXT("X=%d  Y=%d"), LOWORD(lp), HIWORD(lp));
        SetWindowText(hWnd, str1);
        break;

    case WM_LBUTTONDOWN:
        strx = LOWORD(lp);
        stry = HIWORD(lp);
        break;

    case WM_LBUTTONUP:
    {
        count++;
        x = LOWORD(lp);
        y = HIWORD(lp);

        width = x - strx;
        height = y - stry;

        if (width <= 10 && height <= 10) {
            hStatic2 = CreateWindowEx(0, TEXT("STATIC"), 0,
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
                LEFT, TOP, WIDTH, HEIGHT, hWnd, 0, hInst1, 0);

            SetWindowText(hStatic2, TEXT("Размер статика должен быть больше чем 10x10!"));
        }
        else {
            DestroyWindow(hStatic2);
            hStatic2 = NULL;

            mystatic = CreateWindowEx(0, TEXT("STATIC"), NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
                strx, stry, width, height, hWnd, (HMENU)count, hInst1, 0);
            TCHAR countStr[20];
            wsprintf(countStr, TEXT("%d"), count);
            SetWindowText(mystatic, countStr);
        }
    }
    break;

    case WM_CONTEXTMENU:
    {
        POINT coordinates;
        coordinates.x = LOWORD(lp);
        coordinates.y = HIWORD(lp);

        ScreenToClient(hWnd, &coordinates);

        HWND element = ChildWindowFromPoint(hWnd, coordinates);
        if (element != NULL && element != hWnd) {
            TCHAR txtresult[100];
            GetWindowRect(element, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            wsprintf(txtresult, TEXT("Static №%d, Ширина: %d, Высота: %d, X: %d, Y: %d"),GetDlgCtrlID(element), width, height, rect.left, rect.top);
            SetWindowText(hWnd, txtresult); 
        }
    }
    break;  
    case WM_RBUTTONDBLCLK:
    {
        POINT clickedPoint;
        clickedPoint.x = LOWORD(lp);
        clickedPoint.y = HIWORD(lp);

        clickedStatic = ChildWindowFromPoint(hWnd, clickedPoint);
        if (clickedStatic != hWnd)
        {
            DestroyWindow(clickedStatic);
        } 
    }
    break;

    }
    return FALSE;
}