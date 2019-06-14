#include "Window.h"
#include <iostream>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window(640, 480, "DirectX Engine");
	Window window2(640, 480, "DirectX Engin2e");

	//Messages queue
	MSG message;
	BOOL get_result;
	while ((get_result = GetMessage(&message, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	if (get_result == -1)
		return -1;
	else return message.wParam;
}
