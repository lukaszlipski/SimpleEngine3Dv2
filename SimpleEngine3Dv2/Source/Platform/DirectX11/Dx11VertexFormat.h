#pragma once
#include "..\Graphic\API\VertexFormat.h"
#include "..\System\Graphics.h"
#include <d3d11.h>

namespace SE3D2
{
	class Dx11VertexFormat : public VertexFormat
	{

	public:
		
		virtual void Bind() override
		{
			Graphics::Get().GetContext()->SetVertexFormat(this);
		}

		virtual void ClearResources() override
		{
			InputLayout->Release();
		}

		inline void SetInputLayout(ID3D11InputLayout* inputLayout) { InputLayout = inputLayout; }
		inline ID3D11InputLayout* GetInputLayout() const { return InputLayout; }

	private:
		ID3D11InputLayout* InputLayout;

	};
}