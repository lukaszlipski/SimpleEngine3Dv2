#include "System/Window.h"
#include "Graphic/API/Context.h"
#include "System/Graphics.h"

using namespace SE3D2;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Window::GetInstance().Startup(800, 600, "SimpleEngine3Dv2");
	Graphics::GetInstance().Startup(GraphicsAPI::DIRECTX11, 800, 600);

	while (Window::GetInstance().ShouldWindowClose())
	{
		Window::GetInstance().Update();
		Graphics::GetInstance().Clear();

		Graphics::GetInstance().Update();
	}

	Graphics::GetInstance().Shutdown();
	Window::GetInstance().Shutdown();

	return 0;
}