#pragma once

#include <string_view>
#include <utility>

namespace node_editor
{
	template<typename>
	constexpr std::string_view get_type_name() = delete;

	template<typename T>
	struct type_name;

	struct type_info_base
	{
		std::string_view type_name;
		std::size_t type_hash;
	};

	template<typename T>
	struct type_info
	{
	public:
		static constexpr std::string_view get_name()
		{
			return type_name<T>::get_type_name();
		}

		static constexpr std::size_t get_hash()
		{
			return std::hash<decltype(get_name())>{}(get_name());
		}
	};

	//*************************************************************************



	//*************************************************************************

	inline bool operator==(const type_info_base& lhs, const type_info_base& rhs)
	{
		return lhs.type_name == rhs.type_name && 
			lhs.type_hash == rhs.type_hash;
	}

	//*************************************************************************

	inline bool operator!=(const type_info_base& lhs, const type_info_base& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	template<typename T>
	inline bool operator==(const type_info_base& lhs, const type_info<T>& rhs)
	{
		return lhs.type_name == rhs.get_name() &&
			lhs.type_hash == rhs.get_hash();
	}

	//*************************************************************************

	template<typename T>
	inline bool operator==(const type_info<T>& lhs, const type_info_base& rhs)
	{
		return rhs == lhs;
	}

	//*************************************************************************

	template<typename T>
	inline bool operator!=(const type_info_base& lhs, const type_info<T>& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	template<typename T>
	inline bool operator!=(const type_info<T>& lhs, const type_info_base& rhs)
	{
		return !(rhs == lhs);
	}

	//*************************************************************************

} // namespace node_editor