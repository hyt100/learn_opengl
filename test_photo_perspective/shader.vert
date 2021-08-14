
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 aMvp;

out vec2 TexCoord;

void main()
{
  vec4 posi = aMvp * vec4(aPos, 1.0);
  gl_Position = vec4(posi.xyz, 1.0);
  TexCoord = aTexCoord;
}
