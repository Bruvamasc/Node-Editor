#ifndef __NODE_EDTIOR_EXECUTOR_H__
#define __NODE_EDITOR_EXECITOR_H__

#include "IExecutor.h"

namespace node_editor
{
	class Executor final : public IExecutor
	{
	public:
		void reset();

		step_result start(flow_pin& entry_point);
		step_result step();
		void stop();

		step_result execute(flow_pin& entry_point);

		INode* get_current_node();
		const INode* get_current_node() const;

		INode* get_next_node();
		const INode* get_next_node() const;

		flow_pin* get_current_flow_pin();
		const flow_pin* get_current_flow_pin() const;

		step_result get_last_step_result() const;

		std::size_t get_step_count() const;

	private:
		step_result set_step_result(step_result result);

	private:
		std::vector<flow_pin*> m_callstack;
		INode* m_current_node{ nullptr };
		flow_pin* m_current_flow_pin{ nullptr };
		step_result m_last_result{ step_result::done };
		std::size_t m_step_count{ 0 };
	};

	//*************************************************************************



	//*************************************************************************
	//							SUPPORT FUNCTIONS
	//*************************************************************************

	std::string to_string(step_result result);

	//*************************************************************************

} // namespace node_editor

#endif // !__NODE_EDITOR_EXECUTOR_H__
