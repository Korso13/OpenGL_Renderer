#pragma once
//File with AutoDraw specializations for various containers

template<typename T>
bool AutoDraw(const std::string& _name, std::vector<T>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (auto& obj : *_object)
        {
            result = result || AutoDraw<T>(std::to_string(index++), &obj);
        }
        ImGui::TreePop();
    }
    
    return result;
}
template<typename T>
bool AutoDraw(const std::string& _name, std::vector<SPTR<T>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (SPTR<T> obj : *_object)
        {
            std::string elemName = std::to_string(index++);
            result = result || AutoDraw<T>(elemName, obj);
        }
        ImGui::TreePop();
    }
    return result;
}
template<typename T>
bool AutoDraw(const std::string& _name, std::vector<UPTR<T>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (UPTR<T> obj : *_object)
        {
            result = result || AutoDraw<T>(std::to_string(index++), obj);
        }
        ImGui::TreePop();
    }
    return result;
}