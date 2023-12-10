#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextureCoord;

uniform sampler2D u_Texture;

void main()
{
    vec4 textureColor = texture(u_Texture, v_TextureCoord);
    color = textureColor;
    color = vec4(1.0);
}
