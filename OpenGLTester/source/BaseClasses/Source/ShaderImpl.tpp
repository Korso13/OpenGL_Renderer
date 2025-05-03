//template methods implementations for Shader header
#pragma once

template<>
inline bool Shader::hasUniformParam<int>(const std::string& _uniformName) const
{
    return m_uniforms.iUniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<bool>(const std::string& _uniformName) const
{
    return m_uniforms.iUniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<int*>(const std::string& _uniformName) const
{
    return m_uniforms.ivUniforms.contains(_uniformName);
}

using intVectorParam = std::array<int, MAX_RENDER_BATCH_SIZE>;
template<>
inline bool Shader::hasUniformParam<intVectorParam>(const std::string& _uniformName) const
{
    return m_uniforms.ivUniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<float>(const std::string& _uniformName) const
{
    return m_uniforms.fUniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<vec2>(const std::string& _uniformName) const
{
    return m_uniforms.v2Uniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<vec3>(const std::string& _uniformName) const
{
    return m_uniforms.v3Uniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<vec4>(const std::string& _uniformName) const
{
    return m_uniforms.v4Uniforms.contains(_uniformName);
}

template<>
inline bool Shader::hasUniformParam<glm::mat4>(const std::string& _uniformName) const
{
    return m_uniforms.m4Uniforms.contains(_uniformName);
}