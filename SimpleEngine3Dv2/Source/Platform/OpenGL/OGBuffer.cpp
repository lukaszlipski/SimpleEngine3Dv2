#include "OGBuffer.h"

namespace SE3D2
{

	bool OGBuffer::CreateBuffer(int32 size, void* data)
	{
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GetBufferType(), mBuffer);
		glBufferData(GetBufferType(), size, data, GL_STATIC_DRAW);
		glBindBuffer(GetBufferType(), 0);
		return true;
	}

	bool OGBuffer::UpdateBuffer(int32 size, int32 offset, void* data)
	{
		if (data)
		{
			glBindBuffer(GetBufferType(), mBuffer);
			glBufferSubData(GetBufferType(), offset, size, data);
			glBindBuffer(GetBufferType(), 0);

			// #TODO: check for operation result
		}

		return true;
	}

	void OGBuffer::DeleteBuffer()
	{
		glDeleteBuffers(1, &mBuffer);
	}


	bool OGVertexBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool OGVertexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	bool OGIndexBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool OGIndexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	bool OGUniformBlockBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;

		return CreateBuffer(size, data);
	}

	bool OGUniformBlockBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

}