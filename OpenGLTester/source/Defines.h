#pragma once
#include <pch.h>
#include "Utilities/Public/ImG_debuger.h"

//macros

//uncomment one of two. Determines whether Renderer class is singlton or allows multiple instances.
//#define MULTIRENDER
#define SINGLERENDER
#define MAX_RENDER_BATCH_SIZE 32 //need for std::array in MaterialInst. Limits maximum batch size. 

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

//todo: once BOOST PP added to proj, use it and MAX_RENDER_BATCH_SIZE to aseemble this enum
enum class TextureId
{
    TEX_0,
    TEX_1,
    TEX_2,
    TEX_3,
    TEX_4,
    TEX_5,
    TEX_6,
    TEX_7,
    TEX_8,
    TEX_9,
    TEX_10,
    TEX_11,
    TEX_12,
    TEX_13,
    TEX_14,
    TEX_15,
    TEX_16,
    TEX_17,
    TEX_18,
    TEX_19,
    TEX_20,
    TEX_21,
    TEX_22,
    TEX_23,
    TEX_24,
    TEX_25,
    TEX_26,
    TEX_27,
    TEX_28,
    TEX_29,
    TEX_30,
    TEX_31,
    TEX_32,
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
