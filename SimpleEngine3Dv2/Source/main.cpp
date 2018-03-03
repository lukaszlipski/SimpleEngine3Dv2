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

using namespace SE3D2;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	File::Get().Startup();
	Window::Get().Startup(800, 600, "SimpleEngine3Dv2");
	Graphics::Get().Startup(GraphicsAPI::OPENGL, 800, 600);

	Shader* vertex = ShaderManager::Get().GetShader<ShaderType::VERTEX>("VertexShaderTest");
	Shader* fragment = ShaderManager::Get().GetShader<ShaderType::FRAGMET>("PixelShaderTest");

	//vertex->Bind();
	//fragment->Bind();

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

	Buffer* vbuffer = Graphics::Get().GetContext()->CreateVertexBuffer(sizeof(CommonVertex) * 4, &vertexArray);

	// Create indices
	uint32_t indicesArray[] = {
		0,1,2,
		0,2,3
	};

	Buffer* ibuffer = Graphics::Get().GetContext()->CreateIndexBuffer(sizeof(uint32_t) * 6, &indicesArray);

	///////////////////////////////// RAW OPENGL  /////////////////////////////////////////

	uint32 pipe;
	glGenProgramPipelines(1, &pipe);
	glUseProgramStages(pipe, GL_VERTEX_SHADER_BIT, static_cast<OGVertexShader*>(vertex)->GetProgram());
	glUseProgramStages(pipe, GL_FRAGMENT_SHADER_BIT, static_cast<OGFragmentShader*>(fragment)->GetProgram());

	glBindProgramPipeline(pipe);

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((OGIndexBuffer*)ibuffer)->GetBuffer());

	glBindVertexBuffer(0, ((OGVertexBuffer*)vbuffer)->GetBuffer(), 0, CommonVertex::VertexFormatInfo.Size);

	glBindVertexArray(0);


	//////////////////////////////////////////////////////////////////////////

	///////////////////////////////// RAW DIRECTX /////////////////////////////////////////

	// Set Vertex buffer
	//ID3D11Buffer *vertexBuffer = static_cast<Dx11VertexBuffer*>(vbuffer)->GetBuffer();
	//UINT stride = sizeof(CommonVertex);
	//UINT offset = 0;
	//static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//
	//// Set Indices buffer
	//ID3D11Buffer *indicesBuffer = static_cast<Dx11IndexBuffer*>(ibuffer)->GetBuffer();
	//static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	//
	//static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//////////////////////////////////////////////////////////////////////////

	while (Window::Get().ShouldWindowClose())
	{
		Window::Get().Update();
		Graphics::Get().Clear();
		
		///////////////////////////////// RAW DIRECTX /////////////////////////////////////////
		//static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->DrawIndexed(6, 0, 0);
		//////////////////////////////////////////////////////////////////////////

		///////////////////////////////// RAW OPENGL  /////////////////////////////////////////
		glBindVertexArray(VAO);
		
		vf->Bind();
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//////////////////////////////////////////////////////////////////////////

		Graphics::Get().Update();
	}

	Graphics::Get().Shutdown();
	Window::Get().Shutdown();
	File::Get().Shutdown();

	return 0;
}