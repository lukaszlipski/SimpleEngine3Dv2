#include "OGContext.h"
#include <GL/glew.h>
#include "../System/Window.h"
#include "OGShader.h"
#include "OGBuffer.h"
#include "OGParamtersBuffer.h"
#include "OGVertexFormat.h"
#include "OGShaderPipeline.h"

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
		Shader* NewShader = nullptr;
		switch (type)
		{
		case ShaderType::VERTEX:
		{
			NewShader = new OGShader<OGVertexShaderPolicy>(name);
			break;
		}
		case ShaderType::PIXEL:
		{
			NewShader = new OGShader<OGFragmentShaderPolicy>(name);
			break;
		}
		}

		if (NewShader->Compile(name))
		{
			return NewShader;
		}
		delete NewShader;

		return nullptr;
	}

	ParametersBuffer* OGContext::CreateParametersBuffer(const std::string& name, int32 size, int32 slot)
	{
		OGUniformBlockBuffer* ConstantBuffer = new OGUniformBlockBuffer();
		if (ConstantBuffer->Create(size) != true)
		{
			return nullptr;
		}

		return new OGParametersBuffer(name, size, slot, ConstantBuffer);
	}

	VertexFormat* OGContext::CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc)
	{
		OGVertexFormat* vertexFormat = new OGVertexFormat(vertexFormatDesc);
		vertexFormat->Create(vertexShader);

		return vertexFormat;
	}

	VertexBuffer* OGContext::CreateVertexBuffer(int32 size, void* data /*= nullptr*/)
	{
		OGVertexBuffer* vb = new OGVertexBuffer();
		vb->Create(size, data);
		return vb;
	}

	IndexBuffer* OGContext::CreateIndexBuffer(int32 size, void* data /*= nullptr*/)
	{
		OGIndexBuffer* ib = new OGIndexBuffer();
		ib->Create(size, data);
		return ib;
	}

	ShaderPipeline* OGContext::CreateShaderPipeline(std::initializer_list<Shader *> stages)
	{
		return new OGShaderPipeline(stages);
	}

	StructuredBuffer* OGContext::CreateStructuredBuffer(int32 stride, int32 size, void* data /*= nullptr*/)
	{
		OGStructuredBuffer* ib = new OGStructuredBuffer();
		ib->Create(size, data);
		return ib;
	}

}