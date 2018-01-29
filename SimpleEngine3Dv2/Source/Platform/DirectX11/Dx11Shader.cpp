#include "Dx11Shader.h"
#include "..\System\File.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include "..\System\Graphics.h"
#include "Dx11Context.h"

namespace SE3D2
{

	bool Dx11PixelShaderPolicy::Compile(const std::string& name, std::string& source, bool shouldCompile)
	{
		HRESULT hr;
		ID3D10Blob* CompiledSource;
		if (!shouldCompile)
		{
			hr = D3DCompile(source.c_str(), source.length(), 0, 0, 0, "main", "ps_5_0", 0, 0, &CompiledSource, 0);
			// #TODO: Add log
			if (hr != S_OK) { return false; }
			source.clear();
			source.resize(CompiledSource->GetBufferSize());
			memcpy(&source[0], CompiledSource->GetBufferPointer(), CompiledSource->GetBufferSize());
			CompiledSource->Release();

		}
		hr = static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreatePixelShader(&source[0], source.length(), 0, reinterpret_cast<ID3D11PixelShader**>(&mShader));
		if (hr != S_OK) { return false; }
		return true;
	}

	void Dx11PixelShaderPolicy::Bind()
	{
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->PSSetShader(mShader, 0, 0);
	}

	void Dx11PixelShaderPolicy::SetConstantBuffer(Dx11ParametersBuffer* pb)
	{
		ID3D11Buffer* cb = pb->GetConstantBuffer();
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->PSSetConstantBuffers(0, 1, &cb);
	}

	bool Dx11VertexShaderPolicy::Compile(const std::string& name, std::string& source, bool shouldCompile)
	{
		HRESULT hr;
		ID3D10Blob* CompiledSource;
		if (!shouldCompile)
		{
			hr = D3DCompile(source.c_str(), source.length(), 0, 0, 0, "main", "vs_5_0", 0, 0, &CompiledSource, 0);
			// #TODO: Add log
			if (hr != S_OK) { return false; }
			source.clear();
			source.resize(CompiledSource->GetBufferSize());
			memcpy(&source[0], CompiledSource->GetBufferPointer(), CompiledSource->GetBufferSize());
			CompiledSource->Release();

		}
		hr = static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateVertexShader(&source[0], source.length(), 0, reinterpret_cast<ID3D11VertexShader**>(&mShader));
		if (hr != S_OK) { return false; }

		return true;
	}

	void Dx11VertexShaderPolicy::Bind()
	{
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->VSSetShader(mShader, 0, 0);
	}

	void Dx11VertexShaderPolicy::SetConstantBuffer(Dx11ParametersBuffer* pb)
	{
		ID3D11Buffer* cb = pb->GetConstantBuffer();
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->VSSetConstantBuffers(0, 1, &cb);
	}

}

