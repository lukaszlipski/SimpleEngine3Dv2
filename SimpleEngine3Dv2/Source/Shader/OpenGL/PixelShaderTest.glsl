#version 440 core

out vec4 FragColor;

layout(std140) uniform PerObject {
	float TestA;
	float TestB;
};

layout(std140) uniform PerObject2 {
	float TestA2;
	float TestB2;
};

void main()
{
    FragColor = vec4(1.0f * TestA, 1.0f * TestA2, 0.0f, 1.0f);
}
