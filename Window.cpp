#include "Window.h"
#include <sstream>
#include "resource.h"

Window::WindowClass Window::WindowClass::WndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = 0; /*CS_NOCLOSE*/
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 256, 256, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(WndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return WndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return WndClass.hInst;
}

Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw EXCEPT_LAST_ERROR();
	}

	// Create Window
	hwnd = CreateWindow(                          
		WindowClass::GetName(),                   // Name of the Window class
		name,                                     // Name of the Instance
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Style
		CW_USEDEFAULT,                            // X position
		CW_USEDEFAULT,                            // Y position
		wr.right - wr.left,                       // Width of window
		wr.bottom - wr.top,                       // Height of window
		nullptr,                                  // Parent Handle
		nullptr,                                  // Menu of window
		WindowClass::GetInstance(),               // Instance of window
		this                                      // Long pointer to parameter
	);
	if (hwnd == nullptr)
	{
		throw EXCEPT_LAST_ERROR();
	}

	// Show Window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hwnd, title.c_str()) == 0)
	{
		throw EXCEPT_LAST_ERROR();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

/* 
   CAUTION: STUFF BELOW MANAGES SETUP AND THUNK OF WINDOW
   MAKING A MISTAKE MAY BE FATAL TO THE PROGRAM
*/

// HandleMsgSetup is to only setup Window
LRESULT CALLBACK Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		// pCreate is pointer to lParam
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		// pWnd is pointer fowarding pCreate to lpCreateParams function
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Storing pointer to Window Class
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Setting Message Procedure to standard handler
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// Fowarding message to Default Handler
		return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
	}

	// Return Default Message Procedure if not WM_NCCREATE
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// pWnd is pointer to Window pointer type to function GetWindowLongPtr
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	// Fowarding message to Default Handler
	return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

// Handling Windows Messages
LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Standard Message switch
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
		//*** KEY MESSAGES ***//
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutoRepeatEnabled())
		{
			kbd.onKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.onChar(static_cast<unsigned char>(wParam));
		break;
		//*** END OF KEY MESSAGES ***//

		//*** MOUSE MESSAGES ***//
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// Check to see if in Client Region (A chess piece shouldn't go where you don't want it to go)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.onMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hwnd);
				mouse.onMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				mouse.onMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.onMouseLeave();
			}

		}
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onMiddlePressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.onMiddleReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.onWheelDelta(pt.x, pt.y, delta);
		break;
	}
		//*** END MOUSE MESSAGES ***//
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Handling Windows Exceptions
/*
	CAUTION: CHANGING THIS STUFF MAY BE CATASTROPHIC IF NOT DONE CORRECTLY
*/

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	ExceptionHandler(line, file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	// Create Output Stream
	std::ostringstream oss;
	// Define Output Stream
	oss << GetType() << std::endl
		<< "[Error Code]  "  << GetErrorCode()   << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	// Defining whatBuffer
	whatBuffer = oss.str();
	// Return whatBuffer
	return whatBuffer.c_str();
}

// Exception Type
const char* Window::Exception::GetType() const noexcept
{
	return "Cherry Handle Exception";
}

// Translating Error Code ( To be seen in text )
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	// Setting Pointer to Message Buffer to nullptr
	char* pMsgBuf = nullptr;

	// Actually Formatting the message
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |           // V
		FORMAT_MESSAGE_FROM_SYSTEM |               // V
		FORMAT_MESSAGE_IGNORE_INSERTS,             // This and above are DWORD Flags (i.e. the way we want it formatted)
		nullptr,                                   // The location of the Message Definition
		hr,                                        // The HResult
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Language it returns as
		reinterpret_cast<LPSTR>(&pMsgBuf),         // A pointer to the buffer
		0,                                         // Size of the Output Buffer (doesn't matter because of the flags set)
		nullptr                                    // Pointer to Arguments
	);
	if (nMsgLen == 0)
	{
		return "Unidentified Error Code (Please contact the developer)";
	}

	std::string ErrorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return ErrorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}