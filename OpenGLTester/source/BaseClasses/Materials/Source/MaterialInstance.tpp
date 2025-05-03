//template methods implementations for MaterialInstance header
#pragma once

template <UniformParam T>
void MaterialInstance::setUniformParam(const std::string& _paramName, T _param)
{
    ASSERT(ShaderMachine::get()->getShader(m_shaderType)->hasUniformParam<T>(_paramName));
    
    m_materialParams[_paramName] = _param;
}

template <UniformParam T>
T MaterialInstance::getUniformParam(const std::string& _paramName) const
{
    ASSERT(ShaderMachine::get()->getShader(m_shaderType)->hasUniformParam<T>(_paramName));
    ASSERT(m_materialParams.contains(_paramName));
    return std::get<T>(*m_materialParams.find(_paramName));
}

template <UniformParam T>
bool MaterialInstance::hasUniformParam(const std::string& _paramName) const
{
    return ShaderMachine::get()->getShader(m_shaderType)->hasUniformParam<T>(_paramName);
}