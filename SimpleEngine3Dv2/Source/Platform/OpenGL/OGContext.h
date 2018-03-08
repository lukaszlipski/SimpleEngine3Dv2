#pragma once
#include "..\Graphic\API\Context.h"
#include "..\Graphic\API\Buffer.h"

namespace SE3D2
{
	class OGContext : public Context
	{
	public:
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void ClearResources() override;
		virtual void Clear() override;
		virtual void SwapBuffers() override;
		virtual Shader* CreateShader(const std::string& name, ShaderType type) override;
		virtual std::string GetName() override { return "OpenGL"; }
		virtual ParametersBuffer* CreateParametersBuffer(const std::string& name, int32 size, int32 slot) override;
		virtual VertexFormat* CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc) override;
		virtual VertexBuffer* CreateVertexBuffer(int32 size, void* data = nullptr) override;
		virtual IndexBuffer* CreateIndexBuffer(int32 size, void* data = nullptr) override;
		virtual ShaderPipeline* CreateShaderPipeline(std::initializer_list<Shader *> stages) override;

	private:
		void* mOpenGLContext;

	};
}