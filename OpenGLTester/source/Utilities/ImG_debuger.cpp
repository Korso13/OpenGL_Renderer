#include "ImG_debuger.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include "ImGUI/imgui_impl_opengl3.h"

ImG_debuger* ImG_debuger::m_instance = nullptr;

ImGuiIO& getIOContext()
{
    ImGui::CreateContext();
    return ImGui::GetIO();
}

ImG_debuger::ImG_debuger()
    : io(getIOContext())
{
    (void)io;
}

void ImG_debuger::initImGUI(GLFWwindow* _window)
{
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    m_isInitialized = true;
}

void ImG_debuger::generateImGUIContent()
{
    if(!m_isInitialized || m_imguiFolders.empty())
        return;

    for(auto& folder : m_imguiFolders)
    {
        //breaking code into folders
        if(ImGui::CollapsingHeader(folder.first.c_str()))
        {
            //sorting elements by their order of addition
            for(auto& [RO, Record] : folder.second.renderOrder)
            {
                //handling specific records
                switch (Record.first)
                {
                case DebugDataType::FLOAT:
                    handleFloatElement(folder.first, Record.second);
                    break;
                case DebugDataType::COLOR:
                    handleColorElement(folder.first, Record.second);
                    break;
                case DebugDataType::INT:
                    handleIntElement(folder.first, Record.second);
                    break;
                case DebugDataType::BOOL:
                    handleBoolElement(folder.first, Record.second);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void ImG_debuger::handleFloatElement(const std::string& _folder, const std::string& _name)
{
    if(!m_imguiFolders[_folder].floats[_name].values ||
        m_imguiFolders[_folder].floats[_name].valuesCount == 0)
            return;

    switch (m_imguiFolders[_folder].floats[_name].toolType)
    {
    case ImGUI_ToolType::SLIDER:
        switch (m_imguiFolders[_folder].floats[_name].valuesCount)
        {
    case(1):
        ImGui::SliderFloat(
            _name.c_str(),
            m_imguiFolders[_folder].floats[_name].values,
            m_imguiFolders[_folder].floats[_name].rangeMin,
            m_imguiFolders[_folder].floats[_name].rangeMax);
            break;
    case(2):
        ImGui::SliderFloat2(
            _name.c_str(),
            m_imguiFolders[_folder].floats[_name].values,
            m_imguiFolders[_folder].floats[_name].rangeMin,
            m_imguiFolders[_folder].floats[_name].rangeMax);
            break;
    case(3):
        ImGui::SliderFloat3(
            _name.c_str(),
            m_imguiFolders[_folder].floats[_name].values,
            m_imguiFolders[_folder].floats[_name].rangeMin,
            m_imguiFolders[_folder].floats[_name].rangeMax);
            break;
    case(4):
        ImGui::SliderFloat4(
            _name.c_str(),
            m_imguiFolders[_folder].floats[_name].values,
            m_imguiFolders[_folder].floats[_name].rangeMin,
            m_imguiFolders[_folder].floats[_name].rangeMax);
            break;
    default:
        break;
        }
        break;
    case ImGUI_ToolType::INPUT:
        //TODO:: add additional input field types support
        break;
    default:
        break;
    }
}

void ImG_debuger::handleIntElement(const std::string& _folder, const std::string& _name)
{
    if(!m_imguiFolders[_folder].ints[_name].values ||
        m_imguiFolders[_folder].ints[_name].valuesCount == 0)
            return;

    switch (m_imguiFolders[_folder].ints[_name].toolType)
    {
    case ImGUI_ToolType::SLIDER:
        switch (m_imguiFolders[_folder].ints[_name].valuesCount)
        {
        case(1):
            ImGui::SliderInt(
                _name.c_str(),
                m_imguiFolders[_folder].ints[_name].values,
                m_imguiFolders[_folder].ints[_name].rangeMin,
                m_imguiFolders[_folder].ints[_name].rangeMax);
            break;
        case(2):
            ImGui::SliderInt2(
                _name.c_str(),
                m_imguiFolders[_folder].ints[_name].values,
                m_imguiFolders[_folder].ints[_name].rangeMin,
                m_imguiFolders[_folder].ints[_name].rangeMax);
            break;
        case(3):
            ImGui::SliderInt3(
                _name.c_str(),
                m_imguiFolders[_folder].ints[_name].values,
                m_imguiFolders[_folder].ints[_name].rangeMin,
                m_imguiFolders[_folder].ints[_name].rangeMax);
            break;
        case(4):
            ImGui::SliderInt4(
                _name.c_str(),
                m_imguiFolders[_folder].ints[_name].values,
                m_imguiFolders[_folder].ints[_name].rangeMin,
                m_imguiFolders[_folder].ints[_name].rangeMax);
            break;
        default:
            break;
        }
        break;
    case ImGUI_ToolType::INPUT:
        //TODO:: add additional input field types support
        break;
    default:
        break;
    }
}

void ImG_debuger::handleColorElement(const std::string& _folder, const std::string& _name)
{
    if(!m_imguiFolders[_folder].colors[_name].values ||
        m_imguiFolders[_folder].colors[_name].valuesCount == 0)
            return;
    
    ImGui::ColorEdit3(_name.c_str(), m_imguiFolders[_folder].colors[_name].values);
}

void ImG_debuger::handleBoolElement(const std::string& _folder, const std::string& _name)
{
    if(!m_imguiFolders[_folder].bools[_name].values ||
        m_imguiFolders[_folder].bools[_name].valuesCount == 0)
            return;
    
    ImGui::Checkbox(_name.c_str(), m_imguiFolders[_folder].bools[_name].values);
}

void ImG_debuger::imGUI_Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGUI Debugger");
    generateImGUIContent();
    if(ImGui::CollapsingHeader("General info"))
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImG_debuger::folderExists(const std::string& _folderName)
{
    return m_imguiFolders.find(_folderName) != m_imguiFolders.end();
}

void ImG_debuger::addFolder(std::string&& _folderName)
{
    m_imguiFolders.emplace(_folderName, ImGUI_folder());
}