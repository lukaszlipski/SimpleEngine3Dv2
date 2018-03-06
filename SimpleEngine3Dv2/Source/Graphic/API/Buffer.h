#pragma once
#include "..\Utility\Types.h"

namespace SE3D2
{
	enum class BufferType
	{
		Vertex = 0,
		Index,
		Constant
	};

	class Buffer
	{
	public:
		virtual ~Buffer() = default;

		virtual bool Create(int32 size, void* data = nullptr) = 0;
		virtual bool Update(int32 size, int32 offset, void* data) = 0;
		virtual void ClearResource() = 0;
		virtual BufferType GetType() const = 0;

		inline int32 GetSize() const { return mSize; }

	protected:
		int32 mSize;
	};

	class VertexBuffer : public Buffer
	{
	public:
		virtual ~VertexBuffer() = default;

	};

	class IndexBuffer : public Buffer
	{
	public:
		virtual ~IndexBuffer() = default;

	};

	class ConstantBuffer : public Buffer
	{
	public:
		virtual ~ConstantBuffer() = default;

	};

}