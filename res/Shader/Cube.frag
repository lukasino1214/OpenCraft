#version 330 core
out vec4 FragColor;

in vec3 color;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(color, 1.0f);
}