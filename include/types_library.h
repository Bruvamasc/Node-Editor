#pragma once

//#include "INode_pin.h"
#include "type_traits.h"

#include <vector>
#include <list>
#include <map>
#include <any>

namespace node_editor
{
	//class INode_pin;

	struct flow final {};

	struct any final
	{
		bool is_has_value() const
		{
			return internal_pin;
		}
		
		operator bool() const
		{
			return is_has_value();
		}

		const INode_pin* internal_pin{ nullptr };
	};

	struct any_data final
	{
		type_info_base type_data;

		std::any data;
	};

	template<typename T>
	struct container final
	{
		using container_type = std::remove_cv_t<T>;
		using value_type = typename container_type::value_type;
		
		container_type internal_container;
	};

	template<typename T>
	struct container_pointer final
	{
		using container_type = std::remove_cv_t<T>;
		using value_type = typename container_type::value_type;

		bool is_has_value() const
		{
			return internal_container;
		}

		operator bool() const
		{
			return is_has_value();
		}

		container_type* internal_container{ nullptr };
	};

	//*************************************************************************

	template<>
	struct type_name<void*>
	{
		constexpr static std::string_view get_type_name() 
		{ 
			return std::string_view{ "void*" }; 
		}
	};

	//*************************************************************************

	template<>
	struct type_name<flow>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "flow" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<any>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "any" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<any_data>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "any_data" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<char>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "char" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<bool>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "bool" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<int>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "int" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<float>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "float" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<double>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "double" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<std::string>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "std::string" };
		}
	};

	//*************************************************************************

	template<>
	struct type_name<std::string_view>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "std::string_view" };
		}
	};

	//*************************************************************************

	template<typename T>
	struct type_name<container<T>>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "contaienr" };
		}
	};

	//*************************************************************************

	template<typename T>
	struct type_name<container_pointer<T>>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "contaienr_pointer" };
		}
	};

	//*************************************************************************

	template<typename T>
	struct type_name<std::vector<T>>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "std::vector" };
		}
	};

	//*************************************************************************

	template<typename T>
	struct type_name<std::list<T>>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "std::list" };
		}
	};

	//*************************************************************************

	template<typename T, typename U>
	struct type_name<std::map<T, U>>
	{
		constexpr static std::string_view get_type_name()
		{
			return std::string_view{ "std::map" };
		}
	};

	//*************************************************************************



	//*************************************************************************

	/*template<>
	class Node_pin<any> final : public INode_pin
	{
	public:
		using value_type = any;
		using reference = value_type&;
		using const_reference = const value_type&;
		using links_container = std::vector<INode_pin*>;

		Node_pin(std::size_t id, INode& owner_node)
			:m_id{ id }, m_owner_node{ &owner_node }
		{

		}

		Node_pin(std::size_t id, INode& owner_node,
			const_reference in_value)
			:m_value{ in_value }, m_id{ id }, m_owner_node{ &owner_node }
		{

		}

		Node_pin(std::size_t id, INode& owner_node,
			value_type&& in_value)
			:m_value{ std::move(in_value) },
			m_id{ id },
			m_owner_node{ &owner_node }
		{

		}

		Node_pin(const Node_pin<value_type>& rhs)
			:m_value{ rhs.m_value }, m_id{ rhs.m_id }
		{
			m_links.reserve(rhs.m_links.size());

			for (auto&& pin : rhs.m_links)
			{
				m_links.push_back(pin);
			}
		}

		Node_pin& operator=(const Node_pin<value_type>& rhs)
		{
			if (this != &rhs)
			{
				*this = Node_pin<value_type>{ rhs };
			}

			return *this;
		}

		Node_pin(Node_pin<value_type>&& rhs)
			:m_value{ std::move(rhs.m_value) }, m_id{ rhs.m_id },
			m_links{ std::move(rhs.m_links) }
		{

		}

		Node_pin& operator=(Node_pin<value_type>&& rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}

			return *this;
		}

		~Node_pin()
		{
			remove_links();
		}

		void set_owner_node(INode& node) override
		{
			m_owner_node = &node;
		}

		INode* get_owner_node() const override
		{
			return m_owner_node;
		}

		void set_id(std::size_t id) override
		{
			m_id = id;
		}

		std::size_t get_id() const override
		{
			return m_id;
		}

		bool set_raw_value(const std::any& data) override
		{
			auto&& value_ptr = std::any_cast<value_type>(&data);

			bool result = false;

			if (value_ptr)
			{
				m_value = *value_ptr;
			}

			return result;
		}

		void set_connection_type(connection_type type) override
		{
			m_connection_type = type;
		}

		connection_type get_connection_type() const override
		{
			return m_connection_type;
		}

		void set_behavior_type(behavior_type type) override
		{
			m_behavior_type = type;
		}

		behavior_type get_behavior_type() const override
		{
			return m_behavior_type;
		}

		void set_name(std::string_view name) override
		{
			m_name = name;
		}

		std::string_view get_name() const override
		{
			return m_name;
		}

		void set_description(std::string_view description) override
		{
			m_description = description;
		}

		std::string_view get_description() const override
		{
			return m_description;
		}

		INode_pin* find_pin(const INode_pin& pin) const
		{
			using std::begin;
			using std::end;

			INode_pin* result{ nullptr };

			auto&& it = std::find(begin(m_links), end(m_links), &pin);

			if (it != end(m_links))
			{
				result = *it;
			}

			return result;
		}

		void add_link(INode_pin& pin) override
		{
			if (this == &pin) return;

			if (m_behavior_type == behavior_type::receiver)
			{
				m_value.internal_pin = &pin;
			}

			m_links.push_back(&pin);
		}

		void remove_link(const INode_pin& pin) override
		{
			using std::begin;
			using std::end;

			auto&& it = std::remove(begin(m_links), end(m_links), &pin);

			if (it != end(m_links))
			{
				auto pin = *it;
				*it = nullptr;

				if (pin)
				{
					pin->remove_link(*this);
				}
			}

			m_links.erase(it, end(m_links));

			if (m_value.internal_pin == &pin)
			{
				m_value.internal_pin = nullptr;
			}
		}

		void remove_links() override
		{
			for (auto&& pin : m_links)
			{
				auto pin_ptr = pin;
				pin = nullptr;

				if (pin_ptr)
				{
					pin_ptr->remove_link(*this);
				}
			}

			m_links.clear();
		}

		span<INode_pin*> get_links() override
		{
			return span<INode_pin*>{ m_links };
		}

		span<const INode_pin* const> get_links() const override
		{
			return span<const INode_pin* const>{ m_links };
		}

		bool is_has_links() const override
		{
			return !m_links.empty();
		}

		bool is_has_link(const INode_pin& pin) const override
		{
			auto&& linked_pin = find_pin(pin);

			return linked_pin;
		}

		std::size_t get_type_hash() const override
		{
			if (m_value.is_has_value())
			{
				return m_value.internal_pin->get_type_hash();
			}

			return type_info<value_type>::get_hash();
		}

		std::string_view get_type_name() const override
		{
			if (m_value.is_has_value())
			{
				return m_value.internal_pin->get_type_name();
			}

			return type_info<value_type>::get_name();
		}

		pin_info get_type_info() const override
		{
			if (m_value.is_has_value())
			{
				return m_value.internal_pin->get_type_info();	
			}

			return pin_info
			{
				get_type_name(),
				get_type_hash()
			};
		}

		std::unique_ptr<INode_pin> clone() const override
		{
			return std::make_unique<Node_pin>(*this);
		}

		void set_value(const_reference in_value)
		{
			m_value = in_value;
		}

		void set_value(value_type&& in_value)
		{
			m_value = std::move(in_value);
		}

		reference get_value()
		{
			return m_value;
		}

		const_reference get_value() const
		{
			return m_value;
		}

		void swap(Node_pin<value_type>& rhs)
			noexcept(noexcept(std::is_nothrow_swappable_v<value_type>))
		{
			using std::swap;

			swap(m_value, rhs.m_value);
			swap(m_id, rhs.m_id);
			swap(m_connection_type, rhs.m_connection_type);
			swap(m_behavior_type, rhs.m_behavior_type);
			swap(m_name, rhs.m_name);
			swap(m_links, rhs.m_links);
			swap(m_owner_node, rhs.m_owner_node);
		}

	private:


	private:
		value_type m_value;
		std::size_t m_id;
		connection_type m_connection_type{ connection_type::output };
		behavior_type m_behavior_type{ behavior_type::provider };
		std::string m_name;
		std::string m_description;
		links_container m_links;
		INode* m_owner_node{ nullptr };
	};*/

} // namespace node_editor











//template<>
//constexpr std::string_view get_type_name<void*>()
//{
//	return std::string_view{ "void*" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<flow>()
//{
//	return std::string_view{ "flow" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<any>()
//{
//	return std::string_view{ "any" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<char>()
//{
//	return std::string_view{ "char" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<bool>()
//{
//	return std::string_view{ "bool" };
//}
//
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<int>()
//{
//	return std::string_view{ "int" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<float>()
//{
//	return std::string_view{ "float" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<double>()
//{
//	return std::string_view{ "double" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<std::string>()
//{
//	return std::string_view{ "std::string" };
//}
//
////*************************************************************************
//
//template<>
//constexpr std::string_view get_type_name<std::string_view>()
//{
//	return std::string_view{ "std::string_view" };
//}