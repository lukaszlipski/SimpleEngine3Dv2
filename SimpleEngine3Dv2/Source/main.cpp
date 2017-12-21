#include "System/Window.h"

using namespace SE3D2;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Window::GetInstance().Startup(800, 600, "SimpleEngine3Dv2");

	while (Window::GetInstance().ShouldWindowClose())
	{
		Window::GetInstance().Update();

	}

	Window::GetInstance().Shutdown();
	return 0;
}