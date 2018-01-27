#pragma once
#include "..\Utility\Types.h"
#include "..\Math\Vector3.h"
#include "..\Math\Vector2.h"
#include <map>
#include <vector>

namespace SE3D2
{
	// COUNT needs to be always last value
	// When actualizing VertexAttributeType there is need to update mVertexAttributeMap
	enum class VertexAttributeType : int8
	{
		POSITION = 0, // Vector3
		TEXTURECOORDINATES, // Vector2
		COUNT
	};

	class VertexFormat
	{
	public:
		VertexFormat() 
		{	
			mVertexAttributeMap[VertexAttributeType::POSITION] = sizeof(Vector3);
			mVertexAttributeMap[VertexAttributeType::TEXTURECOORDINATES] = sizeof(Vector2);
		}

		virtual void Bind() = 0;
		virtual void ClearResources() = 0;

		static inline uint32 GetSizeForAttributeType(VertexAttributeType type) { return static_cast<uint32>(mVertexAttributeMap[type]); }

		inline void PushVertexAttribute(VertexAttributeType type) { mVertexAttributes.push_back(type); }
		inline VertexAttributeType GetVertexAttribute(int32 index) const { return mVertexAttributes.at(index); }
		inline int32 GetVertexAttributesSize() const { return static_cast<int32>(mVertexAttributes.size()); }

	protected:
		static std::map<VertexAttributeType, uint64> mVertexAttributeMap;
		std::vector<VertexAttributeType> mVertexAttributes;

	};

}
