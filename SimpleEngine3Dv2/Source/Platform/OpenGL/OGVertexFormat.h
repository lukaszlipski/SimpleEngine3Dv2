#pragma once
#include "..\Graphic\API\VertexFormat.h"
#include "GL\glew.h"
#include <assert.h>

namespace SE3D2
{
	class OGVertexFormat : public VertexFormat
	{
	public:
		OGVertexFormat(const VertexFormatDec& vertexFormatDesc)
			: VertexFormat(vertexFormatDesc)
		{ }


		virtual void Bind() override
		{
			for (int32 i = 0; i < mVertexFormatDescription.Members.size(); ++i)
			{
				VertexAttribute va = CommonVertex::VertexFormatInfo.Members[i];
				glEnableVertexAttribArray(i);
				
				if (va.Type == "Vector3")
				{
					glVertexAttribFormat(i, va.Size, GL_FLOAT_VEC3, GL_FALSE, va.Offset);
				}
				else if (va.Type == "Vector2")
				{
					glVertexAttribFormat(i, va.Size, GL_FLOAT_VEC2, GL_FALSE, va.Offset);
				}
				else if (va.Type == "float")
				{
					glVertexAttribFormat(i, va.Size, GL_FLOAT, GL_FALSE, va.Offset);
				}
				else if (va.Type == "int32")
				{
					glVertexAttribFormat(i, va.Size, GL_INT, GL_FALSE, va.Offset);
				}
				else
				{
					// #TODO: Unsupported type
					assert(false);
				}

				glVertexAttribBinding(i, 0);
			}
		}


		virtual bool Create(class Shader* shader) override
		{
			return true;
		}

	};
}