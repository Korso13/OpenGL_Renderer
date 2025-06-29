#version 450 core
layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
flat in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor;
    
    int index = int(v_TexIndex);
    
    texColor = texture(u_Textures[index], v_TexCoord);

    o_Color = texColor*v_Color;
};