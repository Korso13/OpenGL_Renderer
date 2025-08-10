#pragma once
//File with AutoDraw specializations for various Engine types that are more complicated than basic data structs

//Template specialization for classes and structs with bool onGui(const std::string&)
template<typename T>
requires(onGuiDefined<T> || std::is_base_of_v<Object, T> || std::is_base_of_v<EngineInternal, T>)
bool AutoDraw(const std::string& _name, T* _object)
{
    OBJ_CHECK
    return _object->onGui(_name);
}

template<>
inline bool AutoDraw(const std::string& _name, Vertex* _object)
{
    OBJ_CHECK
    bool result = false;
    if (ImGui::TreeNode(_name.c_str()))
    {
        result = result || AutoDraw("Vertex local coord", &_object->vCoord);
        result = result || AutoDraw("Vertex world coord", &_object->position);
        result = result || AutoDraw("Vertex color", &_object->color);
        result = result || AutoDraw("Vertex uv", &_object->uv);
        result = result || AutoDraw("Vertex textureId", &_object->textureId);
        result = result || AutoDraw("Vertex index", &_object->vertexIndex);
        ImGui::TreePop();
    }

    return result;
}

