#pragma once

#include "type_traits.h"

#include <string_view>
#include <type_traits>
#include <memory>
#include <any>
#include <vector>
#include <algorithm>
#include <span>

namespace node_editor
{
	class INode;

	struct pin_info
	{
		std::string_view type_name;
		std::size_t type_hash;
	};

	class IPin
	{
	public:
		enum class connection_type
		{
			input,
			output
		};

		enum class behavior_type
		{
			provider,
			receiver
		};

		virtual ~IPin() = default;

		virtual void set_owner_node(INode& node) = 0;
		virtual INode* get_owner_node() const = 0;

		virtual void set_id(std::size_t id) = 0;
		virtual std::size_t get_id() const = 0;

		virtual bool set_raw_value(const std::any& data) = 0;

		virtual void set_connection_type(connection_type type) = 0;
		virtual connection_type get_connection_type() const = 0;

		virtual void set_behavior_type(behavior_type type) = 0;
		virtual behavior_type get_behavior_type() const = 0;

		virtual void set_name(std::string_view name) = 0;
		virtual std::string_view get_name() const = 0;

		virtual void set_description(std::string_view description) = 0;
		virtual std::string_view get_description() const = 0;

		virtual void add_link(IPin& pin) = 0;
		virtual void remove_link(const IPin& pin) = 0;
		virtual void remove_links() = 0;
		virtual std::span<IPin*> get_links() = 0;
		virtual std::span<const IPin* const> get_links() const = 0;
		virtual bool is_has_links() const = 0;
		virtual bool is_has_link(const IPin& pin) const = 0;

		virtual std::size_t get_type_hash() const = 0;
		virtual std::string_view get_type_name() const = 0;
		virtual pin_info get_type_info() const = 0;

		virtual std::unique_ptr<IPin> clone() const = 0;
	};

	//*************************************************************************

	

} // namespace node_editor