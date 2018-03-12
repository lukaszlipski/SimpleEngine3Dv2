#pragma once
#include <d3d11.h>
#include "..\Graphic\API\Buffer.h"
#include "..\System\Graphics.h"
#include "Dx11Context.h"


namespace SE3D2
{

	class Dx11Buffer
	{
	public:
		ID3D11Buffer* GetBuffer() const { return mBuffer; }

	protected:
		bool CreateBuffer(int32 size, void* data);
		bool UpdateBuffer(int32 size, int32 offset, void* data);
		void DeleteBuffer();

		virtual D3D11_BUFFER_DESC GetBufferDesc() = 0;

		ID3D11Buffer* mBuffer;

	};

	class Dx11VertexBuffer : public VertexBuffer, public Dx11Buffer
	{
	public:
		virtual BufferType GetType() const override { return BufferType::VERTEX; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); };

		virtual void Bind(uint32 stride, uint32 offset = 0) override;

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};

	class Dx11IndexBuffer : public IndexBuffer, public Dx11Buffer
	{
	public:
		virtual BufferType GetType() const override { return BufferType::INDEX; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); };

		virtual void Bind() override;

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};

	class Dx11ConstantBuffer : public ConstantBuffer, public Dx11Buffer
	{
	public:
		virtual BufferType GetType() const override { return BufferType::CONSTANT; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); };

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};

	
	class Dx11StructuredBuffer : public StructuredBuffer, public Dx11Buffer
	{
	public:
		Dx11StructuredBuffer(int32 stride)
			: mStride(stride)
		{}

		virtual BufferType GetType() const override { return BufferType::STRUCTURED; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override
		{
			if (mView) mView->Release();
			if (mUView) mUView->Release();
			DeleteBuffer(); 
			SetInvalid();
		};

		ID3D11UnorderedAccessView* GetUAV() const { return mUView; }
		ID3D11ShaderResourceView* GetRV() const { return mView; }

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

		ID3D11UnorderedAccessView* mUView;
		ID3D11ShaderResourceView* mView;

		int32 mStride;

	};

}
