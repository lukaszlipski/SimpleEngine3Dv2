#include "Graphics.h"
#include "..\Platform\Directx11\Dx11Context.h"

namespace SE3D2
{

	bool Graphics::Startup(GraphicsAPI api,uint32 width, uint32 height)
	{
		mContext = Context::Create(api);
		mContext->Init(width, height);
		return true;
	}

	bool Graphics::Shutdown()
	{
		mContext->ClearResources();
		delete mContext;
		return true;
	}

	void Graphics::Clear()
	{
		mContext->Clear();
	}

	void Graphics::Update()
	{
		mContext->SwapBuffers();
	}

}