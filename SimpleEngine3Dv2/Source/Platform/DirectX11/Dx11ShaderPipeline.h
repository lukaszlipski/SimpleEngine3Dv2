#pragma once
#include "..\Graphic\API\ShaderPipeline.h"
#include <d3d11.h>
#include "Dx11Context.h"
#include "..\System\Graphics.h"
#include "Dx11Shader.h"

namespace SE3D2
{
	class Dx11ShaderPipeline : public ShaderPipeline
	{

	public:
		Dx11ShaderPipeline(std::initializer_list<Shader*> stages)
			: ShaderPipeline(stages)
		{ }

		virtual void Bind() override
		{
			if (mStages[0] && mStages[0]->GetType() == ShaderType::VERTEX)
			{
				static_cast<Dx11VertexShader*>(mStages[0])->Bind();
			}
			if (mStages[4] && mStages[4]->GetType() == ShaderType::FRAGMET)
			{
				static_cast<Dx11PixelShader*>(mStages[4])->Bind();
			}
		}


		virtual void Unbind() override
		{ }


		virtual void ClearResource() override
		{ }

	};
}