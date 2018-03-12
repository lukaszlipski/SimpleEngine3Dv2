#pragma once
#include "..\Utility\Types.h"
#include <assert.h>

namespace SE3D2
{
	enum class BufferType
	{
		VERTEX = 0,
		INDEX,
		CONSTANT,
		STRUCTURED
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
		inline bool IsValid() const { return mIsValid; }

	protected:
		inline void SetInvalid() { mIsValid = false; }
		inline void SetValid() { mIsValid = true; }

		int32 mSize;
		bool mIsValid = false;
	};

	class VertexBuffer : public Buffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind(uint32 stride, uint32 offset = 0) = 0;

	};

	enum class IndexType
	{
		UINT32,
		UINT16
	};

	class IndexBuffer : public Buffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;

		inline void SetType(IndexType type) { mType = type; }

		inline int32 GetIndicesNum()
		{
			switch (mType)
			{
			case IndexType::UINT32:
			{
				return GetSize() / sizeof(uint32);
			}
			case IndexType::UINT16:
			{
				return GetSize() / sizeof(uint16);
			}
			default:
			{
				// #TODO: unsupported type
				assert(false);
				return -1;
			}
			}
		}

	protected:
		IndexType mType = IndexType::UINT32;

	};

	class ConstantBuffer : public Buffer
	{
	public:
		virtual ~ConstantBuffer() = default;

	};

	class StructuredBuffer : public Buffer
	{
	public:
		virtual ~StructuredBuffer() = default;

	};

}