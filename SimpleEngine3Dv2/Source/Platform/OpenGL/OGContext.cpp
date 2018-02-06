#include "OGContext.h"
#include <GL/glew.h>
#include "../System/Window.h"

namespace SE3D2
{

	bool OGContext::Init(uint32 width, uint32 height)
	{
		PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
		DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
		DesiredPixelFormat.nVersion = 1;
		DesiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
		DesiredPixelFormat.cColorBits = 32;
		DesiredPixelFormat.cStencilBits = 8;
		DesiredPixelFormat.cDepthBits = 24;
		DesiredPixelFormat.cAlphaBits = 8;
		DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

		HDC WindowDC = GetDC(Window::Get().GetWindowHandle());

		int SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
		PIXELFORMATDESCRIPTOR SuggestedPixelFormat = {};
		DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
		SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

		mOpenGLContext = wglCreateContext(WindowDC);
		if (!wglMakeCurrent(WindowDC, static_cast<HGLRC>(mOpenGLContext)))
		{
			return false;
		}
	
		if (glewInit() != GLEW_OK)
		{
			ClearResources();
			return false;
		}

		return true;
	}

	void OGContext::ClearResources()
	{
		HDC WindowDC = GetDC(Window::Get().GetWindowHandle());
		wglMakeCurrent(WindowDC, 0);
		wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
		ReleaseDC(Window::Get().GetWindowHandle(), WindowDC);
	}

	void OGContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OGContext::SwapBuffers()
	{
		HDC WindowDC = GetDC(Window::Get().GetWindowHandle());
		::SwapBuffers(WindowDC);
		ReleaseDC(Window::Get().GetWindowHandle(), WindowDC);
	}

	Shader* OGContext::CreateShader(const std::string& name, ShaderType type)
	{
		return nullptr;
	}

	ParametersBuffer* OGContext::CreateParametersBuffer(const std::string& name, int32 size, int32 slot)
	{
		return nullptr;
	}

	VertexFormat* OGContext::CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc)
	{
		return nullptr;
	}

}