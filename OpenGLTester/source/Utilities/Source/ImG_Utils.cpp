#include "../Public/ImG_Utils.h"
#include "ImGUI/imgui_internal.h"
#include "Utilities/Public/ImG_Defines.h"
#include "Utilities/Public/Utilities.h"

#define SAFE_NAME(name) (((name).empty()) ? ("default_name"+std::to_string(utils::Utilities::getInternalUID())).c_str() : (name).c_str())

ImVec2 ImGuiUtils::getSizeForChild(const float _windowHeightPercent)
{
    //ImVec2 size = ImGui::GetWindowSize();
    ImVec2 size = ImGui::GetCurrentWindow()->Size;
    size.y *= _windowHeightPercent;
    size.x *= 0.98f;

    return size;
}

template<typename T>
std::pair<bool, T> ImGuiUtils::imGuiComboList(const std::string& _listName, const std::map<std::string, T>& _list, const std::string& _listCurrentValue)
{
	std::pair<bool, T> result;
	result.first = false;
	if (!_list.empty())
	{
		if (ImGui::BeginCombo(SAFE_NAME(_listName), _listCurrentValue.c_str()))
		{
			for (auto& [list_key, list_value] : _list)
			{
				if (list_key.empty())
					continue;

				bool is_selected = (list_key == _listCurrentValue);
				if (is_selected)
					ImGui::SetItemDefaultFocus();

				if (ImGui::Selectable(list_key.c_str(), &is_selected))
				{
					result.first = true;
					result.second = list_value;
				}
				if(!result.first && is_selected)
					result.second = list_value;
			}

			ImGui::EndCombo();
		}
	}
	return result;
};

template<typename T>
std::pair<bool, T> ImGuiUtils::imGuiComboList(
	const std::string& _listName, 
	const std::vector<T>& _list, 
	const std::string& _listCurrentValue, 
	std::function<const std::string (const T&)> _nameGetterPred)
{
	std::pair<bool, T> result;
	result.first = false;
	if (!_list.empty())
	{
		if (ImGui::BeginCombo(SAFE_NAME(_listName), _listCurrentValue.c_str()))
		{
			for (const auto& list_value : _list)
			{
				auto list_key = _nameGetterPred(list_value);
				bool is_selected = (list_key == _listCurrentValue);
				if (is_selected)
					ImGui::SetItemDefaultFocus();

				if (ImGui::Selectable(list_key.c_str(), &is_selected))
				{
					result.first = true;
					result.second = list_value;
				}
				if (!result.first && is_selected)
					result.second = list_value;
			}

			ImGui::EndCombo();
		}
	}
	return result;
}


template<typename T>
std::pair<bool, T> ImGuiUtils::imGuiListBox(
	const std::string& _listName, 
	const std::map<std::string, T>& _list, 
	const std::string& _listCurrentValue)
{
	std::pair<bool, T> result;
	result.first = false;
	if (!_list.empty())
	{
		if (ImGui::BeginListBox(SAFE_NAME(_listName), ImVec2(-FLT_MIN, std::min(CastI(_list.size()), IMG_LISTBOX_ENTRIES_IN_FRAME) * ImGui::GetTextLineHeightWithSpacing()))) //use all width, IMG_LISTBOX_ENTRIES_IN_FRAME items tall
		{
			for (auto& [list_key, list_value] : _list)
			{
				if (list_key.empty())
					continue;

				bool is_selected = (list_key == _listCurrentValue);
				if (is_selected)
					ImGui::SetItemDefaultFocus();

				if (ImGui::Selectable(list_key.c_str(), &is_selected))
				{
					result.first = true;
					result.second = list_value;
				}
				if (!result.first && is_selected)
					result.second = list_value;
			}

			ImGui::EndListBox();
		}
	}
	return result;
}

template<typename T>
std::pair<bool, T> ImGuiUtils::imGuiListBox(
	const std::string& _listName, 
	const std::vector<T>& _list, 
	const std::string& _listCurrentValue, 
	std::function<const std::string (const T&)> _nameGetterPred)
{
	std::pair<bool, T> result;
	result.first = false;
	if (!_list.empty())
	{
		if (ImGui::BeginListBox(SAFE_NAME(_listName), ImVec2(-FLT_MIN, std::min(CastI(_list.size()), IMG_LISTBOX_ENTRIES_IN_FRAME) * ImGui::GetTextLineHeightWithSpacing()))) //use all width, IMG_LISTBOX_ENTRIES_IN_FRAME items tall
		{
			for (const auto& list_value : _list)
			{
				auto list_key = _nameGetterPred(list_value);
				if (list_key.empty())
					continue;
				bool is_selected = (list_key == _listCurrentValue);
				if (is_selected)
					ImGui::SetItemDefaultFocus();
				
				if (ImGui::Selectable(list_key.c_str(), is_selected))
				{
					result.first = true;
					result.second = list_value;
				}
				if (!result.first && is_selected)
					result.second = list_value;
			}

			ImGui::EndListBox();
		}
	}
	return result;
}

