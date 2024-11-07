#include "Blueprint.h"

#include <algorithm>
#include <iostream>

namespace node_editor
{
	void Blueprint::start()
	{
		if (!m_nodes.empty())
		{
			auto&& node = m_nodes.front();

			auto&& output_pins = node->get_output_pins();

			if (!output_pins.empty())
			{
				auto&& pin = *output_pins.begin();

				if (pin &&
					pin->get_type_hash() == type_info<flow>::get_hash() &&
					pin->get_type_name() == type_info<flow>::get_name())
				{
					auto&& flow_pin = cast_to<flow>(*pin);

					if (flow_pin && m_context)
					{
						m_context->start(*flow_pin);
					}
				}
			}
		}
	}

	//*************************************************************************

	void Blueprint::stop()
	{
		if (!m_context) return;

		m_context->stop();
	}

	//*************************************************************************

	void Blueprint::reset()
	{
		if (!m_context) return;

		m_context->reset();
	}

	//*************************************************************************

	INode* Blueprint::add_node(node_pointer node)
	{
		auto&& node_id = generate_id();

		node->set_id(node_id);

		auto&& input_pins = node->get_input_pins();
		auto&& output_pins = node->get_output_pins();

		for (auto&& pin : input_pins)
		{
			pin->set_id(generate_id());
		}

		for (auto&& pin : output_pins)
		{
			pin->set_id(generate_id());
		}

		m_nodes.push_back(std::move(node));

		return m_nodes.back().get();
	}

	//*************************************************************************

	INode* Blueprint::add_node(const INode& node)
	{
		return add_node(node.clone());
	}

	//*************************************************************************

	INode* Blueprint::add_node(INode&& node)
	{
		return add_node(node.clone());
	}

	//*************************************************************************

	void Blueprint::remove_node(const INode& node)
	{
		using std::begin;
		using std::end;

		auto&& remover = [&node](auto&& node_ptr)
			{
				return node_ptr.get() == &node;
			};

		auto&& it = std::remove_if(begin(m_nodes), end(m_nodes), remover);

		if (it != end(m_nodes))
		{
			m_nodes.erase(begin(m_nodes), it);
		}
	}

	//*************************************************************************

	void Blueprint::remove_node(std::size_t node_id)
	{
		using std::begin;
		using std::end;

		auto&& remover = [=](auto&& node_ptr)
			{
				return node_ptr->get_id() == node_id;
			};

		auto&& it = std::remove_if(begin(m_nodes), end(m_nodes), remover);

		if (it != end(m_nodes))
		{
			m_nodes.erase(begin(m_nodes), it);
		}
	}

	//*************************************************************************

	typename Blueprint::nodes_container& Blueprint::get_nodes()
	{
		return m_nodes;
	}

	//*************************************************************************

	const typename Blueprint::nodes_container& Blueprint::get_nodes() const
	{
		return m_nodes;
	}

	//*************************************************************************

	INode* Blueprint::find_node(std::size_t node_id)
	{
		using std::begin;
		using std::end;

		auto&& founder = [=](auto&& node_ptr)
			{
				return node_ptr->get_id() == node_id;
			};

		auto&& it = std::find_if(begin(m_nodes), end(m_nodes), founder);

		return it != end(m_nodes) ? it->get() : nullptr;
	}

	//*************************************************************************

	const INode* Blueprint::find_node(std::size_t node_id) const
	{
		using std::begin;
		using std::end;

		auto&& founder = [=](auto&& node_ptr)
			{
				return node_ptr->get_id() == node_id;
			};

		auto&& it = std::find_if(begin(m_nodes), end(m_nodes), founder);

		return it != end(m_nodes) ? it->get() : nullptr;
	}

	//*************************************************************************

	IPin* Blueprint::find_pin(std::size_t pin_id)
	{
		using std::begin;
		using std::end;

		auto&& founder = [=](auto&& pin_ptr)
			{
				return pin_ptr->get_id() == pin_id;
			};

		IPin* result = nullptr;

		for (auto&& node : m_nodes)
		{
			auto&& input_pins = node->get_input_pins();
			auto&& output_pins = node->get_output_pins();

			auto&& input_it = std::find_if(begin(input_pins), end(input_pins),
				founder);
			auto&& output_it = std::find_if(begin(output_pins),
				end(output_pins), founder);

			if (input_it != end(input_pins))
			{
				result = *input_it;
				break;
			}
			else if (output_it != end(output_pins))
			{
				result = *output_it;
				break;
			}
		}

		return result;
	}

	//*************************************************************************

	const IPin* Blueprint::find_pin(std::size_t pin_id) const
	{
		using std::begin;
		using std::end;

		auto&& founder = [=](auto&& pin_ptr)
			{
				return pin_ptr->get_id() == pin_id;
			};

		IPin* result = nullptr;

		for (auto&& node : m_nodes)
		{
			auto&& input_pins = node->get_input_pins();
			auto&& output_pins = node->get_output_pins();

			auto&& input_it = std::find_if(begin(input_pins), end(input_pins),
				founder);
			auto&& output_it = std::find_if(begin(output_pins),
				end(output_pins), founder);

			if (input_it != end(input_pins))
			{
				result = *input_it;
				break;
			}
			else if (output_it != end(output_pins))
			{
				result = *output_it;
				break;
			}
		}

		return result;
	}

	//*************************************************************************

	void Blueprint::set_context(executor_pointer executor)
	{
		m_context = std::move(executor);
	}

	//*************************************************************************

	const IExecutor* Blueprint::get_context() const
	{
		return m_context.get();
	}

	//*************************************************************************

	std::size_t Blueprint::generate_id()
	{
		++m_last_id;

		std::cout << "Generated id - " << m_last_id << '\n';

		return m_last_id;
	}

	//*************************************************************************

} // namespace node_editor