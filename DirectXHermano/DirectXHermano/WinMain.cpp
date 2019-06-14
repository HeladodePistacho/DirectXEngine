#include "Window.h"
#include "ErrorHandling.h"
#include <iostream>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Window window(640, 480, "DirectX Engine");

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
	catch (const custom_exception& error)
	{
		MessageBox(nullptr, error.GetContent(), error.GetTitle(), MB_OK | MB_ICONEXCLAMATION);
	}
}
