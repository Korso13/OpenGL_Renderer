#version 330 core
in vec2 v_TexCoord;
in vec4 v_Color
in int v_TextureID;

out vec4 color;

uniform sampler2D[4] u_Texture;

void main()
{
    if(v_TextureID == -1)
    {
        color = v_Color;
    }
    else
    {
        vec4 texColor = texture(u_Texture[v_TextureID], v_TexCoord);
        color = texColor;
    }
};