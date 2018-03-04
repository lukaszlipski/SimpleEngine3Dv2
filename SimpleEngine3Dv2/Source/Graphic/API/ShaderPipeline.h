#pragma once
#include "Shader.h"
#include <stdarg.h>

namespace SE3D2
{
	class ShaderPipeline
	{
	public:
		ShaderPipeline(std::initializer_list<Shader*> stages)
		{
			for (auto& shader : stages)
			{
				if (shader->GetType() == ShaderType::VERTEX)
				{
					mStages[0] = shader;
				}
				else if (shader->GetType() == ShaderType::FRAGMET)
				{
					mStages[4] = shader;
				}
				else
				{
					// #TODO: Add more shader stages
				}
			}
			
		}

		virtual ~ShaderPipeline() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void ClearResource() = 0;

	protected:
		Shader* mStages[5];

	};
}