#pragma once
#include "pch.h"
#include "Subscriber.h"

namespace utils
{
    class Utilities
    {
    public:
        Utilities() = default;

        //provides unique IDs for the use by engine internal classes
        static uint32_t getInternalUID();

    private:
        inline static uint32_t m_nextUID = 0;
    };
}

//makes class non-copyable
#define MOVE_ONLY(name) \
    public:\
    name(const name& other) = delete;\
    void operator= (const name& other)= delete;

//Base class for all level-related objects (holds basic functionalities like naming, id, events)
//Events: onNameChange (string payload - old name);
class Object : public Subscriber
{
public:
    
    Object();
    explicit Object(std::string _name);
    ~Object() override = default;
    
    [[nodiscard]] const std::string& getName() const {return m_name;}
    //Changes set Object name. Fires onNameChange event containing old name
    void setName(std::string _newName); 
    [[nodiscard]] uint32_t getUID() const {return m_id;}
    
private:

    std::string m_name;
    const uint32_t m_id;
};

class EngineInternal
{
public:
    
    EngineInternal() = delete;
    explicit EngineInternal(std::string _name);
    virtual ~EngineInternal() = default;
    
    [[nodiscard]] const std::string& getName() const {return m_name;}
    [[nodiscard]] uint32_t getUID() const {return m_id;}
    
private:

    const uint32_t m_id;
    std::string m_name;
};