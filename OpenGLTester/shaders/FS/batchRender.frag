#version 460 core
in vec2 v_TexCoord;
in uint v_TexID;
in vec4 v_color;

out vec4 o_color;

//uniform sampler2DArray u_Textures;
uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

float getSamplerSelectorColorMod(in int texID)
{
    return floor(pow(0.00001, mod(v_TexID, texID))); //produces 1 for texID matching v_TexID and 0 otherwise
}

void main()
{
    vec4 texColor = texture(u_Texture0, v_TexCoord);
    //texColor = texture(u_Textures, vec3(v_TexCoord, float(v_TexID)));

    //texColor = texture(u_Texture0, v_TexCoord)*getSamplerSelectorColorMod(0);
    //texColor += texture(u_Texture1, v_TexCoord)*getSamplerSelectorColorMod(1);
    
    o_color = texColor;
};