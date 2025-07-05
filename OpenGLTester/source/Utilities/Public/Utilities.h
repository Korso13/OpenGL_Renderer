#pragma once
#include "pch.h"
#include "Subscriber.h"
#include "Math.h"

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
    Object(const Object& other) = delete;
    explicit Object(std::string _name);
    ~Object() override = default;
    
    [[nodiscard]] const std::string& getName() const {return m_name;}
    //Changes set Object name. Fires onNameChange event containing old name
    void setName(std::string _newName); 
    [[nodiscard]] uint32_t getUID() const {return m_id;}

    //todo: add virtual protected method onGui for debug rendering + add ImGui renderer as friend
    
private:

    std::string m_name;
    const uint32_t m_id;
};

//Base class for engine entities that are not rendered on screen / not present in the level
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

//Helper struct for using std::visit with vector of variants
//Syntax to use: std::visit(Visitor{[](type1 var){/*type1_handler*/},[](type2 var){/*type2_handler*/}...}, std::variant<>);
template<typename ... Lambdas>
struct Visitor : Lambdas...
{
    Visitor(Lambdas... lambdas) : Lambdas(std::forward<Lambdas>(lambdas))...
    {
    }
    using Lambdas::operator()...;
};

static [[nodiscard]] SPTR<Vertex> CreateVertex(
    const vec3& _vertexLocalCoord,
    const vec2& _uv,
    const unsigned int& _vertexIndex,
    const vec3& _globalPosOffset, 
    const int32_t& _textureID = 0)
{
    SPTR<Vertex> new_vertex = std::make_shared<Vertex>(*(new Vertex));
    new_vertex->vCoord = _vertexLocalCoord;
    new_vertex->position = _globalPosOffset + new_vertex->vCoord;
    new_vertex->uv = _uv;
    new_vertex->textureId = static_cast<float>(_textureID);
    new_vertex->vertexIndex = _vertexIndex;

    return new_vertex;
}