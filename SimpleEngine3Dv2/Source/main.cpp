#include "System/Window.h"
#include "Graphic/API/Context.h"
#include "System/Graphics.h"
#include "System/File.h"

using namespace SE3D2;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	File::Get().Startup();
	Window::Get().Startup(800, 600, "SimpleEngine3Dv2");
	Graphics::Get().Startup(GraphicsAPI::DIRECTX11, 800, 600);

	while (Window::Get().ShouldWindowClose())
	{
		Window::Get().Update();
		Graphics::Get().Clear();

		Graphics::Get().Update();
	}

	Graphics::Get().Shutdown();
	Window::Get().Shutdown();
	File::Get().Shutdown();

	return 0;
}