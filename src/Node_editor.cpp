#include "my/Editor.h"

#include "my/Item_builder.h"
#include "my/node_factories.h"
#include "my/nodes_library.h"
#include "my/Editor_wrapper.h"
#include "my/widgets/Nodes_panel.h"

#include <imgui_extras.h>

#include <iostream>

namespace node_editor
{
	ax::NodeEditor::EditorContext* Editor::m_Editor = nullptr;

	static ImEx::MostRecentlyUsedList Application_GetMostRecentlyOpenFileList()
	{
		return ImEx::MostRecentlyUsedList("MostRecentlyOpenList");
	}

	//*************************************************************************

	Editor::Editor(const Vector2f& start_window_size,
		bool create_default_widgets)
		:m_window_size{ start_window_size }
	{
		namespace ed = ax::NodeEditor;

		//ImEx::MostRecentlyUsedList::Install(ImGui::GetCurrentContext());

		ed::Config config;
		//InstallDocumentCallbacks(config);
		m_Editor = ed::CreateEditor(&config);
		ed::SetCurrentEditor(m_Editor);

		init();

		//ed::NavigateToContent();

		ed::SetCurrentEditor(nullptr);
	}

	//*************************************************************************

	Editor::~Editor()
	{
		namespace ed = ax::NodeEditor;

		if (m_Editor)
		{
			ed::DestroyEditor(m_Editor);
		}

		m_Editor = nullptr;
	}

	//*************************************************************************

	void Editor::change_window_size(float width, float height)
	{
		m_window_size.x = width;
		m_window_size.y = height;
	}

	//*************************************************************************

	void Editor::change_window_size(const Vector2f& size)
	{
		change_window_size(size.x, size.y);
	}

	//*************************************************************************

	void Editor::update_input(float dt)
	{

	}

	//*************************************************************************

	void Editor::render(float dt)
	{
		namespace ed = ax::NodeEditor;

		//ed::SetCurrentEditor(m_Editor);

		update_panels_size();

		render_menu_bar(dt);
		render_top_panel(dt);
		render_left_panels(dt);
		update_panels_size();
		render_right_panels(dt);
		update_panels_size();
		render_main_space(dt);

		/*if (m_active_blueprint)
		{
			m_renderer.draw(m_nodes_panel);

			auto&& index = m_nodes_panel.get_active_item();

			if (index >= 0)
			{
				auto&& node = m_factories[index].second->create_node();

				m_active_blueprint->add_node(std::move(node));
			}

			m_nodes_panel.set_active_item(-1);

			ed::Begin("###main_editor");

			auto&& nodes = m_active_blueprint->get_nodes();

			for (auto&& node : nodes)
			{
				m_renderer.draw(*node);
			}

			handle_create_action();
			handle_destroy_action();

			ed::End();
		}
		else
		{
			ImGui::Dummy(ImGui::GetContentRegionAvail());
		}*/

		//ed::SetCurrentEditor(nullptr);
	}

	//*************************************************************************

	bool Editor::add_node_factory(const std::string& factory_name,
		node_factory_pointer factory)
	{
		using std::begin;
		using std::end;

		auto&& founder = [&factory_name](auto&& pair)
			{
				auto&& [name, _] = pair;

				return name == factory_name;
			};

		auto&& it = std::find_if(begin(m_factories), end(m_factories), founder);

		if (it == end(m_factories))
		{
			m_factories.push_back({ factory_name, std::move(factory) });

			return true;
		}

		return false;
	}

	//*************************************************************************

	void Editor::add_node(std::unique_ptr<INode> node)
	{
		if (m_active_blueprint)
		{
			auto&& input_pins = node->get_input_pins();
			auto&& output_pins = node->get_output_pins();

			for (auto&& pin : input_pins)
			{
				m_renderer.add_pin_color(*pin, generate_color(*pin));
			}

			for (auto&& pin : output_pins)
			{
				m_renderer.add_pin_color(*pin, generate_color(*pin));
			}

			m_active_blueprint->add_node(std::move(node));

			ax::NodeEditor::SetCurrentEditor(m_Editor);
			ax::NodeEditor::NavigateToContent();
			ax::NodeEditor::SetCurrentEditor(nullptr);
		}
	}

	//*************************************************************************

