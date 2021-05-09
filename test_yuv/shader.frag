#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D dataY;
uniform sampler2D dataU;
uniform sampler2D dataV;

void main()
{
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture(dataY, TexCoord).r - 0.0625;
    yuv.y = texture(dataU, TexCoord).r - 0.5;
    yuv.z = texture(dataV, TexCoord).r - 0.5;
    rgb = mat3( 1, 1, 1,  
                0, -0.39465, 2.03211,  
                1.13983, -0.58060, 0) * yuv;

    FragColor = vec4(rgb, 1); 
}