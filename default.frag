#version 330 core
out vec4 FragColor0;
out vec4 FragColor1;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
   FragColor0 = texture(tex0, texCoord);
   FragColor1 = texture(tex1, texCoord);
}