	void Editor::add_node(const INode& node)
	{
		if (m_active_blueprint)
		{
			auto&& input_pins = node.get_input_pins();
			auto&& output_pins = node.get_output_pins();

			for (auto&& pin : input_pins)
			{
				m_renderer.add_pin_color(*pin, generate_color(*pin));
			}

			for (auto&& pin : output_pins)
			{
				m_renderer.add_pin_color(*pin, generate_color(*pin));
			}

			m_active_blueprint->add_node(node);

			ax::NodeEditor::SetCurrentEditor(m_Editor);
			ax::NodeEditor::NavigateToContent();
			ax::NodeEditor::SetCurrentEditor(nullptr);
		}
	}

	//*************************************************************************

	span<typename Editor::node_factory_obj> Editor::get_factories()
	{
		return span<node_factory_obj>{ m_factories };
	}

	//*************************************************************************

	span<const typename Editor::node_factory_obj>
		Editor::get_factories() const
	{
		return span<const node_factory_obj>{ m_factories };
	}

	//*************************************************************************

	bool Editor::create_blueprint(const std::string& name)
	{
		using std::end;

		auto&& it = m_blueprints.find(name);

		if (it != end(m_blueprints))
		{
			m_blueprints.emplace(name, std::make_unique<Blueprint>());

			return true;
		}

		return false;
	}

	//*************************************************************************

	void Editor::set_active_blueprint(const std::string& name)
	{
		using std::end;

		auto&& it = m_blueprints.find(name);

		if (it != end(m_blueprints))
		{
			auto&& [_, blueprint] = *it;

			m_active_blueprint = blueprint.get();
		}
	}

	//*************************************************************************

	void Editor::set_active_blueprint(Blueprint& blueprint)
	{
		m_active_blueprint = &blueprint;
	}

	//*************************************************************************

	Blueprint* Editor::get_active_blueprint()
	{
		return m_active_blueprint;
	}

	//*************************************************************************

	const Blueprint* Editor::get_active_blueprint() const
	{
		return m_active_blueprint;
	}

	//*************************************************************************

	//span<typename Editor::blueprints_container> Editor::get_blueprints()
	//{
	//	return span<blueprints_container>{ m_blueprints };
	//}

	////*************************************************************************

	//span<const typename Editor::blueprints_container> 
	//	Editor::get_blueprints() const
	//{
	//	return span<const blueprints_container>{ m_blueprints };
	//}

	//*************************************************************************

	void Editor::init()
	{
		init_menu();
		init_factories();
		init_panels();

		auto&& blueprint = std::make_unique<Blueprint>();

		m_active_blueprint = blueprint.get();

		m_blueprints.emplace("TEST BLUEPRINT", std::move(blueprint));
	}

	//*************************************************************************

	void Editor::init_menu()
	{
		Menu_bar::Item open_file
		{
			"Open file",
			"Open script file",
			[](auto&& button_name) { std::cout << "Open file!\n"; }
		};

		Menu_bar::Item save_file
		{
			"Save file",
			"Save script file",
			[](auto&& button_name) { std::cout << "Save file!\n"; }
		};

		Menu_bar::Item load_file
		{
			"Load file",
			"Load script file",
			[](auto&& button_name) { std::cout << "Load file!\n"; }
		};

		Menu_bar::Item edit_blueprint
		{
			"Edit blueprint",
			"Edit blueprint scrpit",
			[](auto&& button_name) { std::cout << "Edit blueprint!\n"; }
		};

		Menu_bar::Item programm_settings
		{
			"Settings",
			"Open programm settings",
			[](auto&& button_name) { std::cout << "Programm settings!\n"; }
		};

		m_menu.add_item("File", std::move(open_file));
		m_menu.add_item("File", std::move(save_file));
		m_menu.add_item("File", std::move(load_file));

		m_menu.add_item("Edit", std::move(edit_blueprint));

		m_menu.add_item("About", std::move(programm_settings));
	}

	//*************************************************************************

	void Editor::init_factories()
	{
		auto&& node_int = []()
			{
				return std::make_unique<Node_value<int>>(1);
			};

		using type = std::remove_cv_t<decltype(node_int)>;

		auto&& factory = std::make_unique<Node_factory<type>>(std::move(node_int));

		m_factories.push_back({ "Int node", std::move(factory) });
	}

	//*************************************************************************

	void Editor::init_panels()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

		m_top_panel.set_raw_flags(flags | ImGuiWindowFlags_NoTitleBar);
		m_left_panel.set_raw_flags(flags);
		m_right_panel.set_raw_flags(flags);
		m_central_panel.set_raw_flags(flags);

		m_top_panel.set_title("Top panel");
		m_left_panel.set_title("Left panel");
		m_right_panel.set_title("Right panel");
		m_central_panel.set_title("Blueprint");

