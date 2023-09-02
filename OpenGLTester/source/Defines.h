#pragma once
#include <PCH.h>

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
    std::unordered_map<std::string, int> iUniforms;
    std::unordered_map<std::string, int> fUniforms;
    std::unordered_map<std::string, int> v2Uniforms;
    std::unordered_map<std::string, int> v3Uniforms;
    std::unordered_map<std::string, int> v4Uniforms;
};

