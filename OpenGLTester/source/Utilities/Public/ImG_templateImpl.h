#define ADDDEBUGVALUE_IMPL \
        if(!folderExists(_folderName))\
            addFolder(std::string(_folderName));\
        auto FolderIt = m_imguiFolders.find(_folderName);\
        switch (_type){\
        case DebugDataType::FLOAT:\
            FolderIt->second.floats.emplace(_debugValueCaption, elementData<float>((float*)_valueRef, _valuesCount, _editToolType, _rangeMin, _rangeMax));\
            break;\
        case DebugDataType::COLOR:\
            FolderIt->second.colors.emplace(_debugValueCaption, elementData<float>((float*)_valueRef, _valuesCount, _editToolType, _rangeMin, _rangeMax));\
            break;\
        case DebugDataType::INT:\
            FolderIt->second.ints.emplace(_debugValueCaption, elementData<int>((int*)_valueRef, _valuesCount, _editToolType, _rangeMin, _rangeMax));\
            break;\
        case DebugDataType::BOOL:\
            FolderIt->second.bools.emplace(_debugValueCaption, elementData<bool>((bool*)_valueRef, _valuesCount, _editToolType, _rangeMin, _rangeMax));\
            break;\
        default: ;\
        }\
        size_t renderIndex = (FolderIt)->second.renderOrder.size();\
        FolderIt->second.renderOrder.emplace(renderIndex, std::pair(_type, _debugValueCaption));\
        