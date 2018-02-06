#pragma once
#include "..\..\Graphic\API\Context.h"

namespace SE3D2
{
	class Dx11Context : public Context
	{
	public:
		
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void ClearResources() override;
		virtual void Clear() override;
		virtual void SwapBuffers() override;
		virtual Shader* CreateShader(const std::string& name, ShaderType type) override;
		virtual ParametersBuffer* CreateParametersBuffer(const std::string& name, int32 size, int32 slot) override;
		virtual std::string GetName() override { return "DirectX11"; }
		virtual VertexFormat* CreateVertexFormat(Shader* vertexShader, const VertexFormatDec& vertexFormatDesc) override;

		inline struct ID3D11Device* GetDevice() const { return mD3D11Device; }
		inline struct ID3D11DeviceContext* GetImmediateContext() const { return mD3D11DeviceCtx; }


	private:
		struct ID3D11Device* mD3D11Device;
		struct ID3D11DeviceContext* mD3D11DeviceCtx;
		struct IDXGISwapChain* mSwapChain;
		struct ID3D11RenderTargetView* mBackBufferRT;
		struct ID3D11DepthStencilView* mDepthStencilV;

	};
}