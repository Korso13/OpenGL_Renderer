#pragma once
#include <pch.h>
#include "Utilities/Public/ImG_debuger.h"

//macros

//uncomment one of two. Determines whether Renderer class is singleton or allows multiple instances.
//#define MULTIRENDER
#define SINGLERENDER
#define MAX_RENDER_BATCH_SIZE 32 //need for std::array in MaterialInst. Limits maximum batch size. 
#define RESERVE_SAME_LEVEL_BATCHES 30 //when a new std::vector of batches is added in Renderer, reserve memory for this many batches
#define USE_BATCH_RENDERER_PIPELINE
#define START_RESOLUTION 1280, 980

//Shortcuts
#ifdef SINGLERENDER
    #define RENDERER Renderer::get()
#endif
#define RES_LIB ResourceLibrary::get()
#define RL_TEXTURE ResourceLibrary::get().getTexture
#define MAT_LIB MaterialLibrary::get()
#define MESH_FACTORY MeshesFactory::get()
#define DEBUG_UI ImG_debuger::get()
#define M_SPTR std::make_shared
#define M_UPTR std::make_unique
#define CAST_SPTR std::dynamic_pointer_cast
#define SCAST_SPTR std::static_pointer_cast

#define STR(object) std::to_string(object)
#define CSTR(object) std::to_string(object).c_str()

#define CAST_I static_cast<int>
#define CAST_SZ static_cast<size_t>
#define CAST_F static_cast<float>

//using and typedefs
template<typename Ty>
using SPTR = std::shared_ptr<Ty>;
template<typename Ty>
using UPTR = std::unique_ptr<Ty>;
template<typename Ty>
using WPTR = std::weak_ptr<Ty>;

//enums
enum class ShaderType
{
    NONE = 0,
    SIMPLE = 1,
    TEXTURE_STD = 2,
    BATCH_RENDER = 3,
    SIZE
};

//todo: once BOOST PP added to proj, use it and MAX_RENDER_BATCH_SIZE to assemble this enum
enum class TextureId
{
    TEX_0 = 0,
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
    explicit ivec2() : x(0), y(0) {}
    explicit ivec2(const int32_t& _element1, const int32_t& _element2) : x(_element2), y(_element1){}
    explicit ivec2(const int32_t& _oneSize) : x(_oneSize), y(_oneSize){}
    
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

struct uvec2
{
    explicit uvec2() : x(0), y(0) {}
    explicit uvec2(const unsigned int& _element1, const unsigned int& _element2) : x(_element2), y(_element1){}
    explicit uvec2(const unsigned int& _oneSize) : x(_oneSize), y(_oneSize){}
    explicit uvec2(const ivec2& _ivecInit) : x(_ivecInit.x), y(_ivecInit.y){}
    
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
    vec2& operator= (const vec2& _vec2) { x = _vec2.x; y = _vec2.y; return *this;}
    vec2() = default;
    float x = 0.f;
    float y = 0.f;
};

struct vec3
{
    vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z){}
    vec3(const float _oneSize) : x(_oneSize), y(_oneSize), z(_oneSize){}
    explicit vec3(const glm::vec3& _vec3) : x(_vec3.x), y(_vec3.y), z(_vec3.z) {}
    vec3& operator=(const vec3& _vec3) { x = _vec3.x; y = _vec3.y; z = _vec3.z; return *this;}
    vec3() = default;

    [[nodiscard]] glm::vec3 glmFormat() const {return {x, y, z};}
    
public:
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

struct vec4
{
    vec4(const float _x, const float _y, const float _z, const float _a) : x(_x), y(_y), z(_z), a(_a){}
    explicit vec4(const glm::vec3& _vec3, const float _a) : x(_vec3.x), y(_vec3.y), z(_vec3.z), a(_a){}
    explicit vec4(const glm::vec4& _vec4) : x(_vec4.x), y(_vec4.y), z(_vec4.z), a(_vec4.a){}
    vec4& operator= (const vec4& _vec4) { x = _vec4.x; y = _vec4.y; z = _vec4.z; a = _vec4.a; return *this;}
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
    std::unordered_map<std::string, std::pair<GLint, int32_t>> ivUniforms;
};

//NOTE: update VertexAO::setVertexAttributesTyped<Vertex>() after changing fields of Vertex!!
struct Vertex
{
    vec3 vCoord = vec3(0.f); //vertex local coord
    vec3 position = vec3(0.f); //Vertex finite coord
    vec4 color = {1.f, 1.f, 1.f, 1.f};
    vec2 uv = vec2(0.f);
    float textureId = -1.f;
    unsigned int vertexIndex = 0;
};

class Node; //forward decl

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
    Transform operator/(Transform& _other) const;

    bool onGui(const std::string& _name);
    
private:

    vec3 m_translation = vec3(0.f);
    vec3 m_scale = vec3(1.f);
    //todo: add rotation, once you figure out and write quaternions...
};
