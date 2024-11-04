#pragma once

//#include "my/Node_pin.h"
//#include "my/Node.h"
//#include "my/Execution_context.h"
//#include "my/Blueprint.h"
//#include "my/widgets/Menu_bar.h"
//#include "my/Renderer.h"
//#include "my/Renderer_utility.h"
//#include "my/INode_factory.h"
//#include "my/widgets/IWidget.h"
//#include "my/widgets/Panel.h"
//
//#include "support_utility.h"
//
//#include <imgui_node_editor.h>
//
//#include <map>
//#include <string>
//#include <string_view>
//#include <memory>

#include "fwd.h"

namespace node_editor
{
	class Node_editor final : public std::enable_shared_from_this<Node_editor>
	{
	public:

		Context_data* get_context_data();
		const Context_data* get_context_data() const;

		Render_context* get_render_context();
		const Render_context* get_render_context() const;

	//	using blueprint_pointer = std::unique_ptr<Blueprint>;
	//	using blueprints_container = std::map<std::string, blueprint_pointer>;
	//	using node_factory_pointer = std::unique_ptr<INode_factory>;
	//	using node_factory_obj = std::pair<std::string, node_factory_pointer>;
	//	using node_factories_container = std::vector<node_factory_obj>;
	//	using panels_container = std::vector<std::unique_ptr<IWidget>>;

	//	Editor(const Vector2f& start_window_size,
	//		bool create_default_widgets = true);

	//	Editor(const Editor&) = delete;
	//	Editor& operator=(const Editor&) = delete;

	//	Editor(Editor&&) = default;
	//	Editor& operator=(Editor&&) = default;

	//	~Editor();

	//	//void open();
	//	//void close();

	//	void change_window_size(float width, float height);
	//	void change_window_size(const Vector2f& size);

	//	void update_input(float dt);
	//	void render(float dt);

	//	bool add_node_factory(const std::string& factory_name,
	//		node_factory_pointer factory);

	//	void add_node(std::unique_ptr<INode> node);
	//	void add_node(const INode& node);

	//	span<node_factory_obj> get_factories();
	//	span<const node_factory_obj> get_factories() const;

	//	bool create_blueprint(const std::string& name);

	//	void set_active_blueprint(const std::string& name);
	//	void set_active_blueprint(Blueprint& blueprint);
	//	Blueprint* get_active_blueprint();
	//	const Blueprint* get_active_blueprint() const;

	//	/*span<blueprints_container> get_blueprints();
	//	span<const blueprints_container> get_blueprints() const;*/

	//private:
	//	void init();
	//	void init_menu();
	//	void init_factories();
	//	void init_panels();

	//	void render_menu_bar(float dt);
	//	void render_top_panel(float dt);
	//	void render_left_panels(float dt);
	//	void render_right_panels(float dt);
	//	void render_main_space(float dt);

	//	void update_panels_size();

	//	void handle_create_action();
	//	void handle_destroy_action();

	//	Color generate_color(const INode_pin& pin) const;

	//private:
	//	static ax::NodeEditor::EditorContext* m_Editor;

	//	Renderer m_renderer;

	//	node_factories_container m_factories;

	//	blueprints_container m_blueprints;
	//	Blueprint* m_active_blueprint{ nullptr };

	//	Vector2f m_window_size;
	//	Vector2f m_panels_padding{ 100.f, 100.f };

	//	float m_menu_bar_height = 19.f;

	//	Menu_bar m_menu;
	//	Panel m_top_panel;
	//	Panel m_left_panel;
	//	Panel m_central_panel;
	//	Panel m_right_panel;

	//	std::vector<IWidget*> m_left_panels;
	//	std::vector<IWidget*> m_right_panels;

	//	panels_container m_panels;
	};

} // namespace node_editor