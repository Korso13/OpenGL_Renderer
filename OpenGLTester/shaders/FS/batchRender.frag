#version 450 core
layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D u_Textures[10];

//float getSamplerSelectorColorMod(in int texID)
//{
//    return floor(pow(0.00001, mod(v_TexID, texID))); //produces 1 for texID matching v_TexID and 0 otherwise
//}

void main()
{
    vec4 texColor;
    
    if(v_TexIndex < 0.0)
    {
        o_Color =  v_Color;
        return;
    }
    
    int index = int(v_TexIndex);
    texColor = texture(u_Textures[index], v_TexCoord);

    o_Color = texColor*v_Color;
};