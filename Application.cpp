#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application()
	:
	wnd(900, 500, "Chess")
{}

int Application::Run()
{
	MSG msg;
	BOOL bret;

	while (bret = GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	if (bret == -1)
	{
		throw EXCEPT_LAST_ERROR();
	}

	return msg.wParam;
}

void Application::DoFrame()
{
	
}