#ifndef __NODE_EDITOR_RENDERER_CONTEXT_H__
#define __NODE_EDITOR_RENDERER_CONTEXT_H__

#include "INode.h"
#include "IPin.h"

#include <imgui.h>

#include <map>
#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace node_editor
{
	struct Pin_info
	{
		using color_type = ImVec4;

		color_type color;
	};

	struct Context_data
	{
		INode* current_node{ nullptr };
		INode* prev_node{ nullptr };
		INode* next_node{ nullptr };

		IPin* current_pin{ nullptr };
		IPin* prev_pin{ nullptr };
		IPin* next_pin{ nullptr };
	};

	class Render_context
	{
	public:
		using pin_info_container = std::map<std::string, Pin_info>;
		using iterator = typename pin_info_container::iterator;
		using const_iterator = typename pin_info_container::const_iterator;

		void set_pin_info(const IPin& pin, const Pin_info& info);
		void set_pin_info(const IPin& pin, Pin_info&& info);
		const Pin_info* find_pin_info(const IPin& pin) const;

		void set_pin_icon_size(const ImVec2& icons_size);
		const ImVec2 get_pin_icon_size() const;

	private:

	private:
		pin_info_container m_pins_info;
		ImVec2 m_pin_icon_size{ 32.f, 32.f };
	};

} // namespace node_editor

#endif // !__NODE_EDITOR_RENDERER_CONTEXT_H__
