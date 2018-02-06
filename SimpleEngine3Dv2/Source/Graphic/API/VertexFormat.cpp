#include "VertexFormat.h"

namespace SE3D2
{

	BEGIN_VERTEX_FORMAT(CommonVertex)
		VERTEX_MEMBER(CommonVertex, Vector3, Position)
		VERTEX_MEMBER(CommonVertex, Vector2, TexCoord)
	END_VERTEX_FORMAT(CommonVertex)

}