#pragma once

#include "Defines.h"
#include "pch.h"
#include <concepts>
#include "ImG_Defines.h"
#include "ImGUI/imgui_internal.h"

template<typename T>
concept onGuiDefined = requires(T _object, bool _result)
{
    _result = _object.onGui("Test");
};

template<typename T>
concept isSmartPointer = requires(T _object)
{
    _object.reset();
};

//used to simplify exposing variables to drawClassVariables() (shorter than std::pair<std::string, your_type_with_cv>)
template<typename Y>
using NamedVar = std::pair<std::string, Y>;
template<typename ContainerType>
using NamedContainer = std::pair<std::string, ContainerType>;
using SubMenu = std::pair<std::string, std::function<bool(const std::string&)>>;
#define SUB_MENU_CALL(contents) [&](const std::string& _name) -> bool {contents}

#define OBJ_CHECK if (!_object) {ImGui::Text("%s pointer invalid!", _name.c_str()); return false;}

namespace AutoDrawers
{
    
    //General templates
    template<typename T>
    bool AutoDraw(const std::string& _name, T* _object);
    template<typename T>
    bool AutoDraw(const std::string& _name, const T* _object);
    //rarely used, avoid it
    template<typename T>
    bool AutoDraw(const std::string& _name, T _object);
    // template<typename T>
    // bool AutoDraw(const std::string& _name, T&& _object);

#include "ImG_AutoDrawCoreTypes.inl"
#include "ImG_AutoDrawEngineTypes.inl"

    //NOTE: pointer unpackers must be after types, but before containers!

    //SPTR unpacker
    template<typename T>
    bool AutoDraw(const std::string& _name, SPTR<T>& _object)
    {
        OBJ_CHECK
        return AutoDraw<T>(_name, _object.get());
    }
    template<typename T>
    bool AutoDraw(const std::string& _name, const SPTR<T>& _object)
    {
        OBJ_CHECK
        return AutoDraw<T>(_name, _object.get());
    }
    //UPTR unpacker
    template<typename T>
    bool AutoDraw(const std::string& _name, UPTR<T>& _object)
    {
        OBJ_CHECK
        return AutoDraw<T>(_name, _object.get());
    }
    template<typename T>
    bool AutoDraw(const std::string& _name, const UPTR<T>& _object)
    {
        OBJ_CHECK
        return AutoDraw<T>(_name, _object.get());
    }
    //WPTR unpacker
    template<typename T>
    bool AutoDraw(const std::string& _name, WPTR<T>& _object)
    {
        if (_object.expired())
        {
            ImGui::Text("%s pointer invalid!", _name.c_str());
            return false;
        }
        return AutoDraw<T>(_name, _object.lock().get());
    }
    template<typename T>
    bool AutoDraw(const std::string& _name, const WPTR<T>& _object)
    {
        if (_object.expired())
        {
            ImGui::Text("%s pointer invalid!", _name.c_str());
            return false;
        }
        return AutoDraw<T>(_name, _object.lock().get());
    }

    //NOTE: Containers must go after all other AutoDraw handlers as they call them
#include "ImG_AutoDrawContainers.inl"
    
    //Draw packed variables of some class
    //Creates tree node with className and renders within all passed variables if it can
    //NOTE: Always place this function after AutoDraw specializations!!
    template <typename ... T>
    bool DrawClassVariables(const std::string& _className, NamedVar<T>... _namedVariablesPointers)
    {
        bool result = false;
   
        if (ImGui::TreeNode(_className.c_str()))
        {
            ((result =  result || AutoDraw(_namedVariablesPointers.first, _namedVariablesPointers.second)), ...);
            ImGui::TreePop();
        }

        return result;
    }
}

#include "ImG_AutoDrawExternalOnGui.inl"

#undef OBJ_CHECK