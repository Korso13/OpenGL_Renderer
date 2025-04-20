#pragma once
#include <pch.h>
#include <Defines.h>

inline vec3 operator+(const vec3 _first, const vec3 _second)
{
    vec3 result;
    result.x = _first.x + _second.x;
    result.y = _first.y + _second.y;
    result.z = _first.z + _second.z;
    return result;
}

inline vec3 operator-(const vec3 _first, const vec3 _second)
{
    vec3 result;
    result.x = _first.x + _second.x;
    result.y = _first.y + _second.y;
    result.z = _first.z + _second.z;
    return result;
}

inline vec3 operator*(const vec3 _first, const vec3 _second)
{
    vec3 result;
    result.x = _first.x * _second.x;
    result.y = _first.y * _second.y;
    result.z = _first.z * _second.z;
    return result;
}

inline vec3 operator/(const vec3 _first, vec3 _second)
{
    vec3 result;
    if(_second.x == 0.f) _second.x = 0.00001f;
    if(_second.y == 0.f) _second.y = 0.00001f;
    if(_second.z == 0.f) _second.z = 0.00001f;
    result.x = _first.x / _second.x;
    result.y = _first.y / _second.y;
    result.z = _first.z / _second.z;
    return result;
}

inline Transform Transform::operator+(const Transform& _other) const
{
    Transform result;
    result.m_translation = m_translation + _other.m_translation;
    result.m_scale = m_scale + _other.m_scale;
    //todo: add rotation
    return result;
}

inline Transform Transform::operator-(const Transform& _other) const
{
    Transform result;
    result.m_translation = m_translation - _other.m_translation;
    result.m_scale = m_scale - _other.m_scale;
    //todo: add rotation
    return result;
}

inline Transform Transform::operator*(const Transform& _other) const
{
    Transform result;
    result.m_translation = m_translation * _other.m_translation;
    result.m_scale = m_scale * _other.m_scale;
    //todo: add rotation
    return result;
}

inline Transform Transform::operator/(const Transform& _other) const
{
    Transform result;
    result.m_translation = m_translation / _other.m_translation;
    result.m_scale = m_scale / _other.m_scale;
    //todo: add rotation
    return result;
}
