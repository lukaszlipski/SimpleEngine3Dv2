#pragma once
#include "..\Graphic\API\VertexFormat.h"
#include "..\Graphic\API\Shader.h"


namespace SE3D2
{
	class Dx11VertexFormat : public VertexFormat
	{

	public:
		Dx11VertexFormat(const VertexFormatDec& vertexFormatDesc)
			: VertexFormat(vertexFormatDesc)
		{ }

		
		virtual void Bind() override;
		virtual bool Create(Shader* shader) override;

	private:
		struct ID3D11InputLayout* mInputLayout;


	};
}