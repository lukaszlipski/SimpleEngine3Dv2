#pragma once
#include "..\Utility\Types.h"
#include "..\Math\Vector3.h"
#include "..\Math\Vector2.h"
#include <stddef.h>
#include <vector>

#define DECLARE_VERTEX_FORMAT(name) static VertexFormatDec VertexFormatInfo;
#define BEGIN_VERTEX_FORMAT(name) VertexFormatDec name::VertexFormatInfo = { #name, {
#define VERTEX_MEMBER(name, type, member) {#member, #type ,sizeof(name::member),offsetof(name,member)},
#define END_VERTEX_FORMAT(name) }, sizeof(CommonVertex)}; 

namespace SE3D2
{

	struct VertexAttribute
	{
		const char* Name;
		const char* Type;
		uint32 Size;
		uint32 Offset;
	};

	struct VertexFormatDec
	{
		const char* Name;
		std::vector<VertexAttribute> Members;
		int Size;
	};
	
	struct CommonVertex
	{
		Vector3 Position;
		Vector2 TexCoord;
		DECLARE_VERTEX_FORMAT(CommonVertex)
	};

	class VertexFormat
	{
	public:
		VertexFormat(const VertexFormatDec& vertexFormatDesc) 
			: mVertexFormatDescription(vertexFormatDesc)
		{}

		virtual ~VertexFormat() = default;

		virtual void Bind() = 0;
		virtual bool Create(class Shader* shader) = 0;

		inline std::string GetName() { return mVertexFormatDescription.Name; }

	protected:
		VertexFormatDec mVertexFormatDescription;

	};

}
