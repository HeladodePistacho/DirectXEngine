#include <Windows.h>
#include <iostream>

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

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND handle, UINT message, WPARAM w_param, LPARAM l_param); //Connect our Window Instance with the Message Handling system
	static LRESULT CALLBACK HandleMsgThunk(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
	LRESULT HanldeMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

	std::string TranslateError(HRESULT);
private:
	int width, height;
	HWND window_handle;
};
