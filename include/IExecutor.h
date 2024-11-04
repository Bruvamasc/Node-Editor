#pragma once

#include "types_library.h"
#include "Pin.h"

#include <memory>
#include <vector>
#include <string>

namespace node_editor
{
	class INode;

	enum class step_result
	{
		success,
		done,
		error
	};

	//*************************************************************************

	class IExecutor
	{
	public:
		using flow_pin = Pin<flow>;

		virtual ~IExecutor() = default;

		virtual void reset() = 0;

		virtual step_result start(flow_pin& entry_point) = 0;
		virtual step_result step() = 0;
		virtual void stop() = 0;

		virtual step_result execute(flow_pin& entry_point) = 0;

		virtual INode* get_current_node() = 0;
		virtual const INode* get_current_node() const = 0;

		virtual INode* get_next_node() = 0;
		virtual const INode* get_next_node() const = 0;

		virtual flow_pin* get_current_flow_pin() = 0;
		virtual const flow_pin* get_current_flow_pin() const = 0;

		virtual step_result get_last_step_result() const = 0;

		virtual std::size_t get_step_count() const = 0;
	};

} // namespace node_editor