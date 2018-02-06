#include <d3d11.h>
#include "Dx11VertexFormat.h"
#include "Dx11Context.h"
#include "..\System\Graphics.h"
#include <string>
#include <locale>
#include <algorithm>
#include <assert.h>

namespace SE3D2
{

	void Dx11VertexFormat::Bind()
	{
		static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetImmediateContext()->IASetInputLayout(mInputLayout);
	}

	bool Dx11VertexFormat::Create(Shader* shader)
	{
		// Only allow creation for vertex shader
		if (shader->GetType() != ShaderType::VERTEX)
		{
			return false;
		}
		
		// Create input layout description
		uint32 MembersSize = static_cast<uint32>(mVertexFormatDescription.Members.size());
		std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;
		std::vector<std::string> Names;
		Layout.reserve(MembersSize);
		Names.reserve(MembersSize);
		for (auto& attrib : mVertexFormatDescription.Members)
		{
			Names.push_back(attrib.Name);
			std::transform(Names.back().begin(), Names.back().end(), Names.back().begin(), ::toupper);

			DXGI_FORMAT Format;

			if (attrib.Type == "Vector3")
			{
				Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (attrib.Type == "Vector2")
			{
				Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (attrib.Type == "float")
			{
				Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (attrib.Type == "int32")
			{
				Format = DXGI_FORMAT_R32_SINT;
			}
			else
			{
				// Unsupported format
				assert(false);
			}
			
			Layout.push_back({ Names.back().c_str(), 0, Format, 0, attrib.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}


		if (static_cast<Dx11Context*>(Graphics::Get().GetContext())->GetDevice()->CreateInputLayout(Layout.data(), MembersSize, shader->GetSource().c_str(), shader->GetSource().length(), &mInputLayout) != S_OK)
		{
			return false;
		}

		return true;
	}

}