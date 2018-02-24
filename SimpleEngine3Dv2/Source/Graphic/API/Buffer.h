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
		Buffer(int32 size)
			: mSize(size)
		{ }

		virtual ~Buffer() = default;

		virtual bool Create(void* data = nullptr) = 0;
		virtual bool Update(void* data) = 0;
		virtual void ClearResource() = 0;

		inline BufferType GetType() const { return mType; }
		inline int32 GetSize() const { return mSize; }

	protected:
		int32 mSize;
		BufferType mType;

	};
}