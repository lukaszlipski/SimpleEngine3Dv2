#pragma once
#include <d3d11.h>
#include "..\Graphic\API\Buffer.h"
#include "..\System\Graphics.h"
#include "Dx11Context.h"


namespace SE3D2
{

	class Dx11VertexBufferPolicy
	{
	public:
		D3D11_BUFFER_DESC GetBufferDesc(int32 size);
		inline BufferType GetType() const { return BufferType::Vertex; }
	};

	class Dx11IndexBufferPolicy
	{
	public:
		D3D11_BUFFER_DESC GetBufferDesc(int32 size);
		inline BufferType GetType() const { return BufferType::Index; }
	};

	class Dx11ConstantBufferPolicy
	{
	public:
		D3D11_BUFFER_DESC GetBufferDesc(int32 size);
		inline BufferType GetType() const { return BufferType::Constant; }
	};

	template<typename T>
	class Dx11Buffer : public Buffer
	{

	public:
		Dx11Buffer(int32 size)
			: Buffer(size)
		{ }

		virtual bool Create(void* data = nullptr) override
		{

			// Set type for buffer based on policy
			mType = mPolicy.GetType();

			D3D11_BUFFER_DESC BufferDesc = mPolicy.GetBufferDesc(GetSize());

			D3D11_SUBRESOURCE_DATA* BufferData = nullptr;

			// If data is not present, create empty buffer
			if (data)
			{
				D3D11_SUBRESOURCE_DATA ResourceData = {};
				ResourceData.pSysMem = data;
				BufferData = &ResourceData;
			}

			if (static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateBuffer(&BufferDesc, BufferData, &mBuffer) != S_OK)
			{ 
				return false; 
			}

			return true;
		}

		virtual bool Update(void* data) override
		{
			Dx11Context* dic = static_cast<Dx11Context*>(Graphics::Get().GetContext());
			dic->GetImmediateContext()->UpdateSubresource(mBuffer, 0, 0, data, 0, 0);

			// #TODO: check for operation result

			return true;
		}

		inline ID3D11Buffer* GetBuffer() const { return mBuffer; }

		virtual void ClearResource() override
		{
			mBuffer->Release();
		}

	private:
		T mPolicy;
		ID3D11Buffer* mBuffer;

	};

	using Dx11VertexBuffer = Dx11Buffer<Dx11VertexBufferPolicy>;
	using Dx11IndexBuffer = Dx11Buffer<Dx11IndexBufferPolicy>;
	using Dx11ConstantBuffer = Dx11Buffer<Dx11ConstantBufferPolicy>;

}
