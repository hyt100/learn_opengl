
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 aMvp;

out vec3 textureDir;

void main()
{
  gl_Position = aMvp * vec4(aPos, 1.0);
  textureDir = aPos;
}
