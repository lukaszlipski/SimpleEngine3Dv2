#pragma once
#include "../../Utility/Types.h"
#include "Shader.h"
#include <string>
#include "VertexFormat.h"

namespace SE3D2
{

	enum class GraphicsAPI : int8
	{
		DIRECTX11 = 0,
		OPENGL
	};

	class Context
	{

	public:
		
		static Context* Create(GraphicsAPI api);
		
		inline GraphicsAPI GetAPI() const { return mCurrentAPI; }

		virtual std::string GetName() = 0;
		virtual bool Init(uint32 width, uint32 height) = 0;
		virtual void ClearResources() = 0;
		virtual void Clear() = 0;
		virtual void SwapBuffers() = 0;
		virtual Shader* CreateShader(const std::string& name, ShaderType type) = 0;
		virtual ParametersBuffer* CreateParametersBuffer(const std::string& name, int32 size, int32 slot) = 0;
		virtual void SetVertexFormat(VertexFormat* format) = 0;

	protected:
		inline Context* SetAPI(GraphicsAPI api) 
		{
			mCurrentAPI = api;
			return this;
		}

	private:
		GraphicsAPI mCurrentAPI;

	};


}