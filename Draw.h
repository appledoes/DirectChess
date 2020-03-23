#pragma once

#include "Graphics.h"

class Draw : Graphics
{
public:
	Draw() = default;
	Draw(const Draw&) = delete;
	Draw& operator = (const Draw&) = delete;

	bool InitWnd(HWND hwnd);

	void Paint();
};