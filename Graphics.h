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
private:
	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* RenderTarget;
};