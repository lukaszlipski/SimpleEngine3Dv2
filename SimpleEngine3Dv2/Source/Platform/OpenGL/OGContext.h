#pragma once
#include "..\Graphic\API\Context.h"

namespace SE3D2
{
	class OGContext : public Context
	{
	public:
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void ClearResources() override;
		virtual void Clear() override;
		virtual void SwapBuffers() override;

	private:
		void* mOpenGLContext;

	};
}