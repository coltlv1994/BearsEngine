#version 330 core
out vec4 FragColor;

in vec3 fc;

void main()
{
    FragColor = vec4(fc, 1.0);
} 