#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string>

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

		bool Startup(uint32_t width, uint32_t height, const std::string& title);
		bool Shutdown();
		void Close();
		void Update();

		inline uint32_t GetWidth() const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }
		inline bool ShouldWindowClose() const { return mRunning; }

	private:
		uint32_t mWidth;
		uint32_t mHeight;
		HWND mWindowHandle;
		std::string mTitle;
		static bool mRunning;

		Window() {};

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}