#include <d3d11.h>
#include "../../System/Window.h"
#include "../System/File.h"
#include "Dx11Context.h"
#include "Dx11Shader.h"
#include "Dx11VertexFormat.h"
#include "Dx11Buffer.h"
#include "Dx11ShaderPipeline.h"

namespace SE3D2
{

	bool Dx11Context::Init(uint32 width, uint32 height)
	{
		// Create SwapChain, Dx11Device, Dx11DeviceContext
		DXGI_MODE_DESC swapChainModeDesc = {};
		swapChainModeDesc.Height = height;
		swapChainModeDesc.Width = width;
		swapChainModeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainModeDesc.RefreshRate.Numerator = 60;
		swapChainModeDesc.RefreshRate.Denominator = 1;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc = swapChainModeDesc;
		swapChainDesc.OutputWindow = Window::Get().GetWindowHandle();
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		
		if (D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0
			#if _DEBUG 
			| D3D11_CREATE_DEVICE_DEBUG
			#endif
			, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mD3D11Device, 0, &mD3D11DeviceCtx) != S_OK) 
		{
			return false;
		}

		// Create back buffer
		ID3D11Texture2D* BackBuffer;
		if (mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer)) != S_OK) 
		{
			ClearResources();
			return false;
		}

		if (mD3D11Device->CreateRenderTargetView(BackBuffer, 0, &mBackBufferRT) != S_OK) 
		{ 
			ClearResources();
			return false;
		}
		BackBuffer->Release();

		// Create depth and stencil buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Height = height;
		depthStencilDesc.Width = width;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D* DepthStencilTex;
		if (mD3D11Device->CreateTexture2D(&depthStencilDesc, 0, &DepthStencilTex) != S_OK) 
		{
			ClearResources();
			return false;
		}

		if (mD3D11Device->CreateDepthStencilView(DepthStencilTex, 0, &mDepthStencilV) != S_OK) 
		{ 
			ClearResources();
			return false;
		}

		// Create viewport
		D3D11_VIEWPORT Viewport = {};
		Viewport.Height = static_cast<float>(height);
		Viewport.Width = static_cast<float>(width);
		Viewport.MinDepth = 0.0f;
		Viewport.MaxDepth = 1.0f;

		mD3D11DeviceCtx->RSSetViewports(1, &Viewport);
		mD3D11DeviceCtx->OMSetRenderTargets(1, &mBackBufferRT, mDepthStencilV);

		return true;
	}

	void Dx11Context::ClearResources()
	{
		mDepthStencilV->Release();
		mBackBufferRT->Release();
		mSwapChain->Release();
		mD3D11DeviceCtx->Release();
		mD3D11Device->Release();
	}

	void Dx11Context::Clear()
	{
		const float ClearColor[] = { 0.0f,0.0f,0.0f,1.0f };
		mD3D11DeviceCtx->ClearRenderTargetView(mBackBufferRT, ClearColor);
		mD3D11DeviceCtx->ClearDepthStencilView(mDepthStencilV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Dx11Context::SwapBuffers()
	{
		mSwapChain->Present(0, 0);
	}

	Shader* Dx11Context::CreateShader(const std::string& name, ShaderType type)
	{
		Shader* NewShader = nullptr;
		switch (type)
		{
		case ShaderType::VERTEX:
		{
			NewShader = new Dx11Shader<Dx11VertexShaderPolicy>(name);
			break;
		}
		case ShaderType::PIXEL:
		{
			NewShader = new Dx11Shader<Dx11PixelShaderPolicy>(name);
			break;
		}
		case ShaderType::COMPUTE:
		{
			NewShader = new Dx11Shader<Dx11ComputeShaderPolicy>(name);
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

	ParametersBuffer* Dx11Context::CreateParametersBuffer(const std::string& name, int32 size, int32 slot)
	{

		Dx11ConstantBuffer* ConstantBuffer = new Dx11ConstantBuffer();
		if (ConstantBuffer->Create(size) != true)
		{
			return nullptr;
		}

		return new Dx11ParametersBuffer(name, size, slot, ConstantBuffer);

	}

	VertexFormat* Dx11Context::CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc)
	{
		Dx11VertexFormat* vertexFormat = new Dx11VertexFormat(vertexFormatDesc);
		if (!vertexFormat->Create(vertexShader))
		{
			return nullptr;
		}
		return vertexFormat;
	}

	VertexBuffer* Dx11Context::CreateVertexBuffer(int32 size, void* data /*= nullptr*/)
	{
		VertexBuffer* buffer = new Dx11VertexBuffer();
		if (data && size)
		{
			buffer->Create(size, data);
		}
		return buffer;
	}

	IndexBuffer* Dx11Context::CreateIndexBuffer(int32 size, void* data /*= nullptr*/)
	{
		IndexBuffer* buffer = new Dx11IndexBuffer();
		if (data)
		{
			buffer->Create(size, data);
		}
		return buffer;
	}

	ShaderPipeline* Dx11Context::CreateShaderPipeline(std::initializer_list<Shader *> stages)
	{
		return new Dx11ShaderPipeline(stages);
	}

	bool Dx11Context::SetComputeShader(Shader* shader)
	{
		if (shader->GetType() == ShaderType::COMPUTE)
		{
			((Dx11ComputeShader*)shader)->Bind();
			return true;
		}
		return false;
	}

	void Dx11Context::Dispatch(int32 x, int32 y, int32 z)
	{
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->Dispatch(x, y, z);
	}

	StructuredBuffer* Dx11Context::CreateStructuredBuffer(int32 stride, int32 size, void* data /*= nullptr*/)
	{
		StructuredBuffer* buffer = new Dx11StructuredBuffer(stride);
		
		buffer->Create(size, data);
		
		return buffer;
	}

}