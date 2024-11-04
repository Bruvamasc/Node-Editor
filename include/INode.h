#pragma once

#include "Pin.h"
#include "Pin_link.h"
#include "types_library.h"

#include <string_view>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <span>

namespace node_editor
{
	class IExecutor;

	class INode
	{
	public:
		virtual ~INode() = default;

		virtual Pin<flow>* execute(IExecutor& visitor) = 0;

		virtual void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const = 0;

		virtual void set_id(std::size_t id) = 0;
		virtual std::size_t get_id() const = 0;

		virtual void set_name(std::string_view name) = 0;
		virtual std::string_view get_name() const = 0;

		virtual void set_description(std::string_view description) = 0;
		virtual std::string_view get_description() const = 0;

		virtual Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const = 0;

		virtual Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const = 0;

		virtual std::span<IPin*> get_input_pins() = 0;
		virtual std::span<const IPin* const> get_input_pins() const = 0;

		virtual std::span<IPin*> get_output_pins() = 0;
		virtual std::span<const IPin* const> get_output_pins() const = 0;

		virtual std::unique_ptr<INode> clone() const = 0;
	};

	//*************************************************************************

	/*inline void swap(Node& lhs, Node& rhs)
		noexcept(noexcept(std::is_nothrow_swappable_v<Node>))
	{
		lhs.swap(rhs);
	}*/

	//*************************************************************************

	/*Pin_link_result link_pins(INode_pin& receiver, INode_pin& provider)
	{
		auto&& receiver_node = receiver.get_owner_node();

		if (receiver_node)
		{
			return receiver_node->link_pins(receiver, provider);
		}

		return Pin_link_result{ false, "Receiver node has not owner node!" };
	}*/

	//*************************************************************************

} // namespace node_editor