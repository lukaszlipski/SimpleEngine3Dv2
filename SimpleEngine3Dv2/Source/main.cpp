#include "System/Window.h"
#include "Graphic/API/Context.h"
#include "System/Graphics.h"
#include "System/File.h"
#include "Graphic/ShaderManager.h"
#include "Graphic/VertexFormatManager.h"

// Debug
#include <d3d11.h>
#include "Platform/Directx11/Dx11Context.h"
#include "Platform/DirectX11/Dx11Buffer.h"
#include "GL/glew.h"
#include "Platform/OpenGL/OGShader.h"
#include "Platform/OpenGL/OGBuffer.h"
#include "Graphic/API/ShaderPipeline.h"
#include "Platform/OpenGL/OGShaderPipeline.h"
#include "Platform/DirectX11/Dx11ShaderPipeline.h"
#include "Utility/Image.h"

using namespace SE3D2;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	File::Get().Startup();
	Window::Get().Startup(1024, 720, "SimpleEngine3Dv2");
	Graphics::Get().Startup(GraphicsAPI::DIRECTX11, 1024, 720);

	//Image Img("Image/test.tga");
	//Img.TryLoad();

	Shader* vertex = ShaderManager::Get().GetShader<ShaderType::VERTEX>("VertexShaderTest");
	Shader* fragment = ShaderManager::Get().GetShader<ShaderType::PIXEL>("PixelShaderTest");

	ShaderPipeline* shaderPip = Graphics::Get().GetContext()->CreateShaderPipeline({ vertex, fragment });
	shaderPip->Bind();

	ParametersBuffer* pb = vertex->FindParametersBuffer("PerObject");
	pb->SetFloat("Float", 0.2f);
	vertex->SetParametersBuffer(pb);

	ParametersBuffer* pb2 = fragment->FindParametersBuffer("PerObject2");
	pb2->SetFloat("Float", 1.0f);
	fragment->SetParametersBuffer(pb2,1);

	VertexFormat* vf = VertexFormatManager::Get().GetVertexFormat<CommonVertex>(vertex);
	vf->Bind();

	// Vertex buffer
	CommonVertex vertexArray[] = {
		{ 0.5f,0.5f,0.5f , 1,0},
		{ 0.5f,-0.5f,0.5f , 0,1},
		{ -0.5f,-0.5f,0.5f , 0,0},
		{ -0.5f,0.5f,0.5f , 1,0}
	};

	VertexBuffer* vbuffer = Graphics::Get().GetContext()->CreateVertexBuffer(sizeof(CommonVertex) * 4, &vertexArray);
	vbuffer->Bind(CommonVertex::VertexFormatInfo.Size);

	// Create indices
	uint32_t indicesArray[] = {
		0,1,2,
		0,2,3
	};

	IndexBuffer* ibuffer = Graphics::Get().GetContext()->CreateIndexBuffer(sizeof(uint32_t) * 6, &indicesArray);
	ibuffer->Bind();

	Shader* compute = ShaderManager::Get().GetShader<ShaderType::COMPUTE>("ComputeShaderTest");
	StructuredBuffer* sb = Graphics::Get().GetContext()->CreateStructuredBuffer(4, sizeof(uint32_t) * 6, &indicesArray);
	///////////////////////////////// RAW OPENGL  /////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////// RAW DIRECTX /////////////////////////////////////////
	static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//////////////////////////////////////////////////////////////////////////

	while (Window::Get().ShouldWindowClose())
	{
		Window::Get().Update();
		Graphics::Get().Clear();
		
		Graphics::Get().GetContext()->SetComputeShader(compute);
		compute->SetStructuredBuffer("test", sb);
		Graphics::Get().GetContext()->Dispatch(6, 1, 1);
		compute->SetStructuredBuffer("test");
		shaderPip->Bind();
		fragment->SetStructuredBuffer("test", sb);

		///////////////////////////////// RAW DIRECTX /////////////////////////////////////////
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->DrawIndexed(ibuffer->GetIndicesNum(), 0, 0);
		//////////////////////////////////////////////////////////////////////////

		///////////////////////////////// RAW OPENGL  /////////////////////////////////////////	
		//glDrawElements(GL_TRIANGLES, ibuffer->GetIndicesNum(), GL_UNSIGNED_INT, 0);
		//////////////////////////////////////////////////////////////////////////

		fragment->SetStructuredBuffer("test");

		Graphics::Get().Update();
	}

	Graphics::Get().Shutdown();
	Window::Get().Shutdown();
	File::Get().Shutdown();

	return 0;
}