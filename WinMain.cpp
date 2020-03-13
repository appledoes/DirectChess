#include "Application.h"

int WINAPI wWinMain(
	_In_     HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     PWSTR          pCmdLine,
	_In_     int            nCmdShow
)
{
	try
	{
		return Application{}.Run();
	}
	catch (const ExceptionHandler & e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception & e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}