		m_left_panel.set_flag(panel_flag::use_min_size);
		m_left_panel.set_flag(panel_flag::use_max_size);
		m_left_panel.set_flag(panel_flag::resize_only_right);

		m_right_panel.set_flag(panel_flag::use_min_size);
		m_right_panel.set_flag(panel_flag::use_max_size);
		m_right_panel.set_flag(panel_flag::resize_only_left);

		m_top_panel.set_size(m_window_size.x, 30.f);

		float space_height = m_window_size.y - m_menu.get_rect().size.y -
			m_top_panel.get_size().y;

		Vector2f panel_min_size{ 150.f, space_height };

		m_left_panel.set_min_size(panel_min_size);
		m_right_panel.set_min_size(panel_min_size);

		m_left_panel.set_size(panel_min_size);
		m_right_panel.set_size(panel_min_size);

		update_panels_size();

		auto&& nodes_panel = std::make_unique<Nodes_panel>(
			*this, m_left_panel.get_position() + Vector2f{ 5.f, 5.f },
			m_left_panel.get_size() - Vector2f{ 10.f, 10.f });

		m_left_panels.push_back(nodes_panel.get());
		m_panels.push_back(std::move(nodes_panel));
	}

	//*************************************************************************

	void Editor::render_menu_bar(float dt)
	{
		m_menu.draw();
	}

	//*************************************************************************

	void Editor::render_top_panel(float dt)
	{
		Vector2f position
		{
			m_menu.get_position().x,
			m_menu.get_position().y + m_menu.get_rect().size.y
		};

		m_top_panel.set_position(position);

		if (auto&& drawer = m_top_panel.draw())
		{
			ImGui::Text("Test text");
		}
	}

	//*************************************************************************

	void Editor::render_left_panels(float dt)
	{
		Vector2f position
		{
			m_menu.get_position().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		m_left_panel.set_position(position);

		/*auto&& title = m_left_panel.get_title();
		auto&& text_size = ImGui::CalcTextSize(title.data(),
			title.data() + title.size());
		position.y += text_size.y;*/

		if (auto&& drawer = m_left_panel.draw())
		{
			for (auto&& panel : m_left_panels)
			{
				panel->set_position(position);
				panel->draw();

				position.y += panel->get_rect().size.y;
			}
		}
	}

	//*************************************************************************

	void Editor::render_right_panels(float dt)
	{
		Vector2f position
		{
			m_window_size.x - m_right_panel.get_size().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		m_right_panel.set_position(position);

		/*auto&& title = m_right_panel.get_title();
		auto&& text_size = ImGui::CalcTextSize(title.data(),
			title.data() + title.size());
		position.y += text_size.y;*/

		if (auto&& drawer = m_right_panel.draw())
		{
			for (auto&& panel : m_left_panels)
			{
				panel->set_position(position);
				panel->draw();

				position.y += panel->get_rect().size.y;
			}
		}
	}

	//*************************************************************************

	void Editor::render_main_space(float dt)
	{
		namespace ed = ax::NodeEditor;

		Vector2f position
		{
			m_left_panel.get_position().x + m_left_panel.get_size().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		m_central_panel.set_position(position);

		ImVec2 size
		{
			m_central_panel.get_size().x,
			m_central_panel.get_size().y
		};

		ed::SetCurrentEditor(m_Editor);

		if (auto&& drawer = m_central_panel.draw())
		{
			if (m_active_blueprint)
			{
				ed::Begin("ABOBA###main_editor");

				auto&& nodes = m_active_blueprint->get_nodes();

				for (auto&& node : nodes)
				{
					m_renderer.draw(*node);
				}

				handle_create_action();
				handle_destroy_action();

				ed::End();
			}
			else
			{
				ImGui::Dummy(ImGui::GetContentRegionAvail());
			}
		}

		ed::SetCurrentEditor(nullptr);
	}

	//*************************************************************************

	void Editor::update_panels_size()
	{
		float space_height = m_window_size.y - m_menu.get_rect().size.y -
			m_top_panel.get_size().y;

		Vector2f panel_min_size{ 150.f, space_height };
		Vector2f left_panel_max_size
		{
			m_window_size.x - m_right_panel.get_size().x - m_panels_padding.x,
			space_height
		};

		Vector2f right_panel_max_size
		{
			m_window_size.x - m_left_panel.get_size().x - m_panels_padding.x,
			space_height
		};

		m_left_panel.set_min_size(panel_min_size);
		m_right_panel.set_min_size(panel_min_size);

		m_left_panel.set_size(m_left_panel.get_size().x, space_height);
		m_right_panel.set_size(m_right_panel.get_size().x, space_height);

		m_left_panel.set_max_size(left_panel_max_size);
		m_right_panel.set_max_size(right_panel_max_size);

		float space_width = m_window_size.x - m_left_panel.get_size().x -
			m_right_panel.get_size().x;

		m_top_panel.set_position(0.f, m_menu.get_rect().size.y);
		m_top_panel.set_size(m_window_size.x, m_top_panel.get_size().y);
		m_central_panel.set_size(space_width, space_height);

		Vector2f left_position
		{
			m_menu.get_position().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		Vector2f right_position
		{
			m_window_size.x - m_right_panel.get_size().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		Vector2f central_position
		{
			m_left_panel.get_position().x + m_left_panel.get_size().x,
			m_top_panel.get_position().y + m_top_panel.get_size().y
		};

		m_left_panel.set_position(left_position);
		m_right_panel.set_position(right_position);
		m_central_panel.set_position(central_position);
	}

	//*************************************************************************

	void Editor::handle_create_action()
	{
		using std::swap;

		namespace ed = ax::NodeEditor;

		if (!m_active_blueprint) return;

		auto&& blueprint = *m_active_blueprint;

		Item_builder item_builder;

		if (!item_builder) return;

		if (auto link_builder = item_builder.query_new_link())
		{
			auto&& start_pin_id = static_cast<std::size_t>(
				link_builder->start_pin_id.Get());
			auto&& end_pin_id = static_cast<std::size_t>(
				link_builder->end_pin_id.Get());

			auto start_pin = blueprint.find_pin(start_pin_id);
			auto end_pin = blueprint.find_pin(end_pin_id);

			if (!start_pin || !end_pin) return;

			// Editor return pins in order draw by the user. It is up to the
			// user to determine if it is valid. In blueprints we accept only links
			// from receivers to providers. Other graph types may allow bi-directional
			// links between nodes and this ordering make this feature possible.
			if (is_receiver_pin(*start_pin) && is_provider_pin(*end_pin))
			{
				swap(start_pin, end_pin);
			}

			auto start_node = start_pin->get_owner_node();
			auto end_node = end_pin->get_owner_node();

			if (!start_node || !end_node) return;

			//std::cout << "Start pin id : " << start_pin->get_id() << '\n';
			//std::cout << "End pin id : " << end_pin->get_id() << '\n';

			auto can_link_result = end_node->is_can_link_pins(*start_pin, *end_pin);

			if (can_link_result)
			{
				ed::Suspend();
				ImGui::BeginTooltip();
				ImGui::Text("Valid Link%s%s",
					can_link_result.get_description().empty() ? "" : ": ",
					can_link_result.get_description().empty() ? "" : can_link_result.get_description().data());
				ImGui::Separator();
				ImGui::TextUnformatted("From:");
				//ImGui::Bullet(); ImGui::Text("%" PRI_pin, FMT_pin(startPin));
				//ImGui::Bullet(); ImGui::Text("%" PRI_node, FMT_node(startPin->m_Node));
				ImGui::TextUnformatted("To:");
				//ImGui::Bullet(); ImGui::Text("%" PRI_pin, FMT_pin(endPin));
				//ImGui::Bullet(); ImGui::Text("%" PRI_node, FMT_node(endPin->m_Node));
				ImGui::EndTooltip();
				ed::Resume();

				if (link_builder->accept())
				{
					if (is_receiver_pin(*start_pin))
					{
						start_pin->remove_links();
					}
					else
					{
						end_pin->remove_links();
					}

					start_pin->add_link(*end_pin);
					end_pin->add_link(*start_pin);

					//if (end_node->link_pins(*start_pin, *end_pin))
					//{
					std::cout << "Links were connected!\n";
					//}

					/*auto transaction = document.BeginUndoTransaction("Create Link");

					if (startPin->LinkTo(*endPin))
						LOGV("[HandleCreateAction] %" PRI_pin " linked with %" PRI_pin, FMT_pin(startPin), FMT_pin(endPin));
					else
						transaction->Discard();*/
				}
			}
			else
			{
				ed::Suspend();
				ImGui::SetTooltip(
					"Invalid Link: %s",
					can_link_result.get_description().data()
				);
				ed::Resume();

				link_builder->reject();
			}
		}
		else if (auto nodeBuilder = item_builder.query_new_node())
		{
			// Arguably creation of node is simpler than a link.
			ed::Suspend();
			ImGui::SetTooltip("Create Node...");
			ed::Resume();

			// Node builder accept return true when user release mouse button.
			// When this happen we request CreateNodeDialog to open.
			//if (nodeBuilder->accept())
			//{
			//	// Get node from which link was pulled (if any). After creating
			//	// node we will try to make link with first matching pin of the node.
			//	auto pin = blueprint.FindPin(static_cast<uint32_t>(nodeBuilder->m_PinId.Get()));

			//	ed::Suspend();
			//	LOGV("[HandleCreateAction] Open CreateNodeDialog");
			//	m_CreateNodeDailog.Open(pin);
			//	ed::Resume();
			//}
		}
	}

	//*************************************************************************

	void Editor::handle_destroy_action()
	{
		namespace ed = ax::NodeEditor;

		if (!m_active_blueprint) return;

		auto&& blueprint = *m_active_blueprint;

		Item_deleter item_deleter;

		if (!item_deleter) return;

		//auto deferredTransaction = document.GetDeferredUndoTransaction("Destroy Action");

		std::vector<INode*> nodes_to_delete;
		uint32_t broken_link_count = 0;

		// Process all nodes marked for deletion
		while (auto node_deleter = item_deleter.query_deleted_node())
		{
			//deferredTransaction->Begin("Delete Item");

			// Remove node, pass 'true' so links attached to node will also be queued for deletion.
			if (node_deleter->accept(true))
			{
				auto&& node_id = static_cast<std::size_t>(node_deleter->node_id.Get());

				auto node = blueprint.find_node(node_id);

				if (node != nullptr)
					// Queue nodes for deletion. We need to serve links first to avoid crash.
					nodes_to_delete.push_back(node);
			}
		}

		// Process all links marked for deletion
		while (auto link_deleter = item_deleter.query_deleted_link())
		{
			//deferredTransaction->Begin("Delete Item");

			if (link_deleter->accept())
			{
				auto&& start_pin_id = static_cast<std::size_t>(link_deleter->start_pin_id.Get());
				auto&& end_pin_id = static_cast<std::size_t>(link_deleter->end_pin_id.Get());
				auto start_pin = blueprint.find_pin(start_pin_id);
				auto end_pin = blueprint.find_pin(end_pin_id);

				if (start_pin && end_pin && start_pin->is_has_link(*end_pin))
				{
					//LOGV("[HandleDestroyAction] %" PRI_pin " unlinked from %" PRI_pin, FMT_pin(startPin), FMT_pin(startPin->GetLink()));
					start_pin->remove_link(*end_pin);
					++broken_link_count;
				}
			}
		}

		// After links was removed, now it is safe to delete nodes.
		for (auto node : nodes_to_delete)
		{
			//LOGV("[HandleDestroyAction] %" PRI_node, FMT_node(node));

			blueprint.remove_node(*node);
		}

		if (!nodes_to_delete.empty() || broken_link_count)
		{
			/*LOGV("[HandleDestroyAction] %" PRIu32 " node%s deleted, %" PRIu32 " link%s broken",
				static_cast<uint32_t>(nodesToDelete.size()), nodesToDelete.size() != 1 ? "s" : "",
				brokenLinkCount, brokenLinkCount != 1 ? "s" : "");*/
		}
	}

	//*************************************************************************

	Color Editor::generate_color(const INode_pin& pin) const
	{
		auto&& pin_type_info = pin.get_type_info();

		if (pin_type_info == type_info<void*>{})
		{
			return Color{ 255, 255, 255, 255 };
		}
		else if (pin_type_info == type_info<flow>{})
		{
			return Color{ 255, 255, 255, 255 };
		}
		else if (pin_type_info == type_info<char>{})
		{
			return Color{ 0, 0, 50, 255 };
		}
		else if (pin_type_info == type_info<bool>{})
		{
			return Color{ 0, 204, 0, 255 };
		}
		else if (pin_type_info == type_info<int>{})
		{
			return Color{ 153, 0, 153, 255 };
		}
		else if (pin_type_info == type_info<float>{})
		{
			return Color{ 255, 102, 255, 255 };
		}
		else if (pin_type_info == type_info<double>{})
		{
			return Color{ 153, 255, 51, 255 };
		}
		else if (pin_type_info == type_info<std::string>{})
		{
			return Color{ 0, 0, 51, 255 };
		}
		else if (pin_type_info == type_info<any>{})
		{
			return Color{ 255, 255, 255, 255 };
		}
		else if (pin_type_info == type_info<std::string_view>{})
		{
			return Color{ 102, 51, 0, 255 };
		}

		return Color{ 255, 102, 255, 255 };
	}

	//*************************************************************************

} // namespace node_editor