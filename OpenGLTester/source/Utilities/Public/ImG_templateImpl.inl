#pragma once

template <>
inline void ImG_debuger::addDebugValueToFolder(const std::string& _folderName, DebugDataType _type,
                                               std::string _debugValueCaption, int32_t* _valueRef, const size_t _valuesCount, ImGUI_ToolType _editToolType,
                                               const int32_t&& _rangeMin, const int32_t&& _rangeMax)
{
    if(!folderExists(_folderName))
        addFolder(std::string(_folderName));
    auto FolderIt = m_imguiFolders.find(_folderName);
    FolderIt->second.ints.emplace(_debugValueCaption, elementData<int32_t>(_valueRef, _valuesCount, _editToolType, std::forward<const int32_t>(_rangeMin), std::forward<const int32_t>(_rangeMax)));
    
    size_t renderIndex = (FolderIt)->second.renderOrder.size();
    FolderIt->second.renderOrder.emplace(renderIndex, std::pair(_type, _debugValueCaption));
}

template <>
inline void ImG_debuger::addDebugValueToFolder(const std::string& _folderName, DebugDataType _type,
                                               std::string _debugValueCaption, bool* _valueRef, const size_t _valuesCount, ImGUI_ToolType _editToolType,
                                               const bool&& _rangeMin, const bool&& _rangeMax)
{
    if(!folderExists(_folderName))
        addFolder(std::string(_folderName));
    auto FolderIt = m_imguiFolders.find(_folderName);
    FolderIt->second.bools.emplace(_debugValueCaption, elementData<bool>(_valueRef, _valuesCount, _editToolType, false, true));
    
    size_t renderIndex = (FolderIt)->second.renderOrder.size();
    FolderIt->second.renderOrder.emplace(renderIndex, std::pair(_type, _debugValueCaption));
}

template <>
inline void ImG_debuger::addDebugValueToFolder(const std::string& _folderName, DebugDataType _type,
                                               std::string _debugValueCaption, float* _valueRef, const size_t _valuesCount, ImGUI_ToolType _editToolType,
                                               const float&& _rangeMin, const float&& _rangeMax)
{
    if(!folderExists(_folderName))
        addFolder(std::string(_folderName));
    auto FolderIt = m_imguiFolders.find(_folderName);
    if(_type == DebugDataType::FLOAT)
        FolderIt->second.floats.emplace(_debugValueCaption, elementData<float>(_valueRef, _valuesCount, _editToolType, std::forward<const float>(_rangeMin), std::forward<const float>(_rangeMax)));
    else if(_type == DebugDataType::COLOR)
        FolderIt->second.colors.emplace(_debugValueCaption, elementData<float>(_valueRef, _valuesCount, _editToolType, std::forward<const float>(_rangeMin), std::forward<const float>(_rangeMax)));
    
    size_t renderIndex = (FolderIt)->second.renderOrder.size();
    FolderIt->second.renderOrder.emplace(renderIndex, std::pair(_type, _debugValueCaption));
}