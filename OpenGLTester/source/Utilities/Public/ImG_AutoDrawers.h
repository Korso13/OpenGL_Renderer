#pragma once

#include "pch.h"
#include "Defines.h"
#include "ImG_Defines.h"

template<typename T>
concept onGuiDefined = requires(T _object, bool _result)
{
    _result = _object.onGui("Test");
};

//used to simplify exposing variables to drawClassVariables() (shorter than std::pair<std::string, your_type_with_cv>)
template<typename Y>
using NamedVar = std::pair<std::string, Y*>;

#define OBJ_CHECK if (!_object) {std::cerr << _name << " has invalid pointer!\n"; return false;}

namespace AutoDrawers
{
    
    //General template
    template<typename T>
    bool AutoDraw(const std::string& _name, T* _object);

    //Draw packed variables of some class
    //Creates tree node with className and renders within all passed variables if it can
    template <typename ... T>
    bool DrawClassVariables(const std::string& _className, NamedVar<T>... _namedVariablesPointers)
    {
        bool result = false;
   
        if (ImGui::TreeNode(_className.c_str()))
        {
            result = result || (AutoDraw(_namedVariablesPointers.first, _namedVariablesPointers.second), ...);
            ImGui::TreePop();
        }

        return result;
    }

    //Template specialization for classes and structs with bool onGui()
    template<typename T>
    requires(onGuiDefined<T>)
    bool AutoDraw(const std::string& _name, T* _object)
    {
        OBJ_CHECK
        return _object.onGui(_name);
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, bool* _object)
    {
        OBJ_CHECK
        return ImGui::Checkbox(_name.c_str(), _object);
    }
    
    template<typename T>
    requires std::signed_integral
    bool AutoDraw(const std::string& _name, T* _object)
    {
        OBJ_CHECK
        return ImGui::DragInt(_name.c_str(), _object, INT_SLIDER_SPEED, MIN_SIGNED_INT_SLIDER_VALUE, MAX_SIGNED_INT_SLIDER_VALUE, "%d");
    }

    template<typename T>
    requires std::unsigned_integral
    bool AutoDraw(const std::string& _name, T* _object)
    {
        OBJ_CHECK
        return ImGui::DragInt(_name.c_str(), _object, INT_SLIDER_SPEED, MIN_UNSIGNED_INT_SLIDER_VALUE, MAX_UNSIGNED_INT_SLIDER_VALUE, "%d");
    }
    
    template<typename T>
    requires std::floating_point
    bool AutoDraw(const std::string& _name, T* _object)
    {
        OBJ_CHECK
        return ImGui::DragFloat(_name.c_str(), _object, FLOAT_SLIDER_SPEED, MIN_FLOAT_SLIDER_VALUE, MAX_FLOAT_SLIDER_VALUE, FLOAT_PRECISION);
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, std::function<void()>* _object)
    {
        OBJ_CHECK
        if (ImGui::Button(_name.c_str()))
        {
            if (_object) (*_object)();
            return true;
        }
        return false;
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, ivec2* _object)
    {
        OBJ_CHECK
        bool changed = false;
        int v[2] = {_object->x, _object->y};
        changed = ImGui::DragInt2(_name.c_str(), v, INT_SLIDER_SPEED, MIN_SIGNED_INT_SLIDER_VALUE, MAX_SIGNED_INT_SLIDER_VALUE, "%d");
        if (changed)
        {
            _object->x = v[0];
            _object->y = v[1];
        }
        return changed; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, const ivec2* _object)
    {
        OBJ_CHECK
        OBJ_CHECK
        int v[2] = {_object->x, _object->y};
        ImGui::InputInt2(_name.c_str(), v, ImGuiInputTextFlags_ReadOnly);
        return false; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, uvec2* _object)
    {
        OBJ_CHECK
        bool changed = false;
        int v[2] = {CAST_I(_object->x), CAST_I(_object->y)};
        changed = ImGui::DragInt2(_name.c_str(), v, INT_SLIDER_SPEED, MIN_UNSIGNED_INT_SLIDER_VALUE, MAX_UNSIGNED_INT_SLIDER_VALUE, "%d");
        if (changed)
        {
            _object->x = static_cast<unsigned int>(std::max(0, v[0]));
            _object->y = static_cast<unsigned int>(std::max(0, v[1]));
        }
        return changed; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, const uvec2* _object)
    {
        OBJ_CHECK
        int v[2] = {CAST_I(_object->x), CAST_I(_object->y)};
        ImGui::InputInt2(_name.c_str(), v, ImGuiInputTextFlags_ReadOnly);
        return false; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, vec2* _object)
    {
        OBJ_CHECK
        bool changed = false;
        float v[2] = {_object->x, _object->y};
        changed = ImGui::DragFloat2(_name.c_str(), v, FLOAT_SLIDER_SPEED, MIN_FLOAT_SLIDER_VALUE, MAX_FLOAT_SLIDER_VALUE, FLOAT_PRECISION);
        if (changed)
        {
            _object->x = v[0];
            _object->y = v[1];
        }
        return changed; 
    }

    template<>
    inline bool AutoDraw(const std::string& _name, const vec2* _object)
    {
        OBJ_CHECK
        float v[2] = {_object->x, _object->y};
        ImGui::InputFloat2(_name.c_str(), v, FLOAT_PRECISION, ImGuiInputTextFlags_ReadOnly);
        return false; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, vec3* _object)
    {
        OBJ_CHECK
        bool changed = false;
        float v[3] = {_object->x, _object->y, _object->z};
        changed = ImGui::DragFloat3(_name.c_str(), v, FLOAT_SLIDER_SPEED, MIN_FLOAT_SLIDER_VALUE, MAX_FLOAT_SLIDER_VALUE, FLOAT_PRECISION);
        if (changed)
        {
            _object->x = v[0];
            _object->y = v[1];
            _object->z = v[2];
        }
        return changed; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, const vec3* _object)
    {
        OBJ_CHECK
        float v[3] = {_object->x, _object->y, _object->z};
        ImGui::InputFloat3(_name.c_str(), v, FLOAT_PRECISION, ImGuiInputTextFlags_ReadOnly);
        return false; 
    }
    
    //todo: make separate color struct that essentially inherits vec4 and has defined bool onGui(const std::string&) method for rendering color picker
    //switch to DragFloat4 after that
    template<>
    inline bool AutoDraw(const std::string& _name, vec4* _object)
    {
        OBJ_CHECK
        bool changed = false;
        float v[4] = {_object->x, _object->y, _object->z, _object->a};
        //changed = ImGui::DragFloat4(_name.c_str(), v, FLOAT_SLIDER_SPEED, MIN_FLOAT_SLIDER_VALUE, MAX_FLOAT_SLIDER_VALUE, FLOAT_PRECISION);
        changed = ImGui::ColorPicker4(_name.c_str(), v);
        if (changed)
        {
            _object->x = v[0];
            _object->y = v[1];
            _object->z = v[2];
            _object->a = v[3];
        }
        return changed; 
    }
    
    template<>
    inline bool AutoDraw(const std::string& _name, const vec4* _object)
    {
        OBJ_CHECK
        float v[4] = {_object->x, _object->y, _object->z, _object->a};
       ImGui::InputFloat4(_name.c_str(), v, FLOAT_PRECISION, ImGuiInputTextFlags_ReadOnly);
        return false; 
    }
    
}

#undef OBJ_CHECK