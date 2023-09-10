#pragma once
#include <PCH.h>

#include "ImG_templateImpl.h"

enum class DebugDataType
{
    FLOAT,
    COLOR,
    INT,
    BOOL
};

enum class ImGUI_ToolType
{
    SLIDER,
    INPUT,
    CHECKBOX,
    COLORPICKER
};

template<typename T>
struct elementData
{
    elementData(T* _inValsPtr, const size_t _valuesCount, ImGUI_ToolType _inType, const T _rangeMin = 0, const T _rangeMax = 0)
    :
    values(_inValsPtr),
    valuesCount(_valuesCount),
    toolType(_inType),
    rangeMin(_rangeMin),
    rangeMax(_rangeMax)
    {}
    elementData() = default;
    
    T* values = nullptr;
    size_t valuesCount = 0;
    ImGUI_ToolType toolType;
    T rangeMin = 0;
    T rangeMax = 0;
};

struct ImGUI_folder
{
    std::unordered_map<std::string, elementData<float>> floats;
    std::unordered_map<std::string, elementData<float>> colors;
    std::unordered_map<std::string, elementData<int>>   ints;
    std::unordered_map<std::string, elementData<bool>>  bools;

    std::map<size_t, std::pair<DebugDataType, std::string>> renderOrder;
};

struct ImGuiIO;

class ImG_debuger
{
public:

    static ImG_debuger* get()
    {
        if(!m_instance)
        {
            m_instance = new ImG_debuger();
        }
        return m_instance;
    }

    void initImGUI(GLFWwindow* _window);
    void imGUI_Render();

    bool folderExists(const std::string& _folderName);
    void addFolder(std::string&& _folderName);
    
    template<typename T>
    void addDebugValueToFolder(
        const std::string& _folderName,
        DebugDataType _type,
        std::string _debugValueCaption,
        T* _valueRef,
        const size_t _valuesCount,
        ImGUI_ToolType _editToolType,
        const T _rangeMin = 0,
        const T _rangeMax = 0
        )
    {
        ADDDEBUGVALUE_IMPL
    }
    
private:
    
    ImG_debuger();

    void generateImGUIContent();
    void handleFloatElement(const std::string& _folder, const std::string& _name);
    void handleIntElement(const std::string& _folder, const std::string& _name);
    void handleColorElement(const std::string& _folder, const std::string& _name);
    void handleBoolElement(const std::string& _folder, const std::string& _name);

private:
    
    static ImG_debuger* m_instance;
    
    ImGuiIO& io;
    bool m_isInitialized = false;
    std::unordered_map<std::string, ImGUI_folder> m_imguiFolders;
};
