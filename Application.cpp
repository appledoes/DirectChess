#include "Application.h"
#include <sstream>
#include <iomanip>

Application::Application()
	:
	wnd(900, 500, "Chess")
{}

int Application::Run()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void Application::DoFrame()
{
	
}