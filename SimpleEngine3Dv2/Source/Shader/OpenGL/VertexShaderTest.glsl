#version 440 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

layout(std140) uniform PerObject{
	float Float;
	float TestB3;
};

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
    gl_Position = vec4(aPosition + Float, 1.0f);
}
