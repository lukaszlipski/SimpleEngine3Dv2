#pragma once
#include "..\..\Graphic\API\Context.h"

namespace SE3D2
{
	class Dx11Contex : public Context
	{
	public:
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void ClearResources() override;
		virtual void Clear() override;
		virtual void SwapBuffers() override;

	private:
		struct ID3D11Device* mD3D11Device;
		struct ID3D11DeviceContext* mD3D11DeviceCtx;
		struct IDXGISwapChain* mSwapChain;
		struct ID3D11RenderTargetView* mBackBufferRT;
		struct ID3D11DepthStencilView* mDepthStencilV;

	};
}