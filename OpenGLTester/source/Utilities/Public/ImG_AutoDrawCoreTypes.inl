#pragma once
//File with AutoDraw specializations for trivial and very basic types

template<>
inline bool AutoDraw(const std::string& _name, bool* _object)
{
    OBJ_CHECK
    return ImGui::Checkbox(_name.c_str(), _object);
}

template<typename T>
requires std::signed_integral<T>
bool AutoDraw(const std::string& _name, T* _object)
{
    OBJ_CHECK
    return ImGui::DragInt(_name.c_str(), _object, INT_SLIDER_SPEED, MIN_SIGNED_INT_SLIDER_VALUE, MAX_SIGNED_INT_SLIDER_VALUE, "%d");
}

template<typename T>
requires std::signed_integral<T>
bool AutoDraw(const std::string& _name, const T* _object)
{
    OBJ_CHECK
    int const_int_holder = *_object;
    return ImGui::InputInt(_name.c_str(), &const_int_holder, 1, 1, ImGuiInputTextFlags_ReadOnly);
}

template<typename T>
requires std::unsigned_integral<T>
bool AutoDraw(const std::string& _name, T* _object)
{
    OBJ_CHECK
    bool changed = false;
    int uint_holder = CAST_I(*_object); //ImGui has no overload for uints :(
    changed = changed || ImGui::DragInt(_name.c_str(), &uint_holder, INT_SLIDER_SPEED, MIN_UNSIGNED_INT_SLIDER_VALUE, MAX_UNSIGNED_INT_SLIDER_VALUE, "%d");
    if (changed)
        *_object = static_cast<T>(uint_holder);
    return changed;
}

template<typename T>
requires std::unsigned_integral<T>
bool AutoDraw(const std::string& _name, const T* _object)
{
    OBJ_CHECK
    int uint_holder = CAST_I(*_object); //ImGui has no overload for uints :(
    return ImGui::InputInt(_name.c_str(), &uint_holder, 1, 1, ImGuiInputTextFlags_ReadOnly);
}

template<typename T>
requires std::is_floating_point_v<T>
bool AutoDraw(const std::string& _name, T* _object)
{
    OBJ_CHECK
    return ImGui::DragFloat(_name.c_str(), _object, FLOAT_SLIDER_SPEED, MIN_FLOAT_SLIDER_VALUE, MAX_FLOAT_SLIDER_VALUE, FLOAT_PRECISION);
}

template<typename T>
requires std::is_floating_point_v<T>
bool AutoDraw(const std::string& _name, const T* _object)
{
    OBJ_CHECK
    float const_float_holder = *_object;
    return ImGui::InputFloat(_name.c_str(), &const_float_holder, 1.f, 1.f, FLOAT_PRECISION, ImGuiInputTextFlags_ReadOnly);
}

template<>
inline bool AutoDraw(const std::string& _name, std::string* _object)
{
    OBJ_CHECK
    return ImGui::InputText(_name.c_str(), _object->data(), _object->size());
}

template<>
inline bool AutoDraw(const std::string& _name, const std::string* _object)
{
    OBJ_CHECK
    ImGui::Text("%s", _name.c_str());
    return false;
}

//easy maker of pressable button if you need it!
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

//used for passing sub-calls to ImGui passed to AutoDrawers::DrawClassVariables
template<>
inline bool AutoDraw(const std::string& _name, std::function<bool(const std::string&)> _object)
{
    OBJ_CHECK
    return _object(_name);
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
inline bool AutoDraw(const std::string& _name, glm::vec4* _object)
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

//todo: make better enum values' representation once enum serialization is implemented
template<typename T>
requires std::is_enum_v<T>
bool AutoDraw(const std::string& _name, T* _object)
{
    OBJ_CHECK
    bool changed = false;
    int enum_holder = CAST_I(*_object);
    changed = changed || ImGui::InputInt(_name.c_str(), &enum_holder, 1, 1);
    if (changed)
        *_object = static_cast<T>(enum_holder);
    return changed; 
}