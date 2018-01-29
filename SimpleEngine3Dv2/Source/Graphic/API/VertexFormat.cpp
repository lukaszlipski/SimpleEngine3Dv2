#include "VertexFormat.h"

namespace SE3D2
{
	BEGIN_VERTEX_FORMAT(CommonVertex)
		VERTEX_MEMBER(CommonVertex, Position)
		VERTEX_MEMBER(CommonVertex, TexCoord)
	END_VERTEX_FORMAT(CommonVertex)
}