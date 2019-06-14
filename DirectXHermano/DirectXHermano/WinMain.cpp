#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Reghister Windows Class
	WNDCLASSEX windows_class = { 0 };
	windows_class.cbSize = sizeof(windows_class);
	windows_class.style = CS_OWNDC;
	windows_class.lpfnWndProc = DefWindowProc;
	windows_class.cbClsExtra = 0;
	windows_class.cbWndExtra = 0;
	windows_class.hInstance = hInstance;
	windows_class.hCursor = nullptr;
	windows_class.hIcon = nullptr;
	windows_class.hbrBackground = nullptr;
	windows_class.lpszMenuName = nullptr;
	windows_class.lpszClassName = "DirectXEngine";
	windows_class.hIconSm = nullptr;

	RegisterClassEx(&windows_class);

	//Make an instance of this class
	HWND window_handler = CreateWindowEx(0, 
		windows_class.lpszClassName, "DirectXEngine",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(window_handler, SW_SHOW);

	while (true);

	return 0;
}