std::pair<bool, std::string> ImGuiUtils::imGuiListBox(const std::string& _listName, const std::set<std::string>& _list, const std::string& _listCurrentValue)
{
	std::pair<bool, std::string> result;
	result.first = false;
	result.second = _listCurrentValue;

	if (!_list.empty())
	{
		if (ImGui::BeginListBox(SAFE_NAME(_listName), ImVec2(-FLT_MIN, std::min(CAST_I(_list.size()), IMG_LISTBOX_ENTRIES_IN_FRAME) * ImGui::GetTextLineHeightWithSpacing()))) //use all width, IMG_LISTBOX_ENTRIES_IN_FRAME items tall
		{
			for (auto& list_value : _list)
			{
				if (list_value.empty())
					continue;

				bool is_selected = list_value == _listCurrentValue;
				if (is_selected)
					ImGui::SetItemDefaultFocus();

				if (ImGui::Selectable(list_value.c_str(), &is_selected))
				{
					result.first = true;
					result.second = list_value;
				}
				if (!result.first && is_selected)
					result.second = list_value;
			}

			ImGui::EndListBox();
		}
	}

	return result;
}

//todo: fix me pls (indent is not selecting correct glyphs, probably needs rewrite into 2 methods: 1 mapping hierarchy (with caching?) and 1 providing indent for specific element)
std::string ImGuiUtils::formHierarchyIndentPrefix(const std::map<size_t, std::pair<size_t, size_t>>& _indentationData)
{
	if (_indentationData.size() < 2)
		return "";

	std::string prefix_line;
	const size_t current_indent = _indentationData.size()-1; //-1 because indent is 0-based
	//std::cout << "========================\n";
#define LOG_INDENT(line) //std::cout << "[" << line << "]" <<"Node indent(current_indent): " << current_indent << ", indent_level: " << indent_level << ", indent_item_idx: " << indent_item_idx << ", indent_level_size: " << indent_level_size << "\nPrefix: " << prefix_line << "\n"
	
	for (const auto& [indent_level, indent_level_data] : _indentationData)
	{
		const size_t indent_item_idx = indent_level_data.first;
		const size_t indent_level_size = indent_level_data.second;
		
		//│ ├ └ ┬ ─      *Used symbols*
		if (indent_level == 1)
		{
			if (indent_item_idx+1 < indent_level_size)
			{
				prefix_line += "|";
				LOG_INDENT(215);
				continue;
			}
			else if (indent_item_idx+1 == indent_level_size)
			{
				prefix_line += "└";
				LOG_INDENT(221);
				continue;
			}
		}
		// tier < current tier-1
		else if ((current_indent > 0) &&
			(indent_level < (current_indent-1)))
		{
			if (indent_item_idx+1 < indent_level_size)
			{
				prefix_line += "|";
				LOG_INDENT(232);
				continue;
			}
			else if (indent_item_idx+1 == indent_level_size)
			{
				prefix_line += "└";
				LOG_INDENT(238);
				continue;
			}
		}
		//current tier-1
		else if ((indent_level > 0) && (indent_level == (current_indent-1)))
		{
			if (indent_item_idx+1 < indent_level_size)
			{
				prefix_line += "├";
				LOG_INDENT(248);
				continue;
			}
			else if (indent_item_idx+1 == indent_level_size)
			{
				prefix_line += "└";
				LOG_INDENT(254);
				continue;
			}
		}
		//current tier
		else if (indent_level == current_indent && indent_level > 0)
		{
			if (indent_item_idx == 0)
			{
				prefix_line += "┬";
				LOG_INDENT(264);
				continue;
			}
			else if (indent_item_idx+1 < indent_level_size)
			{
				prefix_line += "─";
				LOG_INDENT(270);
				continue;
			}
			else if (indent_item_idx+1 == indent_level_size)
			{
				prefix_line += "└";
				LOG_INDENT(276);
				continue;
			}
		}
		
		prefix_line += " ";
		LOG_INDENT(282);
	}

	//std::cout << "\nPrefix: " << prefix_line << "\n========================\n";
	return prefix_line;
}