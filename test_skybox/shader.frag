#version 330 core
out vec4 FragColor;
in vec3 textureDir;

uniform samplerCube cubeMap;

void main()
{
    FragColor = texture(cubeMap, textureDir); 
}