#pragma once
#include <string>
#include "Types.h"

namespace SE3D2
{
	struct Path
	{
		inline static std::string GetExtension(const std::string& path)
		{
			size_t index = path.find_last_of('.');
			if (index == std::string::npos || index == 0)
			{
				return "";
			}
			return path.substr(index + 1);
		}
	};

}
