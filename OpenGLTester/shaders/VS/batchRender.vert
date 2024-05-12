#version 450 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 UV;
layout(location = 3) in int TextureID;
layout(location = 4) in uint VertexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec4 v_Color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = UV;
    v_Color = Color;
    v_TexIndex = float(TextureID);
};