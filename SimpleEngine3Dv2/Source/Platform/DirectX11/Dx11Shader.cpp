#include "Dx11Shader.h"
#include "..\System\File.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include "..\System\Graphics.h"
#include "Dx11Context.h"
#include "Dx11VertexFormat.h"
#include "..\Graphic\VertexFormatManager.h"

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

		// Create VertexFormat
		ID3D11InputLayout* InputLayout;

		ID3D11ShaderReflection* pReflector = {};
		D3DReflect(&source[0], source.length(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&pReflector));
		D3D11_SHADER_DESC Desc;
		pReflector->GetDesc(&Desc);

		std::vector<D3D11_INPUT_ELEMENT_DESC> InputElemDescArr;
		Dx11VertexFormat* vertexFormat = new Dx11VertexFormat();
		uint32 offset = 0;
		for (uint32_t i = 0; i < Desc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetInputParameterDesc(i, &paramDesc);
			
			D3D11_INPUT_ELEMENT_DESC Input = {};
			Input.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			std::string SemanticName(paramDesc.SemanticName);
			if (SemanticName == std::string("POSITION"))
			{
				vertexFormat->PushVertexAttribute(VertexAttributeType::POSITION);

				Input.SemanticName = "POSITION";
				Input.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				Input.AlignedByteOffset = offset;
				offset += static_cast<uint32>(vertexFormat->GetSizeForAttributeType(VertexAttributeType::POSITION));
			}
			else if (SemanticName == std::string("TEXCOORD"))
			{
				vertexFormat->PushVertexAttribute(VertexAttributeType::TEXTURECOORDINATES);

				Input.SemanticName = "TEXCOORD";
				Input.Format = DXGI_FORMAT_R32G32_FLOAT;
				Input.AlignedByteOffset = offset;
				offset += vertexFormat->GetSizeForAttributeType(VertexAttributeType::TEXTURECOORDINATES);
			}
			else
			{
				// #TODO : unsupported vertex attribute
				continue;
			}

			InputElemDescArr.push_back(Input);
		}

		hr = static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateInputLayout(InputElemDescArr.data(), static_cast<uint32>(InputElemDescArr.size()), source.c_str(), static_cast<uint32>(source.length()), &InputLayout);
		if (hr != S_OK) { 
			mShader->Release();
			return false; 
		}

		vertexFormat->SetInputLayout(InputLayout);

		VertexFormatManager::Get().Add(name, std::unique_ptr<VertexFormat>(vertexFormat));

		pReflector->Release();

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

