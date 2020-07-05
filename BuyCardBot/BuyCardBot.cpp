#include <iostream>
#include <Windows.h>
#include "Helper.h"

inline VOID CheckMouseMoved(POINT &pt1, POINT &pt2)
{
	if (pt1.x != pt2.x || pt1.y != pt2.y)
		exit(EXIT_SUCCESS);
}

VOID MouseClick(INPUT &keyEvent)
{
	keyEvent.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &keyEvent, sizeof(INPUT));
	Sleep(100);
	keyEvent.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, &keyEvent, sizeof(INPUT));
	Sleep(100);
}

VOID MouseMove(HWND hWnd, INPUT& keyEvent, FLOAT factorX, FLOAT factorY)
{
	POINT pt1, pt2;
	RECT rect;
	GetWindowRect(hWnd, &rect);
	LONG winX = rect.left, winY = rect.top;
	LONG winWidth = rect.right - rect.left;
	LONG winHeight = rect.bottom - rect.top;
	LONG x = winX + winWidth * factorX;
	LONG y = winY + winHeight * factorY;
	keyEvent.mi.dx = x * (65536 / GetSystemMetrics(SM_CXSCREEN));
	keyEvent.mi.dy = y * (65536 / GetSystemMetrics(SM_CYSCREEN));
	keyEvent.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	SendInput(1, &keyEvent, sizeof(INPUT));
	GetCursorPos(&pt1);
	Sleep(200);
	GetCursorPos(&pt2);
	CheckMouseMoved(pt1, pt2);
}

VOID buyCard(const HWND hWnd,const INT idx)
{
	FLOAT factorX;
	switch (idx) {
	case 1:
		factorX = 0.25f;
		break;
	case 2:
		factorX = 0.40f;
		break;
	case 3:
		factorX = 0.50f;
		break;
	case 4:
		factorX = 0.65f;
		break;
	case 5:
		factorX = 0.75f;
		break;
	case 6:
		factorX = 0.90f;
		break;
	default:
		return;
	}

	INPUT keyEvent = { 0 };
	keyEvent.type = INPUT_MOUSE;
	keyEvent.mi.mouseData = 0;

	MouseMove(hWnd, keyEvent, factorX, 0.5f);
	MouseClick(keyEvent);
	MouseMove(hWnd, keyEvent, 0.35f, 0.67f);
	MouseClick(keyEvent);
	MouseMove(hWnd, keyEvent, 0.5f, 0.55f);
	MouseClick(keyEvent);
	POINT pt1, pt2;
	GetCursorPos(&pt1);
	Sleep(2200);
	GetCursorPos(&pt2);
	CheckMouseMoved(pt1, pt2);
	MouseMove(hWnd, keyEvent, 0.55f, 0.97f);
	MouseClick(keyEvent);
}

INT main(INT argc, PCHAR argv[])
{
	INT Idx = 0;
	std::cout << "Input index (1-6) to config the bot to buy which card sets (1:leftmost ~ 6:rightmost)" << std::endl;
	std::cout << "Your input : ";
	std::cin >> Idx;
	
	if (Idx < 0 || Idx > 6) {
		std::cerr << "Index range must be within 1 ~ 6" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	HWND hWnd = FindWindowA(NULL, "Yu-Gi-Oh! Legacy of the Duelist : Link Evolution");
	if (hWnd == 0) {
		ErrorHandler("FindWindowA");
		exit(EXIT_FAILURE);
	}
	SetForegroundWindow(hWnd);

	for (;;) {
		if (Idx == 0)
			break;
		buyCard(hWnd, Idx);
		if (FindWindowA(NULL, "Yu-Gi-Oh! Legacy of the Duelist : Link Evolution") == 0)
			break;
	}

	return EXIT_SUCCESS;
}
