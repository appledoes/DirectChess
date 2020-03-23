#include "Draw.h"

bool Draw::InitWnd(HWND hwnd)
{
	return Init(hwnd);
}

void Draw::Paint()
{
	BeginDraw();

	ClearScreen(1.0f, 1.0f, 1.0f);
	DrawCircle(100, 100, 50, 0.0f, 0.0f, 0.0f, 1.0f);

	EndDraw();
}