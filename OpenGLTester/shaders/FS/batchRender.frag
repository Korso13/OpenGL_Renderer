#version 430 core
out vec4 color;
in vec2 v_TexCoord;
in float v_TexID;
uniform sampler2D u_Texture[2];

void main()
{
    vec4 texColor;
    texColor = texture(u_Texture[int(v_TexID)], v_TexCoord);
    color = texColor;
};