#pragma once

#include "WinOpt.h"
#include <d2d1.h>

class Graphics
{
public:
	Graphics();
	~Graphics();
	
	bool Init(HWND hwnd);

	void BeginDraw();
	void EndDraw();

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
private:
	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* RenderTarget;
};