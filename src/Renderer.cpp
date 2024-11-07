#include "Renderer.h"

namespace node_editor
{
	//*************************************************************************
	//						LAYERS MANAGER IMPLEMENTATION
	//*************************************************************************

	Layers_manager::Layers_manager()
	{
		auto&& draw_list = ImGui::GetWindowDrawList();

		draw_list->ChannelsSplit(static_cast<int>(render_layer::main_ui));
	}

	//*************************************************************************

	void Layers_manager::set_active_layer(render_layer layer)
	{
		auto&& draw_list = ImGui::GetWindowDrawList();

		draw_list->ChannelsSetCurrent(static_cast<int>(layer));

		m_active_layer = layer;
	}

	//*************************************************************************

	render_layer Layers_manager::get_active_layer() const
	{
		return m_active_layer;
	}

	//*************************************************************************

	void Layers_manager::merge_layers()
	{
		auto&& draw_list = ImGui::GetWindowDrawList();

		draw_list->ChannelsMerge();
	}

	//*************************************************************************



	//*************************************************************************
	//						RENDER CONTEXT IMPLEMENTATION
	//*************************************************************************

	void Render_context::set_pin_info(const std::string& pin_name,
		const Pin_info& info)
	{
		m_pins_info[pin_name] = info;
	}

	//*************************************************************************

	Pin_info* Render_context::find_pin_info(const std::string& pin_name)
	{
		using std::end;

		Pin_info* result{ nullptr };

		auto&& it = m_pins_info.find(pin_name);

		if (it != end(m_pins_info))
		{
			result = &(it->second);
		}

		return result;
	}

	//*************************************************************************

	const Pin_info* Render_context::find_pin_info(const std::string& pin_name) 
		const
	{
		using std::end;

		const Pin_info* result{ nullptr };

		auto&& it = m_pins_info.find(pin_name);

		if (it != end(m_pins_info))
		{
			result = &(it->second);
		}

		return result;
	}

	//*************************************************************************

	void Render_context::set_pin_icon_size(const ImVec2& icons_size)
	{
		m_pin_icon_size = icons_size;
	}

	//*************************************************************************

	const ImVec2 Render_context::get_pin_icon_size() const
	{
		return m_pin_icon_size;
	}

	//*************************************************************************

	Node_state* Render_context::add_node(const Node_state& node)
	{
		auto&& result = m_nodes[node.id];

		result = node;

		return &result;
	}

	//*************************************************************************

	Node_state* Render_context::get_node(std::size_t id)
	{
		return &m_nodes[id];
	}

	//*************************************************************************

	Pin_state* Render_context::add_pin(const Pin_state& pin)
	{
		auto&& result = m_pins[pin.id];

		result = pin;

		return &result;
	}

	//*************************************************************************

	Pin_state* Render_context::get_pin(std::size_t id)
	{
		return &m_pins[id];
	}

	//*************************************************************************

	Link_state* Render_context::add_link(const Link_state& link)
	{
		auto&& result = m_links[link.id];

		result = link;

		return &result;
	}

	//*************************************************************************

	Link_state* Render_context::get_link(std::size_t id)
	{
		return &m_links[id];
	}

	//*************************************************************************

	ImVec2 Render_context::get_start_node_position() const
	{
		return ImVec2();
	}

	//*************************************************************************

	Layers_manager& Render_context::get_layers_manager()
	{
		return m_layers;
	}

	//*************************************************************************

} // namespace node_editor