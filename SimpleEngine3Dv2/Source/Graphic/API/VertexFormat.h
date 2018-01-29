#pragma once
#include "..\Utility\Types.h"
#include "..\Math\Vector3.h"
#include "..\Math\Vector2.h"
#include <stddef.h>

#define DECLARE_VERTEX_FORMAT(name) static VertexFormatDec<name> VertexFormatInfo;
#define BEGIN_VERTEX_FORMAT(name) VertexAttribute VertexFormatDec<name>::Members[] = { 
#define VERTEX_MEMBER(name,member) {#member,sizeof(name::member),offsetof(name,member)},
#define END_VERTEX_FORMAT(name) }; \
	int VertexFormatDec<name>::VertexAttributesCount = sizeof(VertexFormatDec<name>::Members) / sizeof(VertexAttribute);

namespace SE3D2
{
	struct VertexAttribute
	{
		const char* mName;
		int mSize;
		int mOffset;
	};

	template<typename T>
	struct VertexFormatDec
	{
		static VertexAttribute Members[];
		static int VertexAttributesCount;
		const static int Size = sizeof(T);
	};

	struct CommonVertex
	{
		Vector3 Position;
		Vector2 TexCoord;
		DECLARE_VERTEX_FORMAT(CommonVertex)
	};

}
