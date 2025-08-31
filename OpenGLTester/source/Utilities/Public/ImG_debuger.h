#pragma once
#include <pch.h>

#include "ImGUI/imgui.h"

class ImGuiUtils;

//todo: add support for vector types
enum class DebugDataType
{
    FLOAT,
    COLOR,
    INT,
    BOOL,
    BUTTON
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
    elementData(T* _inValsPtr, const size_t _valuesCount, ImGUI_ToolType _inType, const T&& _rangeMin = 0, const T&& _rangeMax = 0)
    :
    values(_inValsPtr),
    valuesCount(_valuesCount),
    toolType(_inType),
    rangeMin(std::move(_rangeMin)),
    rangeMax(std::move(_rangeMax))
    {}
    elementData() = default;
    
    T* values = nullptr;
    size_t valuesCount = 0;
    ImGUI_ToolType toolType;
    T rangeMin = T();
    T rangeMax = T();
};

struct ImGUI_folder
{
    std::unordered_map<std::string, elementData<float>> floats;
    std::unordered_map<std::string, elementData<float>> colors;
    std::unordered_map<std::string, elementData<int>>   ints;
    std::unordered_map<std::string, elementData<bool>>  bools;
    std::unordered_map<std::string, std::function<void()>>  buttons;

    std::map<size_t, std::pair<DebugDataType, std::string>> renderOrder;
};

struct ImGuiIO;

class ImG_debuger
{
    friend ImGuiUtils;
public:

    static ImG_debuger* get()
    {
        if(!m_instance)
        {
            m_instance = new ImG_debuger();
        }
        return m_instance;
    }

    //todo: Make these two private and add EngineController as friend to call them, once it's implemented
    void initImGUI(GLFWwindow* _window);
    void imGUI_Render();

    bool folderExists(const std::string& _folderName) const;
    void addFolder(std::string&& _folderName);
    void addMainMenuItem(const std::string& _menuName, std::function<void()>&& _menuActivationCB);
    
    template<typename T>
    void addDebugValueToFolder(
        const std::string& _folderName,
        DebugDataType _type,
        std::string _debugValueCaption,
        T* _valueRef,
        const size_t _valuesCount = 1,
        ImGUI_ToolType _editToolType = ImGUI_ToolType::SLIDER,
        const T&& _rangeMin = T(),
        const T&& _rangeMax = T()
        );

    void addDebugButtonToFolder(const std::string& _folder, const std::string& _buttonName, std::function<void()>&& _btnActivationCB);
    
private:
    
    ImG_debuger();
    void loadFonts();
    void loadStyle();

    void generateImGUIContent();
    void drawHierarchyTab();
    void drawCameraTab();
    void drawRendererTab();
    void handleFloatElement(const std::string& _folder, const std::string& _name);
    void handleIntElement(const std::string& _folder, const std::string& _name);
    void handleColorElement(const std::string& _folder, const std::string& _name);
    void handleBoolElement(const std::string& _folder, const std::string& _name);
    void handleButtonElement(const std::string& _folder, const std::string& _name);
    
private:
    
    static ImG_debuger* m_instance;
    
    ImGuiIO& io;
    bool m_isInitialized = false;

    //Fonts...
    bool m_fontLoaded = false;
    ImVector<ImWchar> m_fontRanges; 
    ImFontConfig m_fontConfig;
    //
    std::string m_lastSelectedHierarchyItem = "";
    std::function<void()> m_scheduleBetweenFrames = nullptr;
    std::unordered_map<std::string, ImGUI_folder> m_imguiFolders;
};

//template methods impls
#include "ImG_templateImpl.inl"