#include "Dx11Buffer.h"
#include <assert.h>

namespace SE3D2
{

	bool Dx11Buffer::CreateBuffer(int32 size, void* data)
	{

		D3D11_BUFFER_DESC BufferDesc = GetBufferDesc();

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

	bool Dx11Buffer::UpdateBuffer(int32 size, int32 offset, void* data)
	{
		if (data)
		{
			Dx11Context* dic = static_cast<Dx11Context*>(Graphics::Get().GetContext());
			dic->GetImmediateContext()->UpdateSubresource(mBuffer, 0, 0, data, 0, 0);

			// #TODO: check for operation result
		}

		return true;
	}

	void Dx11Buffer::DeleteBuffer()
	{
		mBuffer->Release();
	}

	bool Dx11VertexBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool Dx11VertexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	D3D11_BUFFER_DESC Dx11VertexBuffer::GetBufferDesc()
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = mSize;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		return BufferDesc;
	}

	D3D11_BUFFER_DESC Dx11IndexBuffer::GetBufferDesc()
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = mSize;
		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		return BufferDesc;
	}

	bool Dx11IndexBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool Dx11IndexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	D3D11_BUFFER_DESC Dx11ConstantBuffer::GetBufferDesc()
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = mSize;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		return BufferDesc;
	}

	bool Dx11ConstantBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool Dx11ConstantBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

}