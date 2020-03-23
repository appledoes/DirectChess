#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx)
	:
	wicFactory(nullptr),
	wicDecoder(nullptr),
	wicFrame(nullptr),
	wicConverter(nullptr)
{
	this->gfx = gfx;
	bmp = nullptr;
	HRESULT hr;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory
	);

	if (hr != S_OK) throw EXCEPT_LAST_ERROR();

	hr = wicFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder
	);

	if (hr != S_OK) throw EXCEPT_LAST_ERROR();

	hr = wicDecoder->GetFrame(0, &wicFrame);

	if (hr != S_OK) throw EXCEPT_LAST_ERROR();

	hr = wicFactory->CreateFormatConverter(&wicConverter);

	if (hr != S_OK) throw EXCEPT_LAST_ERROR();

	hr = wicConverter->Initialize(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0,
		WICBitmapPaletteTypeCustom
	);

	if (hr != S_OK) throw EXCEPT_LAST_ERROR();

	gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		wicConverter,
		nullptr,
		&bmp
	);

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
}

SpriteSheet::~SpriteSheet()
{
	if (bmp) bmp->Release();
}

void SpriteSheet::Draw(float left, float top)
{
	gfx->GetRenderTarget()->DrawBitmap(
		bmp,
		D2D1::RectF(
			left,
			top,
			bmp->GetSize().width,
			bmp->GetSize().height
		),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(
			left,
			top,
			bmp->GetSize().width,
			bmp->GetSize().height
		)
	);
}