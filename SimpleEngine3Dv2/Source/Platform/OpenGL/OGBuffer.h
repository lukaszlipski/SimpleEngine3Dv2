#pragma once
#include "..\Graphic\API\Buffer.h"
#include "GL\glew.h"

namespace SE3D2
{

	class OGBuffer
	{
	public:
		inline uint32 GetBuffer() const { return mBuffer; }

	protected:
		bool CreateBuffer(int32 size, void* data);
		bool UpdateBuffer(int32 size, int32 offset, void* data);
		void DeleteBuffer();

		uint32 mBuffer;

		virtual uint32 GetBufferType() const = 0;
		virtual uint32 GetAccessType() const = 0;

	};

	class OGVertexBuffer : public VertexBuffer, public OGBuffer
	{

	public:
		virtual bool Create(int32 size, void* data = nullptr) override;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); }
		virtual BufferType GetType() const override { return BufferType::VERTEX; };

		virtual void Bind(uint32 stride, uint32 offset = 0) override;

	protected:
		virtual uint32 GetBufferType() const override { return GL_ARRAY_BUFFER; }
		virtual uint32 GetAccessType() const override { return GL_STATIC_DRAW; };

	};

	class OGIndexBuffer : public IndexBuffer, public OGBuffer
	{

	public:
		virtual bool Create(int32 size, void* data = nullptr) override;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); }
		virtual BufferType GetType() const override { return BufferType::INDEX; };

		virtual void Bind() override;

	protected:
		virtual uint32 GetBufferType() const override { return GL_ELEMENT_ARRAY_BUFFER; }
		virtual uint32 GetAccessType() const override { return GL_STATIC_DRAW; };

	};

	class OGUniformBlockBuffer : public ConstantBuffer, public OGBuffer
	{

	public:
		virtual bool Create(int32 size, void* data = nullptr) override;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); }
		virtual BufferType GetType() const override { return BufferType::CONSTANT; };

	protected:
		virtual uint32 GetBufferType() const override { return GL_UNIFORM_BUFFER; }
		virtual uint32 GetAccessType() const override { return GL_STATIC_DRAW; };

	};

	class OGStructuredBuffer : public StructuredBuffer, public OGBuffer
	{

	public:
		virtual bool Create(int32 size, void* data = nullptr) override;
		virtual bool Update(int32 size, int32 offset, void* data) override;;
		virtual void ClearResource() override { DeleteBuffer(); SetInvalid(); }
		virtual BufferType GetType() const override { return BufferType::STRUCTURED; };

	protected:
		virtual uint32 GetBufferType() const override { return GL_SHADER_STORAGE_BUFFER; }
		virtual uint32 GetAccessType() const override { return GL_DYNAMIC_COPY; };

	};

}