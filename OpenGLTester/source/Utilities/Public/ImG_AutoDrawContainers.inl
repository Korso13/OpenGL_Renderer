#pragma once
//File with AutoDraw specializations for various containers

//Vectors
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
        for (SPTR<T>& obj : *_object)
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
        for (UPTR<T>& obj : *_object)
        {
            result = result || AutoDraw<T>(std::to_string(index++), obj);
        }
        ImGui::TreePop();
    }
    return result;
}
template<typename T>
bool AutoDraw(const std::string& _name, std::vector<WPTR<T>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (WPTR<T>& obj : *_object)
        {
            result = result || AutoDraw<T>(std::to_string(index++), obj);
        }
        ImGui::TreePop();
    }
    return result;
}


//Unordered maps
template<typename K, typename V>
bool AutoDraw(const std::string& _name, std::unordered_map<K,V>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (auto& keyValuePair : *_object)
        {
            const auto& key = keyValuePair.first;
            auto& value = keyValuePair.second;
            if (ImGui::TreeNode(("Element " + STR(index)).c_str()))
            {
                result = result || AutoDraw<K>("<-Key", &key);
                result = result || AutoDraw<V>("Value:", &value);
                
                ImGui::TreePop();
            }
            index++;
        }
        ImGui::TreePop();
    }
    
    return result;
}

template<typename K, typename V>
bool AutoDraw(const std::string& _name, std::unordered_map<K,SPTR<V>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (auto& keyValuePair : *_object)
        {
            const auto& key = keyValuePair.first;
            auto& value = keyValuePair.second;
            if (ImGui::TreeNode(("Element " + STR(index)).c_str()))
            {
                result = result || AutoDraw<K>("<-Key", &key);
                result = result || AutoDraw<V>("Value:", value);
                
                ImGui::TreePop();
            }
            index++;
        }
        ImGui::TreePop();
    }
    
    return result;
}

template<typename K, typename V>
bool AutoDraw(const std::string& _name, std::unordered_map<K,UPTR<V>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (auto& keyValuePair : *_object)
        {
            const auto& key = keyValuePair.first;
            auto& value = keyValuePair.second;
            if (ImGui::TreeNode(("Element " + STR(index)).c_str()))
            {
                result = result || AutoDraw<K>("<-Key", &key);
                result = result || AutoDraw<V>("Value:", value);
                
                ImGui::TreePop();
            }
            index++;
        }
        ImGui::TreePop();
    }
    
    return result;
}

template<typename K, typename V>
bool AutoDraw(const std::string& _name, std::unordered_map<K,WPTR<V>>* _object)
{
    OBJ_CHECK
    bool result = false;
    int index = 0;
    if (ImGui::TreeNode(_name.c_str()))
    {
        for (auto& keyValuePair : *_object)
        {
            const K& key = keyValuePair.first;
            auto& value = keyValuePair.second;
            if (ImGui::TreeNode(("Element " + STR(index)).c_str()))
            {
                result = result || AutoDraw<K>("<-Key", &key);
                result = result || AutoDraw<V>("Value:", value);
                
                ImGui::TreePop();
            }
            index++;
        }
        ImGui::TreePop();
    }
    
    return result;
}


//todo: add handlers for sets, maps, their unordered variants