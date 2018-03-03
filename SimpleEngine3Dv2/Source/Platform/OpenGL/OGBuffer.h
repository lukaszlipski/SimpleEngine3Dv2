#pragma once
#include "..\Graphic\API\Buffer.h"
#include "GL\glew.h"

namespace SE3D2
{

	class OGVertexBufferPolicy
	{
	public:
		inline int32 GetTypeEnum() const { return GL_ARRAY_BUFFER; }
		inline BufferType GetType() const { return BufferType::Vertex; }
	};

	class OGIndexBufferPolicy
	{
	public:
		inline int32 GetTypeEnum() const { return GL_ELEMENT_ARRAY_BUFFER; }
		inline BufferType GetType() const { return BufferType::Index; }
	};

	class OGUniformBlockPolicy
	{
	public:
		inline int32 GetTypeEnum() const { return GL_UNIFORM_BUFFER; }
		inline BufferType GetType() const { return BufferType::Constant; }
	};

	template<typename T>
	class OGBuffer : public Buffer
	{
	public:

		OGBuffer(int32 size)
			: Buffer(size)
		{ }

		virtual bool Create(void* data = nullptr) override;
		virtual bool Update(void* data) override;
		virtual void ClearResource() override;

		inline uint32 GetBuffer() const { return mBuffer; }

	private:
		T mPolicy;
		uint32 mBuffer;

	};

	template<typename T>
	bool OGBuffer<T>::Create(void* data /*= nullptr*/)
	{
		mType = mPolicy.GetType();

		glGenBuffers(1, &mBuffer);
		glBindBuffer(mPolicy.GetTypeEnum(), mBuffer);
		glBufferData(mPolicy.GetTypeEnum(), GetSize(), data, GL_STATIC_DRAW);
		glBindBuffer(mPolicy.GetTypeEnum(), 0);
		return true;
	}

	template<typename T>
	bool OGBuffer<T>::Update(void* data)
	{
		if (data)
		{
			glBindBuffer(mPolicy.GetTypeEnum(), mBuffer);
			glBufferSubData(mPolicy.GetTypeEnum(), 0, GetSize(), data);
			glBindBuffer(mPolicy.GetTypeEnum(), 0);

			// #TODO: check for operation result
		}

		return true;
	}

	template<typename T>
	void OGBuffer<T>::ClearResource()
	{
		glDeleteBuffers(1, &mBuffer);
	}

	using OGVertexBuffer = OGBuffer<OGVertexBufferPolicy>;
	using OGIndexBuffer = OGBuffer<OGIndexBufferPolicy>;
	using OGUniformBlockBuffer = OGBuffer<OGUniformBlockPolicy>;
}