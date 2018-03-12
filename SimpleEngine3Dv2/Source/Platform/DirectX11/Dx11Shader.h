#pragma once
#include "..\Graphic\API\Shader.h"
#include "Dx11ParametersBuffer.h"
#include <d3d11shader.h>
#include <string>
#include <d3dcompiler.h>
#include "..\System\Graphics.h"
#include <assert.h>
#include "..\Graphic\API\Buffer.h"

namespace SE3D2
{
	class Dx11PixelShaderPolicy
	{
	public:
		bool Compile(const std::string& name, std::string& source, bool shouldCompile);
		void Bind();
		void SetConstantBuffer(Dx11ParametersBuffer* pb);
		void SetStructuredBuffer(Dx11StructuredBuffer* sb, uint32 slot);
		ID3D11PixelShader* GetShader() const { return mShader; }
		inline ShaderType GetType() { return ShaderType::PIXEL; }
	private:
		ID3D11PixelShader* mShader;
	};

	class Dx11VertexShaderPolicy
	{
	public:
		bool Compile(const std::string& name, std::string& source, bool shouldCompile);
		void Bind();
		void SetConstantBuffer(Dx11ParametersBuffer* pb);
		void SetStructuredBuffer(Dx11StructuredBuffer* sb, uint32 slot);
		ID3D11VertexShader* GetShader() const { return mShader; }
		inline ShaderType GetType() { return ShaderType::VERTEX; }
	private:
		ID3D11VertexShader* mShader;
	};

	class Dx11ComputeShaderPolicy
	{
	public:
		bool Compile(const std::string& name, std::string& source, bool shouldCompile);
		void Bind();
		void SetConstantBuffer(Dx11ParametersBuffer* pb);
		void SetStructuredBuffer(Dx11StructuredBuffer* sb, uint32 slot);
		ID3D11ComputeShader* GetShader() const { return mShader; }
		inline ShaderType GetType() { return ShaderType::COMPUTE; }
	private:
		ID3D11ComputeShader * mShader;
	};


	template<typename T>
	class Dx11Shader : public Shader
	{
	public:
		Dx11Shader(const std::string& name)
			: Shader(name)
		{ 
			mType = mShaderPolicy.GetType();
		}

		~Dx11Shader()
		{
			mShaderPolicy.GetShader()->Release();
		}

		virtual bool Compile(const std::string& name) override;
		virtual std::string GetExtension() const override { return "hlsl"; }
		virtual void SetParametersBuffer(ParametersBuffer* pb, uint32 globalSlot = 0) override;
		virtual bool SetStructuredBuffer(const std::string& name, StructuredBuffer* sb = nullptr) override;

		void Bind() { mShaderPolicy.Bind(); }

	protected:
		virtual bool CollectResources(const std::string& source) override;

	private:
		T mShaderPolicy;

		void ProcessCBuffer(ID3D11ShaderReflectionConstantBuffer* ConstBuffer, D3D11_SHADER_BUFFER_DESC Desc, int32 index);

	};

