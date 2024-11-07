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

	struct Node_state
	{
		std::size_t id{ 0 };
		ImVec2 position{ 0.f, 0.f };
		bool is_visible{ true };
	};

	struct Pin_state
	{
		std::size_t id{ 0 };
		ImVec2 position{ 0.f, 0.f };
		bool is_input{ true };
		bool is_visible{ true };
	};

	struct Link_state
	{
		std::size_t id{ 0 };
		std::size_t start_pin_id{ 0 };
		std::size_t end_pin_id{ 0 };
		bool is_visible{ true };
	};

	struct Context_data
	{
		INode* current_node{ nullptr };
		INode* prev_node{ nullptr };
		INode* next_node{ nullptr };

		IPin* current_pin{ nullptr };
		IPin* prev_pin{ nullptr };
		IPin* next_pin{ nullptr };

		Node_state* current_node_state{ nullptr };
		Node_state* prev_node_state{ nullptr };
		Node_state* next_node_state{ nullptr };

		Pin_state* current_pin_state{ nullptr };
		Pin_state* prev_pin_state{ nullptr };
		Pin_state* next_pin_state{ nullptr };

		Link_state* current_link_state{ nullptr };
		Link_state* prev_link_state{ nullptr };
		Link_state* next_link_state{ nullptr };

		ImVec4 node_background_color{ 0.5f, 0.5f, 0.5f, 1.f };
		ImVec4 node_top_color{ 0.1f, 0.5f, 0.f, 1.f };
	};

	enum class render_layer
	{
		background,
		canvas,
		node_background,
		node_data,
		pin_links,
		context_ui,
		main_ui
	};

	class Layers_manager final
	{
	public:
		Layers_manager();

		void set_active_layer(render_layer layer);
		render_layer get_active_layer() const;

		void merge_layers();

	private:
		render_layer m_active_layer{ render_layer::background };
	};

	class Render_context
	{
	public:
		using pin_info_container = std::map<std::string, Pin_info>;
		using iterator = typename pin_info_container::iterator;
		using const_iterator = typename pin_info_container::const_iterator;
		using node_state_container = std::map<std::size_t, Node_state>;
		using pin_state_container = std::map<std::size_t, Pin_state>;
		using link_state_container = std::map<std::size_t, Link_state>;

		void set_pin_info(const std::string& pin_name, const Pin_info& info);
		Pin_info* find_pin_info(const std::string& pin_name);
		const Pin_info* find_pin_info(const std::string& pin_name) const;

		void set_pin_icon_size(const ImVec2& icons_size);
		const ImVec2 get_pin_icon_size() const;

		Node_state* add_node(const Node_state& node);
		Node_state* get_node(std::size_t id);

		Pin_state* add_pin(const Pin_state& pin);
		Pin_state* get_pin(std::size_t id);

		Link_state* add_link(const Link_state& link);
		Link_state* get_link(std::size_t id);

		ImVec2 get_start_node_position() const;

		Layers_manager& get_layers_manager();

	private:
		pin_info_container m_pins_info;
		node_state_container m_nodes;
		pin_state_container m_pins;
		link_state_container m_links;
		Layers_manager m_layers;
		ImVec2 m_pin_icon_size{ 32.f, 32.f };
	};

} // namespace node_editor

#endif // !__NODE_EDITOR_RENDERER_CONTEXT_H__
