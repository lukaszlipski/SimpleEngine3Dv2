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
		virtual BufferType GetType() const override { return BufferType::Vertex; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); };

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};

	class Dx11IndexBuffer : public IndexBuffer, public Dx11Buffer
	{
	public:
		virtual BufferType GetType() const override { return BufferType::Index; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); };

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};

	class Dx11ConstantBuffer : public ConstantBuffer, public Dx11Buffer
	{
	public:
		virtual BufferType GetType() const override { return BufferType::Constant; };

		virtual bool Create(int32 size, void* data = nullptr) override;;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); };

	protected:
		virtual D3D11_BUFFER_DESC GetBufferDesc() override;

	};


}
