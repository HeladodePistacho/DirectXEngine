#define MESSAGE_OK 1
#include "Fast.h"
#include <iostream>

#include "KeyBoard.h"
#include "Mouse.h"
#include "FileDrop.h"
#include "Render.h"

class Window
{
private:
	//Singleton to maange registration and clean of window
	class WindowClass
	{
	public:
		static const char* GetName() { return window_class_name; }
		static HINSTANCE GetInstance() { return singleton_window_class.handle_instance; }

	private:
		WindowClass();
		WindowClass(const WindowClass&) = delete; //"=delete" stands for the compiler to not create default copy constructor
		~WindowClass();

		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* window_class_name = "DirectX Engine Window"; //"constexpr" stands for the variable to be evaluated at compile time
		static WindowClass singleton_window_class;
		HINSTANCE handle_instance;
	};

public:
	Window(int width, int height, const char* name);
	Window(const Window&) = delete;
	~Window();
	Window& operator=(const Window&) = delete;

	KeyBoard keyboard;
	Mouse	 mouse;
	DropManager dropper;

	Render&  GetRender() const { return *window_render; }
	HWND GetHandler() const { return window_handle; }

	void SetWindowTitle(const char* name);
	static int ProcessMessages();

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND handle, UINT message, WPARAM w_param, LPARAM l_param); //Connect our Window Instance with the Message Handling system
	static LRESULT CALLBACK HandleMsgThunk(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
	LRESULT HanldeMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

	std::string TranslateError(HRESULT);
private:
	int width, height;
	HWND window_handle;
	Render* window_render = nullptr;
};
