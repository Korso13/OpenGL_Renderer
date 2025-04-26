#include <utility>

#include "../Public/MaterialInstance.h"

MaterialInstance::MaterialInstance(std::string _name, ShaderType _shaderType)
    :
    EngineInternal(std::move(_name)),
    m_shader(_shaderType)
{
}
