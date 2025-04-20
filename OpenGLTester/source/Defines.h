#pragma once
#include <pch.h>
#include "Utilities/Public/ImG_debuger.h"

//macros

//uncomment one of two. Determines whether Renderer class is singlton or allows multiple instances.
//#define MULTIRENDER
#define SINGLERENDER

#define DEBUG_UI ImG_debuger::get()
#define M_SPTR std::make_shared

//using and typedefs
template<typename Ty>
using SPTR = std::shared_ptr<Ty>;

//enums
enum class ShaderType
{
    NONE = 0,
    SIMPLE = 1,
    TEXTURE_STD = 2,
    BATCH_RENDER = 3
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
    vec2() = default;
    float x = 0.f;
    float y = 0.f;
};

struct vec3
{
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
    vec3(glm::vec3& vec3) : x(vec3.x), y(vec3.y), z(vec3.z) {}
    vec3() = default;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

inline vec3 operator+(vec3 _first, vec3 _second)
{
    vec3 result;
    result.x = _first.x + _second.x;
    result.y = _first.y + _second.y;
    result.z = _first.z + _second.z;
    return result;
}

struct vec4
{
    vec4(float _x, float _y, float _z, float _a) : x(_x), y(_y), z(_z), a(_a){}
    vec4(const glm::vec3& vec3, float _a) : x(vec3.x), y(vec3.y), z(vec3.z), a(_a){}
    vec4(const glm::vec4& vec4) : x(vec4.x), y(vec4.y), z(vec4.z), a(vec4.a){}
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