#pragma once
#include <vector>
#include <memory>
#include "API\Shader.h"
#include "API\VertexFormat.h"
#include "..\System\Graphics.h"
#include <assert.h>

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

		template<typename T>
		VertexFormat* GetVertexFormat(Shader* vertexShader)
		{
			// Try to find vertex format
			for (auto& format : mVertexFormats)
			{
				if (format->GetName() == T::VertexFormatInfo.Name)
				{
					return format.get();
				}
			}

			// #TODO: make it thread safe

			// Add because vertex format not found
			VertexFormat* vf = Graphics::Get().GetContext()->CreateVertexFormat(vertexShader, T::VertexFormatInfo);
			if (!vf)
			{
				// Cannot create vertex format
				assert(false);
			}

			mVertexFormats.push_back(std::unique_ptr<VertexFormat>(vf));

			return mVertexFormats.back().get();
		}

	private:
		VertexFormatManager() = default;

		std::vector<std::unique_ptr<VertexFormat>> mVertexFormats;

	};
}