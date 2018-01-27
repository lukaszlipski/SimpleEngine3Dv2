#pragma once
#include "API\VertexFormat.h"
#include <map>
#include <memory>

namespace SE3D2
{
	class VertexFormatManager
	{
	public:
		static VertexFormatManager& Get()
		{
			static VertexFormatManager* instance = new VertexFormatManager();
			return *instance;
		}

		inline void Add(const std::string& name, std::unique_ptr<VertexFormat> vf)
		{
			mVertexFormats[name] = std::move(vf);
		}

		inline VertexFormat* GetVertexFormat(const std::string& name) { return mVertexFormats[name].get(); }

	private:
		std::map<std::string, std::unique_ptr<VertexFormat>> mVertexFormats;
	};
}