#include "node_editor/Executor.h"

namespace node_editor
{
	void Executor::reset()
	{

	}

	//*************************************************************************

	step_result Executor::start(flow_pin& entry_point)
	{
		m_callstack.resize(0);
		m_current_node = entry_point.get_owner_node();
		m_current_flow_pin = &entry_point;
		m_step_count = 0;

		if (!m_current_node || m_current_flow_pin->get_id() == 0)
		{
			return set_step_result(step_result::error);
		}

		return set_step_result(step_result::success);
	}

	//*************************************************************************

	step_result Executor::step()
	{
		//if (m_last_result != step_result::success) return m_last_result;

		//auto&& current_flow_pin = m_current_flow_pin;

		//m_current_node = nullptr;
		//m_current_flow_pin = nullptr;

		//if (m_current_flow_pin->get_id() == 0 &&
		//	m_callstack.empty())
		//{
		//	return set_step_result(step_result::done);
		//}

		//if (current_flow_pin->get_type_hash() != type_info<flow>::get_hash() &&
		//	current_flow_pin->get_type_name() != type_info<flow>::get_name())
		//{
		//	return set_step_result(step_result::error);
		//}

		//m_current_node = current_flow_pin->get_owner_node();

		//++m_step_count;

		//if (m_context_monitor)
		//{
		//	//m_context_monitor->on_pre_step(*this);
		//}

		////auto&& next = current_flow_pin->get_owner_node()->execute(*this, *current_flow_pin);

		////auto&& next = current_flow_pin->get_owner_node()->execute();

		//Node_pin<flow>* next;

		//if (next && next->is_has_link_pin() &&
		//	next->get_link_pin()->get_type_hash() == type_info<flow>::get_hash() &&
		//	next->get_link_pin()->get_type_name() == type_info<flow>::get_name())
		//{
		//	m_current_flow_pin = next;
		//}
		//else if (!m_callstack.empty())
		//{
		//	m_current_flow_pin = m_callstack.back();
		//	m_callstack.pop_back();
		//}

		//if (m_context_monitor)
		//{
		//	//m_context_monitor->on_post_step(*this);
		//}

		return set_step_result(step_result::success);
	}

	//*************************************************************************

	void Executor::stop()
	{
		if (m_last_result != step_result::success) return;

		m_current_node = nullptr;
		m_current_flow_pin = nullptr;
		m_callstack.clear();

		set_step_result(step_result::done);
	}

	//*************************************************************************

	step_result Executor::execute(flow_pin& entry_point)
	{
		start(entry_point);

		step_result result{ step_result::done };

		while (result != step_result::error)
		{
			result = step();
		}

		return result;
	}

	//*************************************************************************

	INode* Executor::get_current_node()
	{
		return m_current_node;
	}

	//*************************************************************************

	const INode* Executor::get_current_node() const
	{
		return m_current_node;
	}

	//*************************************************************************

	INode* Executor::get_next_node()
	{
		INode* result = nullptr;

		/*if (m_current_flow_pin &&
			m_current_flow_pin->is_has_link_pin() &&
			m_current_flow_pin->get_link_pin())
		{
			result = m_current_flow_pin->get_link_pin()->get_owner_node();
		}
		else if (m_current_flow_pin)
		{
			result = m_current_flow_pin->get_owner_node();
		}*/

		return result;
	}

	//*************************************************************************

	const INode* Executor::get_next_node() const
	{
		const INode* result = nullptr;

		/*if (m_current_flow_pin &&
			m_current_flow_pin->is_has_link_pin() &&
			m_current_flow_pin->get_link_pin())
		{
			result = m_current_flow_pin->get_link_pin()->get_owner_node();
		}
		else if (m_current_flow_pin)
		{
			result = m_current_flow_pin->get_owner_node();
		}*/

		return result;
	}

	//*************************************************************************

	typename Executor::flow_pin*
		Executor::get_current_flow_pin()
	{
		return m_current_flow_pin;
	}

	//*************************************************************************

	const typename Executor::flow_pin*
		Executor::get_current_flow_pin() const
	{
		return m_current_flow_pin;
	}

	//*************************************************************************

	step_result Executor::get_last_step_result() const
	{
		return m_last_result;
	}

	//*************************************************************************

	std::size_t Executor::get_step_count() const
	{
		return m_step_count;
	}

	//*************************************************************************

	step_result Executor::set_step_result(step_result result)
	{
		m_last_result = result;

		return result;
	}

	//*************************************************************************



	//*************************************************************************
	//						SUPPORT FUNCTIONS IMPLEMENTATION
	//*************************************************************************

	std::string to_string(step_result result)
	{
		switch (result)
		{
		case step_result::success:
			return std::string{ "success" };
		case step_result::done:
			return std::string{ "done" };
		case step_result::error:
			return std::string{ "error" };
		default:
			return std::string{ "unknow step result" };
		}
	}

	//*************************************************************************

} // namespace node_editor