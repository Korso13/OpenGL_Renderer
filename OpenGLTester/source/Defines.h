#pragma once
#include <PCH.h>
#include "Utilities/ImG_debuger.h"

//macros

//uncomment one of two. Determines whether Renderer class is singlton or allows multiple instances.
#define MULTIRENDER
//#define SINGLERENDER
#define DEBUG_UI ImG_debuger::get()

//using and typedefs
template<typename Ty>
using SPTR = std::shared_ptr<Ty>;

//enums
enum class ShaderType
{
    NONE = 0,
    SIMPLE = 1,
    TEXTURE_STD = 2
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
    vec3() = default;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct vec4
{
    vec4(float _x, float _y, float _z, float _a) : x(_x), y(_y), z(_z), a(_a){}
    vec4() = default;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float a = 0.f;
};

struct Uniforms
{
    std::unordered_map<std::string, GLint> iUniforms;
    std::unordered_map<std::string, GLint> fUniforms;
    std::unordered_map<std::string, GLint> v2Uniforms;
    std::unordered_map<std::string, GLint> v3Uniforms;
    std::unordered_map<std::string, GLint> v4Uniforms;
    std::unordered_map<std::string, GLint> m4Uniforms;
};

