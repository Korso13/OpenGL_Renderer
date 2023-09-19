#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in int textureID;

out vec2 v_TexCoord;
out vec4 v_Color;
out uint v_TextureID;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Color = inColor;
    v_TextureID = textureID;
};