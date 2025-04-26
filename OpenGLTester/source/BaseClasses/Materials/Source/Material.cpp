#include "../Public/Material.h"

const std::string& Material::getName() const
{
    return m_name;
}

ShaderType Material::getUnderlyingShaderType() const
{
    return m_underlyingShaderType;
}