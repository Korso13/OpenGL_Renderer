#version 460 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 UV;
layout(location = 3) in uint TextureID;
layout(location = 4) in uint VertexIndex;

out vec2 v_TexCoord;
out uint v_TexID;
out vec4 v_color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = UV;
    v_color = Color;
    v_TexID = TextureID;
};