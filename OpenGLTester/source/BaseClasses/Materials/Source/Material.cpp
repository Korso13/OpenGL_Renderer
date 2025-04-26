#include "../Public/Material.h"
#include "../Public/MaterialInstance.h"

SPTR<MaterialInstance> Material::createInstance()
{
    if(m_underlyingShaderType == ShaderType::NONE)
        return nullptr;

    auto newMatInst = SPTR<MaterialInstance>(new MaterialInstance(getName() + "_Instance", m_underlyingShaderType));
    return newMatInst;
}

ShaderType Material::getUnderlyingShaderType() const
{
    return m_underlyingShaderType;
}
