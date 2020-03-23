#pragma once
#include <wincodec.h>
#include "Window.h"
#include "Graphics.h"

class SpriteSheet
{
	Graphics* gfx;
	ID2D1Bitmap* bmp;

	IWICImagingFactory* wicFactory;
	IWICBitmapDecoder* wicDecoder;
	IWICBitmapFrameDecode* wicFrame;
	IWICFormatConverter *wicConverter;
public:
	SpriteSheet(wchar_t* filename, Graphics* gfx);
	SpriteSheet(const SpriteSheet&) = delete;
	SpriteSheet& operator = (const SpriteSheet&) = delete;
	~SpriteSheet();

	void Draw(float left, float top);
};