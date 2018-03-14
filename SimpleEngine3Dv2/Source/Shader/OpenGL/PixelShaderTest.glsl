#version 440 core

out vec4 FragColor;

layout(std140) uniform PerObject2 {
	float Float;
	float TestB3;
};

layout( std140 ) buffer test
{
  float Test[ ];
};

void main()
{
    FragColor = vec4(1.0f * Float, 1.0f * Test[1], 0.0f, 1.0f);
}
