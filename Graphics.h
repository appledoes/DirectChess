#pragma once

#include "WinOpt.h"
#include <d2d1.h>

class Graphics
{
protected:
	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* RenderTarget;
public:
	Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	~Graphics();
	
	bool Init(HWND hwnd);

	ID2D1RenderTarget* GetRenderTarget();

	void BeginDraw() noexcept;
	void EndDraw() noexcept;

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
};