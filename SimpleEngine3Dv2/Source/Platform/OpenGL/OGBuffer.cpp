#include "OGBuffer.h"

namespace SE3D2
{

	bool OGBuffer::CreateBuffer(int32 size, void* data)
	{
		glGenBuffers(1, &mBuffer);
		glBindBuffer(GetBufferType(), mBuffer);
		glBufferData(GetBufferType(), size, data, GetAccessType());
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
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool OGVertexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	void OGVertexBuffer::Bind(uint32 stride, uint32 offset /*= 0*/)
	{
		glBindVertexBuffer(0, mBuffer, offset, stride);
	}

	bool OGIndexBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool OGIndexBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	void OGIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
	}

	bool OGUniformBlockBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool OGUniformBlockBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

	bool OGStructuredBuffer::Create(int32 size, void* data /*= nullptr*/)
	{
		mSize = size;
		if (!CreateBuffer(size, data))
		{
			return false;
		}
		SetValid();
		return true;
	}

	bool OGStructuredBuffer::Update(int32 size, int32 offset, void* data)
	{
		return UpdateBuffer(size, offset, data);
	}

}