#include "App.h"
#include "ErrorHandling.h"
#include <iostream>


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DirectXApp* app = new DirectXApp();

	if (app->Start())
	{
		delete app;
	}

	return 1;
	
}


