#version 450 core
layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
flat in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D u_Textures[32];
uniform int u_debugTexIndex;

//float getSamplerSelectorColorMod(in int texID)
//{
//    return floor(pow(0.00001, mod(v_TexID, texID))); //produces 1 for texID matching v_TexID and 0 otherwise
//}

void main()
{
    vec4 texColor;
    
    int index = int(v_TexIndex);
    if(index == u_debugTexIndex)
    {
        o_Color =  v_Color;
        return;
    }
    
    texColor = texture(u_Textures[index], v_TexCoord);

    o_Color = texColor*v_Color;
};