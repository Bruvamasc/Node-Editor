#pragma once

#include "IPin.h"
#include "types_library.h"

#include <string_view>
#include <string>

namespace node_editor
{
	class Pin_link_result final
	{
	public:
		Pin_link_result(bool result, std::string_view description = { "" })
			:m_result{ result }, m_description{ description }
		{

		}

		bool get_result() const
		{
			return m_result;
		}

		std::string_view get_description() const
		{
			return m_description;
		}

		operator bool() const
		{
			return m_result;
		}

	private:
		std::string m_description;
		bool m_result;
	};

	//*************************************************************************

	inline Pin_link_result can_pins_link(const IPin& lhs,
		const IPin& rhs)
	{
		//check owner nodes
		if (lhs.get_owner_node() == rhs.get_owner_node())
		{
			return Pin_link_result
			{
				false,
				"Pins are located on the same node!"
			};
		}

		//check types
		if (lhs.get_type_info() != rhs.get_type_info()
			&& !((lhs.get_type_info() == type_info<any>{})
				|| (rhs.get_type_info() == type_info<any>{})))
		{
			return Pin_link_result{ false, "Pins are different types!" };
		}

		//check input or output type
		if (lhs.get_connection_type() == rhs.get_connection_type() ||
			lhs.get_behavior_type() == rhs.get_behavior_type())
		{
			return Pin_link_result
			{
				false,
				"Pins of the same connection type or behavior type!"
			};
		}

		return Pin_link_result
		{
			true,
			"Pins are can links!"
		};
	}

	//*************************************************************************

	inline Pin_link_result try_link_pins(IPin& lhs, IPin& rhs)
	{
		auto&& result = can_pins_link(lhs, rhs);

		if (result)
		{
			lhs.add_link(rhs);
			rhs.add_link(lhs);

			//lhs.set_link_pin(rhs);
			//rhs.set_link_pin(lhs);

			return Pin_link_result
			{
				true,
				"Pins are linked!"
			};
		}

		return result;
	}

	//*************************************************************************

	inline Pin_link_result try_unlink_pins(IPin& lhs, IPin& rhs)
	{
		if (lhs.is_has_link(rhs) != rhs.is_has_link(lhs))
		{
			lhs.remove_link(rhs);
			rhs.remove_link(lhs);

			//lhs.remove_link_pin();
			//rhs.remove_link_pin();

			return Pin_link_result
			{
				true,
				"Pins are unlinked!"
			};
		}

		return Pin_link_result
		{
			false,
			"Pins are not linked, hence can not be unlinked!"
		};
	}

	//*************************************************************************

} // node_editor