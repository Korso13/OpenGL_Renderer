//file for out-of-header impls of onGui method, required for some classes
#pragma once
#include "Defines.h"
#include "ImG_AutoDrawers.h"

inline bool Transform::onGui(const std::string& _name)
{
    bool result = false;
    if (ImGui::TreeNode(_name.c_str()))
    {
        result = result || AutoDrawers::AutoDraw("Translation", &m_translation);
        result = result || AutoDrawers::AutoDraw("Scale", &m_scale);
        ImGui::TreePop();
    }

    return result;
}
