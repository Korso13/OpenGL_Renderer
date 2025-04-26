#pragma once
#include <pch.h>
#include "Utilities/Public/ImG_debuger.h"

//macros

//uncomment one of two. Determines whether Renderer class is singlton or allows multiple instances.
//#define MULTIRENDER
#define SINGLERENDER

//Shortcuts
#define DEBUG_UI ImG_debuger::get()
#define M_SPTR std::make_shared
#define CAST_SPTR std::dynamic_pointer_cast
#define MAT_LIB MaterialLibrary::get()

#define CAST_I static_cast<int>
#define CAST_SZ static_cast<size_t>
#define CAST_F static_cast<float>

//using and typedefs
template<typename Ty>
using SPTR = std::shared_ptr<Ty>;

//enums
enum class ShaderType
{
    NONE = 0,
    SIMPLE = 1,
    TEXTURE_STD = 2,
    BATCH_RENDER = 3,
    SIZE
};

//structs
struct ivec2
{
    union 
    {
        int x;
        int w;
    };
    union
    {
        int y;
        int h;
    };
};

struct uivec2
{
    union 
    {
        unsigned int x;
        unsigned int w;
    };
    union
    {
        unsigned int y;
        unsigned int h;
    };
};

struct vec2
{
    vec2(float _x, float _y) : x(_x), y(_y){}
    vec2(const float _oneSize) : x(_oneSize), y(_oneSize){}
    vec2(const vec2& _vec2) : x(_vec2.x), y(_vec2.y) {}
    void operator= (const vec2& _vec2) { x = _vec2.x; y = _vec2.y;}
    vec2() = default;
    float x = 0.f;
    float y = 0.f;
};

struct vec3
{
    vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z){}
    vec3(const float _oneSize) : x(_oneSize), y(_oneSize), z(_oneSize){}
    explicit vec3(const glm::vec3& _vec3) : x(_vec3.x), y(_vec3.y), z(_vec3.z) {}
    void operator= (const vec3& _vec3) { x = _vec3.x; y = _vec3.y; z = _vec3.z;}
    vec3() = default;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct vec4
{
    vec4(const float _x, const float _y, const float _z, const float _a) : x(_x), y(_y), z(_z), a(_a){}
    explicit vec4(const glm::vec3& _vec3, const float _a) : x(_vec3.x), y(_vec3.y), z(_vec3.z), a(_a){}
    explicit vec4(const glm::vec4& _vec4) : x(_vec4.x), y(_vec4.y), z(_vec4.z), a(_vec4.a){}
    void operator= (const vec4& _vec4) { x = _vec4.x; y = _vec4.y; z = _vec4.z; a = _vec4.a;}
    vec4() = default;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float a = 0.f;
};

inline vec4 operator+(vec4 _first, vec4 _second)
{
    vec4 result;
    result.x = _first.x + _second.x;
    result.y = _first.y + _second.y;
    result.z = _first.z + _second.z;
    result.a = _first.a + _second.a;
    return result;
}

struct Uniforms
{
    std::unordered_map<std::string, GLint> iUniforms;
    std::unordered_map<std::string, GLint> fUniforms;
    std::unordered_map<std::string, GLint> v2Uniforms;
    std::unordered_map<std::string, GLint> v3Uniforms;
    std::unordered_map<std::string, GLint> v4Uniforms;
    std::unordered_map<std::string, GLint> m4Uniforms;
    std::unordered_map<std::string, std::pair<GLint, int>> ivUniforms;
};

struct Vertex
{
    vec3 Position;
    vec4 Color = {1.f, 1.f, 1.f, 1.f};
    vec2 UV;
    float TextureID = -1.f;
    unsigned int VertexIndex = 0;
};

class Node;
class Transform
{
    friend Node;
public:

    Transform() = default;
    [[nodiscard]] const vec3& getTranslation() const { return m_translation; }
    [[nodiscard]] const vec3& getScale() const { return m_scale; }
    
    Transform operator+(const Transform& _other) const;
    Transform operator-(const Transform& _other) const;
    Transform operator*(const Transform& _other) const;
    Transform operator/(const Transform& _other) const;

private:

    vec3 m_translation = vec3(0.f);
    vec3 m_scale = vec3(1.f);
    //todo: add rotation, once you figure out and write quaternions...
};
