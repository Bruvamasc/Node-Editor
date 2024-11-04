#include "node_editor/nodes_library.h"

#include "node_editor/Executor.h"

#include <iostream>

namespace node_editor
{
	//#pragma region Node to string
		//*************************************************************************
		//							NODE TO STRING IMPLEMENTATION
		//*************************************************************************

	Node_to_string::Node_to_string(std::size_t id)
		:m_id{ id },
		m_name{ "To string" },
		m_description{ "Convert value to string" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}

		m_input_pin.set_name("value");
		m_output_pin.set_name("string");
	}

	//*************************************************************************

	Pin<flow>* Node_to_string::execute(IExecutor& visitor)
	{
		IPin* linked_pin{ nullptr };

		if (m_input_pin.is_has_links())
		{
			linked_pin = m_input_pin.get_links().front();
		}

		//auto&& linked_pin = m_input_pin.get_link_pin();

		std::string output_value;

		if (linked_pin)
		{
			auto&& pin_type = linked_pin->get_type_info();

			if (pin_type == type_info<bool>{})
			{
				auto&& pin = cast_to<bool>(*linked_pin);

				if (pin->get_value())
				{
					output_value = "true";
				}
				else
				{
					output_value = "false";
				}
			}
			else if (pin_type == type_info<char>{})
			{
				auto&& pin = cast_to<char>(*linked_pin);

				output_value.push_back(pin->get_value());
			}
			else if (pin_type == type_info<int>{})
			{
				auto&& pin = cast_to<int>(*linked_pin);

				output_value = std::to_string(pin->get_value());
			}
			else if (pin_type == type_info<float>{})
			{
				auto&& pin = cast_to<float>(*linked_pin);

				output_value = std::to_string(pin->get_value());
			}
			else if (pin_type == type_info<double>{})
			{
				auto&& pin = cast_to<double>(*linked_pin);

				output_value = std::to_string(pin->get_value());
			}
			else if (pin_type == type_info<std::string>{})
			{
				auto&& pin = cast_to<std::string>(*linked_pin);

				output_value = pin->get_value();
			}
			else if (pin_type == type_info<std::string_view>{})
			{
				auto&& pin = cast_to<std::string_view>(*linked_pin);

				output_value = pin->get_value();
			}
		}

		m_output_pin.set_value(output_value);

		return &m_exit_pin;
	}

	//*************************************************************************

	void Node_to_string::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{
		if (&pin != &m_input_pin || !m_input_pin.is_has_links()) return;

		auto&& link_pin = m_input_pin.get_links().front();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);

		/*if (&pin != &m_input_pin || !m_input_pin.is_has_link_pin()) return;

		auto&& link_pin = m_input_pin.get_link_pin();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);*/
	}

	//*************************************************************************

	void Node_to_string::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_to_string::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_to_string::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_to_string::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_to_string::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_to_string::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_to_string::is_can_link_pins(
		const IPin& receiver, const IPin& provider) const
	{
		if (&receiver == &m_input_pin)
		{
			auto&& pin_type = provider.get_type_info();

			bool is_can_linked = false;

			if (pin_type == type_info<bool>{}
			|| pin_type == type_info<char>{}
			|| pin_type == type_info<int>{}
			|| pin_type == type_info<float>{}
			|| pin_type == type_info<double>{}
			|| pin_type == type_info<std::string>{}
			|| pin_type == type_info<std::string_view>{})
			{
				return Pin_link_result{ true };
			}

			return Pin_link_result
			{
				false,
				"Type of provider are not supported!"
			};
		}

		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_to_string::is_can_unlink_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return is_can_link_pins(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_to_string::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_to_string::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_to_string::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_to_string::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_to_string::clone() const
	{
		return std::make_unique<Node_to_string>(*this);
	}

	//*************************************************************************
//#pragma endregion

//#pragma region Node branch
	//*************************************************************************
	//							NODE BRANCH IMPLEMENTATION
	//*************************************************************************

	Node_branch::Node_branch(std::size_t id)
		:m_id{ id },
		m_name{ "Branch" },
		m_description{ "True or false branch" },
		m_input_pin{ 4, *this, true }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}

		m_exit_true_pin.set_name("true");
		m_exit_false_pin.set_name("false");
	}

	//*************************************************************************

	Pin<flow>* Node_branch::execute(IExecutor& visitor)
	{
		auto condition_value = m_input_pin.get_value();

		if (condition_value)
		{
			return &m_exit_true_pin;
		}

		return &m_exit_false_pin;
	}

	//*************************************************************************

	void Node_branch::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{
		if (&pin != &m_input_pin || !m_input_pin.is_has_links()) return;

		auto&& link_pin = m_input_pin.get_links().front();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);

		/*if (&pin != &m_input_pin || !m_input_pin.is_has_link_pin()) return;

		auto&& link_pin = m_input_pin.get_link_pin();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);*/
	}

	//*************************************************************************

	void Node_branch::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_branch::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_branch::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_branch::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_branch::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_branch::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_branch::is_can_link_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_branch::is_can_unlink_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_branch::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_branch::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_branch::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_branch::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_branch::clone() const
	{
		return std::make_unique<Node_branch>(*this);
	}

	//*************************************************************************
//#pragma endregion


