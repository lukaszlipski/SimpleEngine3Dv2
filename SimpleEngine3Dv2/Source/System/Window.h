#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>
#include "..\Utility\Types.h"

namespace SE3D2
{

	class Window
	{

	public:
		static Window& GetInstance()
		{
			static Window *instance = new Window();
			return *instance;
		}

		bool Startup(uint32 width, uint32 height, const std::string& title);
		bool Shutdown();
		void Close();
		void Update();

		inline uint32 GetWidth() const { return mWidth; }
		inline uint32 GetHeight() const { return mHeight; }
		inline bool ShouldWindowClose() const { return mRunning; }
		inline HWND GetWindowHandle() const { return mWindowHandle; }

	private:
		uint32 mWidth;
		uint32 mHeight;
		HWND mWindowHandle;
		std::string mTitle;
		static bool mRunning;

		Window() {};

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}