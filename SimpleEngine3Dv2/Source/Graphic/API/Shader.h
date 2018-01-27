#pragma once
#include "..\Utility\Types.h"
#include "ParametersBuffer.h"
#include <string>
#include <vector>

namespace SE3D2
{
	enum class ShaderType
	{
		VERTEX = 0,
		FRAGMET
	};

	class Shader
	{
	public:
		Shader(const std::string name)
			: mName(name)
		{}

		~Shader()
		{
			for (auto& pb : mParametersBuffers)
			{
				pb->ClearResources();
			}
		}

		virtual bool Compile(const std::string& name) = 0;
		virtual void Bind() = 0;
		virtual void SetParametersBuffer(ParametersBuffer* pb) = 0;
		virtual std::string GetExtension() const = 0;
		std::string GetCompiledExtension() const { return "cs"; }
		inline ShaderType GetType() const { return mType; }
		inline std::string GetName() const { return mName; }
		ParametersBuffer* FindParametersBuffer(const std::string& name) const;

	protected:
		std::string mName;
		ShaderType mType;
		std::vector<std::unique_ptr<ParametersBuffer>> mParametersBuffers;

		virtual bool CollectResources(const std::string& source) = 0;
		bool TryToFindCompiledFile(const std::string& name, std::string& source);
		bool TryToFindSourceFile(const std::string& name, std::string& source);
		bool SaveCompiledFile(const std::string& name, const void* binary, int32 size);

	};

}