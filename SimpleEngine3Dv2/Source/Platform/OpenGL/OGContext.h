#pragma once
#include "..\Graphic\API\Context.h"

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


	private:
		void* mOpenGLContext;

	};
}