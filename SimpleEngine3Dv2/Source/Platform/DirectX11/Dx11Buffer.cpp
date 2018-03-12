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
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool Dx11VertexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	void Dx11VertexBuffer::Bind(uint32 stride, uint32 offset /*= 0*/)
	{
		UINT Stride = stride;
		UINT Offset = offset;
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetVertexBuffers(0, 1, &mBuffer, &Stride, &Offset);
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
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool Dx11IndexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	void Dx11IndexBuffer::Bind()
	{
		DXGI_FORMAT Format;
		switch (mType)
		{
		case IndexType::UINT32:
		{
			Format = DXGI_FORMAT_R32_UINT;
			break;
		}
		case IndexType::UINT16:
		{
			Format = DXGI_FORMAT_R16_UINT;
			break;
		}
		default:
		{
			// #TODO: unsupported format
			assert(false);
		}
		}
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetIndexBuffer(mBuffer, Format, 0);
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
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool Dx11ConstantBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	bool Dx11StructuredBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		if (mSize % mStride != 0)
		{
			// #TODO : Log - Size of the buffer is not divisible by given stride
			assert(false);
		}

		// Create buffer
		if (!CreateBuffer(size, data))
		{
			return false;
		}

		// Create unordered view for that buffer
		D3D11_UNORDERED_ACCESS_VIEW_DESC Uavd = {};
		Uavd.Buffer.NumElements = mSize / mStride;
		Uavd.Format = DXGI_FORMAT_UNKNOWN;
		Uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

		if (static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateUnorderedAccessView(mBuffer, &Uavd, &mUView) != S_OK)
		{
			ClearResource();
			return false;
		}

		// Create view for that buffer
		D3D11_SHADER_RESOURCE_VIEW_DESC Shvd = {};
		Shvd.Format = DXGI_FORMAT_UNKNOWN;
		Shvd.Buffer.NumElements = mSize / mStride;
		Shvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		
		if (static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateShaderResourceView(mBuffer, &Shvd, &mView) != S_OK)
		{
			ClearResource();
			return false;
		}

		SetValid();

		return true;
	}

	bool Dx11StructuredBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	D3D11_BUFFER_DESC Dx11StructuredBuffer::GetBufferDesc()
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = mSize;
		BufferDesc.StructureByteStride = mStride;
		BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		return BufferDesc;
	}

}