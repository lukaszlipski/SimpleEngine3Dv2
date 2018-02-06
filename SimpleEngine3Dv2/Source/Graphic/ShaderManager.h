#pragma once
#include "API\Shader.h"
#include <vector>

namespace SE3D2
{
	class ShaderManager
	{
	public:
		static ShaderManager& Get()
		{
			static ShaderManager* instance = new ShaderManager();
			return *instance;
		}

		template<ShaderType T>
		Shader* GetShader(const std::string& name)
		{
			for (auto& shader : mShaders)
			{
				if (shader->GetName() == name && shader->GetType() == T)
				{
					return shader.get();
				}
			}

			std::unique_ptr<Shader> NewShader(Graphics::Get().GetContext()->CreateShader(name, T));
			if (NewShader.get())
			{
				mShaders.push_back(std::move(NewShader));
				return mShaders.back().get(); // #TODO : make it thread safe
			}
			return nullptr;
		}

	private:
		ShaderManager() {}

		std::vector<std::unique_ptr<Shader>> mShaders;

	};
}
