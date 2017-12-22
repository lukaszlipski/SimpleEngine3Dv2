#pragma once
#include "../Utility/Types.h"
#include "../Graphic/API/Context.h"

namespace SE3D2
{
	class Graphics
	{
	public:
		static Graphics& GetInstance()
		{
			static Graphics* instance = new Graphics();
			return *instance;
		}

		bool Startup(GraphicsAPI api, uint32 width, uint32 height);
		bool Shutdown();
		void Clear();
		void Update();

	private:
		Context* mContext;

	};
}