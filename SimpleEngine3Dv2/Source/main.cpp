#include "System/Window.h"
#include "Graphic/API/Context.h"
#include "System/Graphics.h"
#include "System/File.h"
#include "Graphic/ShaderManager.h"
#include "Graphic/VertexFormatManager.h"

// Debug
#include <d3d11.h>
#include "Platform/Directx11/Dx11Context.h"
#include "Platform/DirectX11/Dx11VertexFormat.h"

using namespace SE3D2;


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	File::Get().Startup();
	Window::Get().Startup(800, 600, "SimpleEngine3Dv2");
	Graphics::Get().Startup(GraphicsAPI::DIRECTX11, 800, 600);

	Shader* vertex = ShaderManager::Get().GetShader<ShaderType::VERTEX>("VertexShaderTest");
	Shader* fragment = ShaderManager::Get().GetShader<ShaderType::FRAGMET>("PixelShaderTest");

	vertex->Bind();
	fragment->Bind();

	ParametersBuffer* pb = vertex->FindParametersBuffer("PerObject");
	pb->SetFloat("Float", 0.2f);
	vertex->SetParametersBuffer(pb);

	VertexFormat* vf = VertexFormatManager::Get().GetVertexFormat<CommonVertex>(vertex);
	vf->Bind();

	///////////////////////////////// RAW DIRECTX /////////////////////////////////////////


	// Vertex buffer
	CommonVertex vertexArray[] = {
		{ 0.5f,0.5f,0.5f , 1,0},
		{ 0.5f,-0.5f,0.5f , 0,1},
		{ -0.5f,-0.5f,0.5f , 0,0},
		{ -0.5f,0.5f,0.5f , 1,0}
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CommonVertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = vertexArray;

	ID3D11Buffer *vertexBuffer;
	if (static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer) != S_OK) { return 1; }

	// Set Vertex buffer
	UINT stride = sizeof(CommonVertex);
	UINT offset = 0;
	static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Create indices
	uint32_t indicesArray[] = {
		0,1,2,
		0,2,3
	};

	D3D11_BUFFER_DESC indicesBufferDesc = {};
	indicesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indicesBufferDesc.ByteWidth = sizeof(uint32_t) * 6;
	indicesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indicesBufferData = {};
	indicesBufferData.pSysMem = indicesArray;

	ID3D11Buffer *indicesBuffer;
	static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateBuffer(&indicesBufferDesc, &indicesBufferData, &indicesBuffer);

	// Set Indices buffer;
	static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetIndexBuffer(indicesBuffer, DXGI_FORMAT_R32_UINT, 0);
	static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//////////////////////////////////////////////////////////////////////////

	while (Window::Get().ShouldWindowClose())
	{
		Window::Get().Update();
		Graphics::Get().Clear();
		///////////////////////////////// RAW DIRECTX /////////////////////////////////////////
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->DrawIndexed(6, 0, 0);
		//////////////////////////////////////////////////////////////////////////
		Graphics::Get().Update();
	}

	Graphics::Get().Shutdown();
	Window::Get().Shutdown();
	File::Get().Shutdown();

	return 0;
}