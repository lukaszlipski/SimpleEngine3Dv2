#pragma once
#include "..\Utility\Types.h"
#include "ParametersBuffer.h"
#include <string>
#include <vector>
#include "Buffer.h"

namespace SE3D2
{
	enum class ShaderType
	{
		VERTEX = 0,
		PIXEL,
		COMPUTE
	};

	struct ShaderBuffer
	{
		std::string mName;
		uint32 mSize;
		uint32 mSlot;
	};

	class Shader
	{
	public:
		Shader(const std::string name)
			: mName(name)
		{}

		virtual ~Shader()
		{
			for (auto& pb : mParametersBuffers)
			{
				pb->ClearResources();
			}
		}

		virtual bool Compile(const std::string& name) = 0;
		virtual void SetParametersBuffer(ParametersBuffer* pb, uint32 globalSlot = 0) = 0;
		virtual bool SetStructuredBuffer(const std::string& name, StructuredBuffer* sb = nullptr) = 0;

		virtual std::string GetExtension() const = 0;
		std::string GetCompiledExtension() const { return "cs"; }
		inline ShaderType GetType() const { return mType; }
		inline std::string GetName() const { return mName; }
		ParametersBuffer* FindParametersBuffer(const std::string& name) const;
		std::string GetSource() const { return mSource; }

	protected:
		std::string mName;
		ShaderType mType;
		std::string mSource;
		std::vector<std::unique_ptr<ParametersBuffer>> mParametersBuffers;
		std::vector<ShaderBuffer> mStructuredBuffers;

		virtual bool CollectResources(const std::string& source) = 0;
		bool TryToFindCompiledFile(const std::string& name, std::string& source);
		bool TryToFindSourceFile(const std::string& name, std::string& source);
		bool SaveCompiledFile(const std::string& name, const void* binary, int32 size);

	};

}