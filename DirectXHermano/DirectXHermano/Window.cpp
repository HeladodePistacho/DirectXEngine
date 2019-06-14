#include "Window.h"

//----------------------WINDOW CLASS-----------------------
Window::WindowClass Window::WindowClass::singleton_window_class;

Window::WindowClass::WindowClass() : handle_instance(GetModuleHandle(nullptr))
{
	//Reghister Windows Class
	WNDCLASSEX windows_class = { 0 };
	windows_class.cbSize = sizeof(windows_class);
	windows_class.style = CS_OWNDC;
	windows_class.lpfnWndProc = HandleMsgSetup;
	windows_class.cbClsExtra = 0;
	windows_class.cbWndExtra = 0;
	windows_class.hInstance = GetInstance();
	windows_class.hCursor = nullptr;
	windows_class.hIcon = nullptr;
	windows_class.hbrBackground = nullptr;
	windows_class.lpszMenuName = nullptr;
	windows_class.lpszClassName = GetName();
	windows_class.hIconSm = nullptr;

	RegisterClassEx(&windows_class);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(window_class_name, GetInstance());
}

//---------------------- WINDOW -----------------------

Window::Window(int width, int height, const char* name)
{
	RECT window_rect;
	window_rect.left = 100;
	window_rect.top = 100;
	window_rect.right = window_rect.left + width;
	window_rect.bottom = window_rect.top + height;

	AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	window_handle = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(window_handle, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(window_handle);
}

LRESULT Window::HandleMsgSetup(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (message == WM_NCCREATE)
	{
		//Get Pointer to window class from creation data
		const CREATESTRUCTW* const p_create = reinterpret_cast<CREATESTRUCTW*>(l_param);
		Window* const p_window = static_cast<Window*>(p_create->lpCreateParams);

		//Set WINAPI-managed user data to store pointer to window class -> Creates a window between the window itself and our window class
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_window));

		//Set message proc to normal handler now that the set up is finished
		SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		//Send the message to the window class handler
		return p_window->HanldeMessage(handle, message, w_param, l_param);
	}
	return DefWindowProc(handle, message, w_param, l_param);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	//Recupera el puntero a la clase window
	Window* const p_window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));

	//Send message to window handler
	return p_window->HanldeMessage(handle, message, w_param, l_param);

}

LRESULT Window::HanldeMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

	//	//Keyboard Input Messages
	//case WM_KEYDOWN:
	//
	//	break;
	//
	//case WM_KEYUP:
	//	break;
	//
	//case WM_CHAR:
	//
	//	break;
	//
	//	//Mouse Input Messages
	//case WM_LBUTTONDOWN:
	//	POINTS mouse_pos = MAKEPOINTS(l_param);
	//
	//
	//	break;
	}
	return DefWindowProc(handle, message, w_param, l_param);
}
