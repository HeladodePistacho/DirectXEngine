#include "Window.h"
#include "ErrorHandling.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"

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

Window::Window(int _width, int _height, const char* name)
{
	window_handle = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, _width, _height,
		nullptr, nullptr, WindowClass::GetInstance(), this);

	if (window_handle == nullptr)
	{
		//custom_exception error("Window Error", TranslateError(GetLastError()).c_str());
		//throw error;
	}

	RECT rect_size;
	GetClientRect(window_handle, &rect_size);

	width = rect_size.right;
	height = rect_size.bottom;

	//char buffer_1[256];
	//sprintf_s(buffer_1, "Width: %i, Height: %i", width, height);
	//
	//custom_exception error("Window Error", buffer_1);
	//throw error;

	ShowWindow(window_handle, SW_SHOWDEFAULT);

	//Im GUI handler
	ImGui_ImplWin32_Init(window_handle);

	//Create graphics object
	window_render = new Render(window_handle, width, height);
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(window_handle);

	if (window_render)
		delete window_render;
}

void Window::SetWindowTitle(const char * name)
{
	if (SetWindowText(window_handle, name) == 0)
	{  
		//custom_exception error("Window Error", TranslateError(GetLastError()).c_str());
		//throw error;
	}
}

int Window::ProcessMessages()
{
	MSG message;

	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE ))
	{
		if (message.message == WM_QUIT)
			return message.wParam;

		TranslateMessage(&message);
		DispatchMessage(&message);		
	}

	return MESSAGE_OK;
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
	if (ImGui_ImplWin32_WndProcHandler(handle, message, w_param, l_param))
		return true;

	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;

		//Clear Key states when we change the focused window to avoid zombie key events
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;

		//Keyboard Input Messages
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//Bit 30 -> 0x40000000 is equals to 1 if the key was before, so we want only to get keydown when it is 0
		if(!(l_param & 0x40000000) || keyboard.IsRepeatEnabled())
			keyboard.OnKeyPressed(static_cast<unsigned char>(w_param));
		break;
	
	case WM_KEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(w_param));
		break;
	
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(w_param));
		break;
	
		//Mouse Input Messages
	case WM_MOUSEMOVE:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);

		//Look if mouse is inside window region
		if (mouse_pos.x >= 0 && mouse_pos.x < width && mouse_pos.y >= 0 && mouse_pos.y < height)
		{
			mouse.OnMouseMove(mouse_pos.x, mouse_pos.y);

			if (!mouse.IsInWindow())
			{
				SetCapture(window_handle);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			//If it is outside keep moving if some button is down
			if (mouse.LeftIsPressed() || mouse.RightIsPressed())
			{
				mouse.OnMouseMove(mouse_pos.x, mouse_pos.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}

		
		break;
	}
	case WM_LBUTTONDOWN:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);
		mouse.OnLeftPressed(mouse_pos.x, mouse_pos.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);
		mouse.OnLeftReleased(mouse_pos.x, mouse_pos.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);
		mouse.OnRightPressed(mouse_pos.x, mouse_pos.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);
		mouse.OnRightReleased(mouse_pos.x, mouse_pos.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		POINTS mouse_pos = MAKEPOINTS(l_param);
		const int delta = GET_WHEEL_DELTA_WPARAM(w_param);
		mouse.OnWeheelDelta(mouse_pos.x, mouse_pos.y, delta);
		
		break;
	}
	}
	return DefWindowProc(handle, message, w_param, l_param);
}

std::string Window::TranslateError(HRESULT error)
{
	char* error_char = nullptr;
	DWORD message_lenght = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&error_char), 0, nullptr);

	if (message_lenght == 0)
		return "UNKNOWN ERROR";

	std::string tmp = error_char;
	LocalFree(error_char);
	return tmp;
}