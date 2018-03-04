#pragma once
#include "..\Graphic\API\ShaderPipeline.h"
#include "GL\glew.h"
#include "OGShader.h"

namespace SE3D2
{
	class OGShaderPipeline : public ShaderPipeline
	{
	public:
		OGShaderPipeline(std::initializer_list<Shader*> stages)
			: ShaderPipeline(stages)
		{
			glGenProgramPipelines(1, &mPipe);
			Bind();

			if (mStages[0])
			{
				glUseProgramStages(mPipe, GL_VERTEX_SHADER_BIT, static_cast<OGVertexShader*>(mStages[0])->GetProgram());
			}
			if (mStages[4])
			{
				glUseProgramStages(mPipe, GL_FRAGMENT_SHADER_BIT, static_cast<OGFragmentShader*>(mStages[4])->GetProgram());
			}

			Unbind();

		}

		virtual void Bind() override
		{
			glBindProgramPipeline(mPipe);
		}


		virtual void ClearResource() override
		{
			glDeleteProgramPipelines(1, &mPipe);
		}


		virtual void Unbind() override
		{
			glBindProgramPipeline(0);
		}

	private:
		uint32 mPipe;

	};
}