	template<typename T>
	bool Dx11Shader<T>::SetStructuredBuffer(const std::string& name, StructuredBuffer* sb)
	{
		for (auto& Buff : mStructuredBuffers)
		{
			if (Buff.mName == name)
			{
				mShaderPolicy.SetStructuredBuffer(static_cast<Dx11StructuredBuffer*>(sb), Buff.mSlot);
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool Dx11Shader<T>::Compile(const std::string& name)
	{
		if (Path::GetExtension(name) != "")
		{
			return false;
		}

		bool Compiled = true;
		if (!TryToFindCompiledFile(name, mSource))
		{
			Compiled = false;
			mSource.clear();
			if (!TryToFindSourceFile(name, mSource))
			{
				// Neither compiled nor source files were found
				return false;
			}
		}

		if (!mShaderPolicy.Compile(name, mSource, Compiled))
		{
			return false;
		}

		if (!Compiled)
		{
			SaveCompiledFile(name, mSource.data(), static_cast<int32>(mSource.size()));
		}

		return CollectResources(mSource);
	}

	template<typename T>
	void Dx11Shader<T>::SetParametersBuffer(ParametersBuffer* pb, uint32 globalSlot)
	{
		for (auto& paramBuffer : mParametersBuffers)
		{
			if (paramBuffer->GetName() == pb->GetName() && paramBuffer->GetDataSize() == pb->GetDataSize() && paramBuffer->GetSlot() == pb->GetSlot())
			{
				pb->Update();
				Dx11ParametersBuffer* dxPb = static_cast<Dx11ParametersBuffer*>(pb);
				mShaderPolicy.SetConstantBuffer(dxPb);
			}
		}
	}

	template<typename T>
	bool Dx11Shader<T>::CollectResources(const std::string& source)
	{
		// Clear previous buffer if there was any
		mParametersBuffers.clear();

		ID3D11ShaderReflection* pReflector = {};
		D3DReflect(&source[0], source.length(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&pReflector));
		D3D11_SHADER_DESC Desc;
		pReflector->GetDesc(&Desc);
		
		// Get constant buffers and structured buffers
		for (uint32 i = 0; i < Desc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer* ConstBuffer = pReflector->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			ConstBuffer->GetDesc(&BufferDesc);

			if (BufferDesc.Type == D3D_CT_CBUFFER) //  constant buffer
			{
				int32 slot = static_cast<int32>(mParametersBuffers.size()); // #TODO : thread safe ?
				ProcessCBuffer(ConstBuffer, BufferDesc, slot);
			}
			else if (BufferDesc.Type == D3D_CT_RESOURCE_BIND_INFO) // structured buffer
			{
				uint32 slot = static_cast<uint32>(mStructuredBuffers.size()); // #TODO : thread safe ?
				mStructuredBuffers.push_back({ BufferDesc.Name, BufferDesc.Size, slot });
				
			}
			else
			{
				// #TODO: unsupported constant buffer type
			}
		}
		

		pReflector->Release();
		return true;
	}

	template<typename T>
	void SE3D2::Dx11Shader<T>::ProcessCBuffer(ID3D11ShaderReflectionConstantBuffer* ConstBuffer, D3D11_SHADER_BUFFER_DESC Desc, int32 index)
	{
		
		ParametersBuffer* TmpBuffer = Graphics::Get().GetContext()->CreateParametersBuffer(Desc.Name, Desc.Size, index);
		if (!TmpBuffer)
		{
			return;
		}
		std::unique_ptr<ParametersBuffer> Buffer(TmpBuffer);

		for (uint32 v = 0; v < Desc.Variables; ++v)
		{
			ID3D11ShaderReflectionVariable* Variable = {};
			Variable = ConstBuffer->GetVariableByIndex(v);
			D3D11_SHADER_VARIABLE_DESC vDesc = {};
			Variable->GetDesc(&vDesc);

			ID3D11ShaderReflectionType* Type = Variable->GetType();
			D3D11_SHADER_TYPE_DESC tDesc = {};
			Type->GetDesc(&tDesc);

			Parameter param;
			param.mName = vDesc.Name;
			param.mOffset = vDesc.StartOffset;
			if (tDesc.Class == D3D_SVC_SCALAR && tDesc.Type == D3D_SVT_FLOAT)
			{
				param.mType = ParameterType::FLOAT;
			}
			else if (tDesc.Class == D3D_SVC_SCALAR && tDesc.Type == D3D_SVT_INT)
			{
				param.mType = ParameterType::INT32;
			}
			else if (tDesc.Class == D3D_SVC_VECTOR && tDesc.Type == D3D_SVT_FLOAT && tDesc.Columns == 2)
			{
				param.mType = ParameterType::VECTOR2;
			}
			else if (tDesc.Class == D3D_SVC_VECTOR && tDesc.Type == D3D_SVT_FLOAT && tDesc.Columns == 3)
			{
				param.mType = ParameterType::VECTOR3;
			}
			else if (tDesc.Class == D3D_SVC_VECTOR && tDesc.Type == D3D_SVT_FLOAT && tDesc.Columns == 4)
			{
				param.mType = ParameterType::VECTOR4;
			}
			else if (tDesc.Class == D3D_SVC_MATRIX_COLUMNS && tDesc.Type == D3D_SVC_MATRIX_COLUMNS && tDesc.Columns == 3)
			{
				param.mType = ParameterType::MATRIX3x3;
			}
			else if (tDesc.Class == D3D_SVC_MATRIX_COLUMNS && tDesc.Type == D3D_SVC_MATRIX_COLUMNS && tDesc.Columns == 4)
			{
				param.mType = ParameterType::MATRIX4x4;
			}
			else
			{
				// #TODO: add to log (unsupported type)
				param.mType = ParameterType::NONE;
				assert(false);
			}

			Buffer->AddParameter(param);
		}

		mParametersBuffers.push_back(std::move(Buffer));
	}

	using Dx11VertexShader = Dx11Shader<Dx11VertexShaderPolicy>;
	using Dx11PixelShader = Dx11Shader<Dx11PixelShaderPolicy>;
	using Dx11ComputeShader = Dx11Shader<Dx11ComputeShaderPolicy>;

}



