#include "Context.h"
#include "../../Platform/Directx11/Dx11Context.h"
#include "../Platform/OpenGL/OGContext.h"

namespace SE3D2
{

	SE3D2::Context* Context::Create(GraphicsAPI api)
	{
		switch (api)
		{
		case GraphicsAPI::DIRECTX11:
		{
			return (new Dx11Context())->SetAPI(api);
			break;
		}
		case GraphicsAPI::OPENGL:
		{
			return (new OGContext())->SetAPI(api);
			break;
		}
		}
		return nullptr;
	}

}