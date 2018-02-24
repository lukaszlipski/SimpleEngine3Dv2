#include "Dx11Buffer.h"

namespace SE3D2
{

	D3D11_BUFFER_DESC Dx11VertexBufferPolicy::GetBufferDesc(int32 size)
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = size;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		return BufferDesc;
	}

	D3D11_BUFFER_DESC Dx11IndexBufferPolicy::GetBufferDesc(int32 size)
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = size;
		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		return BufferDesc;
	}

	D3D11_BUFFER_DESC Dx11ConstantBufferPolicy::GetBufferDesc(int32 size)
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = size;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		return BufferDesc;
	}

}