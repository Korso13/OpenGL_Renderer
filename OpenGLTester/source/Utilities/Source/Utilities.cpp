#include "../Public/Utilities.h"

#include "Utilities/Public/ImG_AutoDrawers.h"

uint32_t utils::Utilities::getInternalUID()
{
    return m_nextUID++;
}

Object::Object()
    : m_id(utils::Utilities::getInternalUID())
{
    m_name = "Object_" + std::to_string(m_id);
}

Object::Object(std::string _name)
    :
    m_name(std::move(_name)),
    m_id(utils::Utilities::getInternalUID())
{
    
}

void Object::setName(std::string _newName)
{
    if(_newName == m_name || _newName.empty())
        return;
    
    const SubscriberEventPayload evt_payload = SubscriberEventPayload{.stringPayload = m_name};
    m_name = std::move(_newName);
    triggerEvent("onNameChange", evt_payload);
}

bool Object::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("Object["+getUniqueName()+"]",
        NamedVar<const uint32_t*>{"Unique id", &m_id},
        NamedVar<std::string*>{"Name", &m_name}
    );
    return result;
}

EngineInternal::EngineInternal(std::string _name)
    :
    m_id(utils::Utilities::getInternalUID()), m_name(std::move(_name))
{

}

bool EngineInternal::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("EngineInternal["+getUniqueName()+"]",
        NamedVar<const uint32_t*>{"Unique id", &m_id},
        NamedVar<std::string*>{"Name", &m_name}
    );
    return result;
}