//#pragma region Node print
	//*************************************************************************
	//							NODE PRINT IMPLEMENTATION
	//*************************************************************************

	Node_print::Node_print(std::size_t id)
		:m_id{ id },
		m_name{ "Print" },
		m_description{ "Print string to console" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}

		m_input_pin.set_name("text");
	}

	//*************************************************************************

	Pin<flow>* Node_print::execute(IExecutor& visitor)
	{
		std::cout << m_input_pin.get_value() << '\n';

		return &m_exit_pin;
	}

	//*************************************************************************

	void Node_print::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{
		if (&pin != &m_input_pin || !m_input_pin.is_has_links()) return;

		auto&& link_pin = m_input_pin.get_links().front();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);

		/*if (&pin != &m_input_pin || !m_input_pin.is_has_link_pin()) return;

		auto&& link_pin = m_input_pin.get_link_pin();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);*/
	}

	//*************************************************************************

	void Node_print::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_print::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_print::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_print::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_print::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_print::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_print::is_can_link_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_print::is_can_unlink_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_print::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_print::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_print::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_print::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_print::clone() const
	{
		return std::make_unique<Node_print>(*this);
	}

	//*************************************************************************
//#pragma endregion

//#pragma Node_do_once
	//*************************************************************************
	//							NODE DO ONCE IMPLEMENTATION
	//*************************************************************************

	Node_do_once::Node_do_once(std::size_t id)
		:m_id{ id },
		m_name{ "Do once" },
		m_description{ "Do once some function" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}
	}

	//*************************************************************************

	Pin<flow>* Node_do_once::execute(IExecutor& visitor)
	{
		if (m_is_completed) return nullptr;

		m_is_completed = true;

		return &m_exit_pin;
	}

	//*************************************************************************

	void Node_do_once::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{

	}

	//*************************************************************************

	void Node_do_once::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_do_once::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_do_once::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_do_once::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_do_once::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_do_once::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_do_once::is_can_link_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_do_once::is_can_unlink_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_do_once::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_do_once::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_do_once::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_do_once::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_do_once::clone() const
	{
		return std::make_unique<Node_do_once>(*this);
	}

	//*************************************************************************
//#pragma endregion

//#pragma Node_do_n
	//*************************************************************************
	//							NODE DO N IMPLEMENTATION
	//*************************************************************************

	Node_do_n::Node_do_n(std::size_t id)
		:m_id{ id },
		m_name{ "Do n" },
		m_description{ "Do n some function" },
		m_input_pin{ 3, *this, 0 }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}

		m_input_pin.set_name("count");
	}

	//*************************************************************************

	Pin<flow>* Node_do_n::execute(IExecutor& visitor)
	{
		if (m_is_completed) return nullptr;

		auto&& pin_value = m_input_pin.get_value();

		++m_count;

		if (m_count >= pin_value)
		{
			m_is_completed = true;

			return nullptr;
		}

		return &m_exit_pin;
	}

	//*************************************************************************

	void Node_do_n::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{

	}

	//*************************************************************************

	void Node_do_n::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_do_n::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_do_n::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_do_n::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_do_n::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_do_n::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_do_n::is_can_link_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_do_n::is_can_unlink_pins(const IPin& receiver,
		const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_do_n::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_do_n::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_do_n::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_do_n::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_do_n::clone() const
	{
		return std::make_unique<Node_do_n>(*this);
	}

	//*************************************************************************
//#pragma endregion

//#pragma Node_for_loop_index
	//*************************************************************************
	//					NODE FOR LOOP INDEX IMPLEMENTATION
	//*************************************************************************

	Node_for_loop_index::Node_for_loop_index(std::size_t id)
		:m_id{ id },
		m_name{ "For loop" },
		m_description{ "For loop width start and end index" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}
	}

	//*************************************************************************

	Pin<flow>* Node_for_loop_index::execute(IExecutor& visitor)
	{
		if (m_is_first)
		{
			auto start_index = m_start_index_pin.get_value();

			m_current_index_pin.set_value(start_index);

			m_is_first = false;
		}
		else
		{
			auto last_index = m_current_index_pin.get_value();

			m_current_index_pin.set_value(last_index + 1);
		}

		auto index = m_current_index_pin.get_value();
		auto end_index = m_end_index_pin.get_value();

		if (index < end_index)
		{
			//visitor.PushReturnPoint(entryPoint);

			return &m_loop_body_pin;
		}

		m_is_first = true;

		return &m_exit_pin;
	}

	//*************************************************************************

	void Node_for_loop_index::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{

	}

	//*************************************************************************

	void Node_for_loop_index::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	std::size_t Node_for_loop_index::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	void Node_for_loop_index::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	std::string_view Node_for_loop_index::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	void Node_for_loop_index::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	std::string_view Node_for_loop_index::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	Pin_link_result Node_for_loop_index::is_can_link_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	Pin_link_result Node_for_loop_index::is_can_unlink_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	std::span<IPin*> Node_for_loop_index::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_for_loop_index::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	std::span<IPin*> Node_for_loop_index::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::span<const IPin* const> Node_for_loop_index::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	std::unique_ptr<INode> Node_for_loop_index::clone() const
	{
		return std::make_unique<Node_for_loop_index>(*this);
	}

	//*************************************************************************
//#pragma endregion

} // namespace node_editor;