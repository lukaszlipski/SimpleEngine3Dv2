#version 440 core

out vec4 FragColor;

layout(std140) uniform PerObject2 {
	float Float;
	float TestB3;
};

void main()
{
    FragColor = vec4(1.0f * Float, 0.0f, 0.0f, 1.0f);
}
