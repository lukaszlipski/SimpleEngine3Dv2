#include "Window.h"

namespace SE3D2
{
	bool Window::mRunning = false;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool Window::Startup(uint32_t width, uint32_t height, const std::string& title)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;

		// Create class name
		std::string ClassName = mTitle + "ClassName";

		WNDCLASSEX WindowClass = {};
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.lpszClassName = ClassName.c_str();
		WindowClass.hInstance = GetModuleHandle(0);
		WindowClass.lpfnWndProc = &WindowProc;

		if (!RegisterClassExA(&WindowClass)) //  Attempt to register class
		{
			return false; 
		}

		// Attempt to create window
		mWindowHandle = CreateWindowExA(0, WindowClass.lpszClassName, mTitle.c_str(), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight, 0, 0, GetModuleHandle(0), 0);
		if (!mWindowHandle)
		{
			return false;
		}

		ShowWindow(mWindowHandle, SW_SHOWNORMAL);
		UpdateWindow(mWindowHandle);
		
		mRunning = true;
		return true;
	}

	bool Window::Shutdown()
	{
		PostQuitMessage(0);
		DestroyWindow(mWindowHandle);

		return true;
	}

	void Window::Close()
	{
		mRunning = false;
	}

	void Window::Update()
	{
		MSG Message;
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				Close();
			}

			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;

		switch (uMsg)
		{

		case WM_CLOSE:
		{
			Window::mRunning = false;
			break;
		}
		case WM_DESTROY:
		{
			break;
		}
		default:
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		}

		return result;
	}
}
