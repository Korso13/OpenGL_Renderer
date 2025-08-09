#pragma once

#include "pch.h"
#include "ImGUI/imgui.h"

//NOTE: might need to move to a separate file with all ImGui defines with settings
#define IMG_LISTBOX_ENTRIES_IN_FRAME 5

class ImGuiUtils
{
public:
    static ImVec2 getSizeForChild(float _windowHeightPercent = 0.5);
	//Templated combolist renderers. 
	//Returns pair: bool - set to true if new item was selected this frame; T - new selected value (if no new item selected, returns value by listCurrentValue key or random if key not found)
	template<typename T>
	static std::pair<bool, T> imGuiComboList(const std::string& _listName, const std::map<std::string, T>& _list, const std::string& _listCurrentValue);
	//Returns pair: bool - set to true if new item was selected this frame; T - new selected value (if no new item selected, returns value by listCurrentValue key or random if key not found)
	template<typename T>
	static std::pair<bool, T> imGuiComboList(const std::string& _listName, const std::vector<T>& _list, const std::string& _listCurrentValue, std::function<const std::string (const T&)> _nameGetterPred);
	
	//Templated list box renderer. 
	//Returns pair: bool - set to true if new item was selected this frame; T - new selected value (if no new item selected, returns value by listCurrentValue key or random if key not found)
	template<typename T>
	static std::pair<bool, T> imGuiListBox(const std::string& _listName, const std::map<std::string, T>& _list, const std::string& _listCurrentValue);
	//Returns pair: bool - set to true if new item was selected this frame; T - new selected value (if no new item selected, returns value by listCurrentValue key or random if key not found)
	template<typename T>
	static std::pair<bool, T> imGuiListBox(const std::string& _listName, const std::vector<T>& _list, const std::string& _listCurrentValue, std::function<const std::string(const T&)> _nameGetterPred);
	//List box renderer for simple strings. 
	//Returns pair: bool - set to true if new item was selected this frame; std::string - new selected value (if no new item selected, returns listCurrentValue)
	static std::pair<bool, std::string> imGuiListBox(const std::string& _listName, const std::set<std::string>& _list, const std::string& _listCurrentValue);
	//todo: add variants of ImGuiComboList and ImGuiListBox for unordered_map (add templated iterator type checker?) 

	//Returns a string that can be used as a text prefix for hierarchy-like Combo list etc. 
	//_indentationData structure: indent level (starts with 0) - current element in indent level(starts with 0) - total elements in indent level (starts with 1 since it's "size")
	static std::string formHierarchyIndentPrefix(const  std::map<size_t, std::pair<size_t, size_t>>& _indentationData);

};
