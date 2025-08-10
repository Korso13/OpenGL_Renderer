#include "../Public/ImG_debuger.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>

#include "Defines.h"
#include "BaseClasses/Nodes/Public/Node.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ManagingClasses/Public/Renderer.h"
#include "Utilities/Public/ImG_AutoDrawers.h"
#include "Utilities/Public/ImG_Utils.h"

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
    addFolder("Main menu");
    m_scheduleBetweenFrames = [this]()
    {
        loadFonts();
        loadStyle();
    };
}

void ImG_debuger::loadFonts()
{
    if (m_fontLoaded) return;
    m_fontLoaded = true;
    
    ImFontGlyphRangesBuilder builder;
    builder.AddText("│├└┬─");   //used in hierarchy drawer
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
    builder.BuildRanges(&m_fontRanges);
    m_fontConfig.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("resources\\fonts\\arial.ttf", 16.0f, &m_fontConfig, m_fontRanges.Data);
    io.Fonts->Build();
}

void ImG_debuger::loadStyle()
{
    ImGui::StyleColorsClassic(); //will do for now
}

void ImG_debuger::addDebugButtonToFolder(const std::string& _folder, const std::string& _buttonName, std::function<void()>&& _btnActivationCB)
{
    if(!folderExists(_folder))
        addFolder(std::string(_folder));

    m_imguiFolders[_folder].buttons.emplace(_buttonName, std::move(_btnActivationCB));

    //registering in render index
    size_t renderIndex = m_imguiFolders[_folder].renderOrder.size();
    m_imguiFolders[_folder].renderOrder.emplace(renderIndex, std::pair(DebugDataType::BUTTON, _buttonName));
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
                case DebugDataType::BUTTON:
                    handleButtonElement(folder.first, Record.second);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void ImG_debuger::renderHierarchy()
{
    auto main_root = RENDERER->getRoot();
    if (main_root == nullptr) return;
    
    SPTR<Node> selected_object = nullptr;
    
    //rendering hierarchy with selectables
    size_t indent = 0;
    std::map<size_t, std::pair<size_t, size_t>> indent_item_counter; //indent level - current element in indent level - total elements in indent level
    indent_item_counter[0] = {0,0}; //root node is the only one on zero indent
    
    if (ImGui::BeginChild("Hierarchy", ImVec2(-1.f, 200.f), ImGuiChildFlags_Border | ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX))
    {
        static std::function<void(const SPTR<Node>&, const std::string&)> hierarchy_handler =
            [&selected_object, &indent, &indent_item_counter, this](const SPTR<Node>& _hierarchyObject, const std::string& _nameOverride = "")
            {
                std::string name = (_nameOverride.empty()) ?
                                    (_hierarchyObject->getUniqueName()) :
                                    _nameOverride;
                // std::cout << "========================\n";
                // std::cout << "Indent for " << name << "\n";
                // std::cout << "========================\n";
                const std::string indentation_prefix = ImGuiUtils::formHierarchyIndentPrefix(indent_item_counter);
                ImGui::Text("%s", indentation_prefix.c_str());
                ImGui::SameLine();
                //indent_item_counter[indent].first++;
                
                //Selection mechanic
                if (name == m_lastSelectedHierarchyItem && !selected_object)
                    selected_object = _hierarchyObject;
                if (ImGui::Selectable(name.c_str(), name == m_lastSelectedHierarchyItem))
                {
                    m_lastSelectedHierarchyItem = name;
                    selected_object = _hierarchyObject;
                }

                //recursive call to children
                const auto& children = _hierarchyObject->getAllChildren();
                //adding indent
                indent++;
                indent_item_counter[indent-1].first++;
                indent_item_counter[indent].first = 0;
                indent_item_counter[indent].second = children.size();
                //iterating over children
                for (const auto& [child_name, child] : children)
                {
                    hierarchy_handler(child, "");
                }
                //finished working with direct children, removing indent
                indent_item_counter.erase(indent); 
                indent--;
            };

        hierarchy_handler(main_root, "Root");
    }
    ImGui::EndChild();
    
    //Rendering selected item's exposed data
    if (ImGui::BeginChild("Selected Item",ImVec2(-1.f, -1.f), ImGuiChildFlags_Border | ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX))
    {
        if (selected_object)
            selected_object->onGui(m_lastSelectedHierarchyItem);
    }
    ImGui::EndChild(); //"Selected Item"
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

void ImG_debuger::handleButtonElement(const std::string& _folder, const std::string& _name)
{
    if(!m_imguiFolders[_folder].buttons[_name])
        return;

    if(ImGui::Button(_name.c_str()))
    {
        m_imguiFolders[_folder].buttons[_name]();
    }
}

void ImG_debuger::imGUI_Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("ImGUI Debugger");
    if (ImGui::BeginTabBar("Main Tab Bar"))
    {
        if (ImGui::BeginTabItem("Main Tab"))
        {
            generateImGUIContent();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Node Hierarchy"))
        {
            renderHierarchy();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("General info"))
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar(); //ends "Main Tab Bar"
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (m_scheduleBetweenFrames)
    {
        m_scheduleBetweenFrames();
        m_scheduleBetweenFrames = nullptr;
    }
}

bool ImG_debuger::folderExists(const std::string& _folderName) const
{
    return m_imguiFolders.find(_folderName) != m_imguiFolders.end();
}

void ImG_debuger::addFolder(std::string&& _folderName)
{
    m_imguiFolders.emplace(std::move(_folderName), ImGUI_folder());
}

void ImG_debuger::addMainMenuItem(const std::string& _menuName, std::function<void()>&& _menuActivationCB)
{
    addDebugButtonToFolder(
        "Main menu",
        _menuName,
        std::move(_menuActivationCB)
    );
}
