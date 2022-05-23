#version 330 core
out vec4 FragColor;
in vec2 text;
uniform sampler2D textura1;
void main()
{
    FragColor = texture(textura1, text);
}
