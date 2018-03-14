#pragma once
#include "../../Utility/Types.h"
#include <string>
#include "Shader.h"
#include "VertexFormat.h"
#include "ShaderPipeline.h"
#include "Buffer.h"

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
		virtual ~Context() = default;
		
		static Context* Create(GraphicsAPI api);
		
		inline GraphicsAPI GetAPI() const { return mCurrentAPI; }

		virtual std::string GetName() = 0;
		virtual bool Init(uint32 width, uint32 height) = 0;
		virtual void ClearResources() = 0;
		virtual void Clear() = 0;
		virtual void SwapBuffers() = 0;
		virtual Shader* CreateShader(const std::string& name, ShaderType type) = 0;
		virtual ParametersBuffer* CreateParametersBuffer(const std::string& name, int32 size, int32 slot) = 0;
		virtual VertexFormat* CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc) = 0;
		virtual VertexBuffer* CreateVertexBuffer(int32 size, void* data = nullptr) = 0;
		virtual IndexBuffer* CreateIndexBuffer(int32 size, void* data = nullptr) = 0;
		virtual StructuredBuffer* CreateStructuredBuffer(int32 stride, int32 size, void* data = nullptr) = 0;
		virtual ShaderPipeline* CreateShaderPipeline(std::initializer_list<Shader*> stages) = 0;
		virtual bool SetComputeShader(Shader* shader) = 0;
		virtual void Dispatch(int32 x, int32 y, int32 z) = 0;

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