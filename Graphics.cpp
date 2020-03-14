#include "Graphics.h"

Graphics::Graphics()
	:
	Factory(nullptr),
	RenderTarget(nullptr)
{}

Graphics::~Graphics()
{
	if (Factory) (Factory)->Release();
	if (RenderTarget) (RenderTarget)->Release();
}

bool Graphics::Init(HWND hwnd)
{	
	HRESULT hres = D2D1CreateFactory(
		D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, 
		&Factory
	);

	if (hres != S_OK) return false;

	RECT rc;
	GetClientRect(hwnd, &rc);

	hres = Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(rc.right, rc.bottom)
		),
		&RenderTarget
	);

	if (hres != S_OK) return false;

	return true;
}

void Graphics::BeginDraw()
{
	RenderTarget->BeginDraw();
}

void Graphics::EndDraw()
{
	RenderTarget->EndDraw();
}