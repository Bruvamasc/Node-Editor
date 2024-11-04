#ifndef __NODE_EDITOR_PIN_H__
#define __NODE_EDITOR_PIN_H__

#include "IPin.h"

namespace node_editor
{
	template<typename T>
	class Pin final : public IPin
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using links_container = std::vector<IPin*>;

		Pin(std::size_t id, INode& owner_node)
			:m_id{ id }, m_owner_node{ &owner_node }
		{

		}

		Pin(std::size_t id, INode& owner_node,
			const_reference in_value)
			:m_value{ in_value }, m_id{ id }, m_owner_node{ &owner_node }
		{

		}

		Pin(std::size_t id, INode& owner_node,
			value_type&& in_value)
			:m_value{ std::move(in_value) },
			m_id{ id },
			m_owner_node{ &owner_node }
		{

		}

		Pin(const Pin<T>& rhs)
			:m_value{ rhs.m_value }, m_id{ rhs.m_id }
		{
			m_links.reserve(rhs.m_links.size());

			for (auto&& pin : rhs.m_links)
			{
				m_links.push_back(pin);
			}
		}

		Pin& operator=(const Pin<T>& rhs)
		{
			if (this != &rhs)
			{
				*this = Pin<T>{ rhs };
			}

			return *this;
		}

		Pin(Pin<T>&& rhs)
			:m_value{ std::move(rhs.m_value) }, m_id{ rhs.m_id },
			m_links{ std::move(rhs.m_links) }
		{

		}

		Pin& operator=(Pin<T>&& rhs)
		{
			if (this != &rhs)
			{
				swap(rhs);
			}

			return *this;
		}

		~Pin()
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
			auto&& value_ptr = std::any_cast<T>(&data);

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

		IPin* find_pin(const IPin& pin) const
		{
			using std::begin;
			using std::end;

			IPin* result{ nullptr };

			auto&& it = std::find(begin(m_links), end(m_links), &pin);

			if (it != end(m_links))
			{
				result = *it;
			}

			return result;
		}

		void add_link(IPin& pin) override
		{
			if (this == &pin) return;

			m_links.push_back(&pin);
		}

		void remove_link(const IPin& pin) override
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

		std::span<IPin*> get_links() override
		{
			return std::span{ m_links };
		}

		std::span<const IPin* const> get_links() const override
		{
			return std::span{ m_links };
		}

		bool is_has_links() const override
		{
			return !m_links.empty();
		}

		bool is_has_link(const IPin& pin) const override
		{
			auto&& linked_pin = find_pin(pin);

			return linked_pin;
		}

		std::size_t get_type_hash() const override
		{
			return type_info<value_type>::get_hash();
		}

		std::string_view get_type_name() const override
		{
			return type_info<value_type>::get_name();
		}

		pin_info get_type_info() const override
		{
			return pin_info
			{
				get_type_name(),
				get_type_hash()
			};
		}

		std::unique_ptr<IPin> clone() const override
		{
			return std::make_unique<Pin>(*this);
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

		void swap(Pin<T>& rhs)
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
	};

	//*************************************************************************

	template<typename T>
	void swap(Pin<T>& lhs, Pin<T>& rhs)
		noexcept(noexcept(std::is_nothrow_swappable_v<Pin<T>>))
	{
		lhs.swap(rhs);
	}

	//*************************************************************************

	template<typename T>
	Pin<T>* cast_to(IPin& pin)
	{
		return dynamic_cast<Pin<T>*>(&pin);
	}

	//*************************************************************************

	template<typename T>
	const Pin<T>* cast_to(const IPin& pin)
	{
		return dynamic_cast<const Pin<T>*>(&pin);
	}

	//*************************************************************************

	template<typename T>
	bool operator==(const pin_info& lhs, const type_info<T>& rhs)
	{
		return lhs.type_hash == rhs.get_hash() &&
			lhs.type_name == rhs.get_name();
	}

	//*************************************************************************

	template<typename T>
	bool operator==(const type_info<T>& lhs, const pin_info& rhs)
	{
		return rhs == lhs;
	}

	//*************************************************************************

	template<typename T>
	bool operator!=(const pin_info& lhs, const type_info<T>& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	template<typename T>
	bool operator!=(const type_info<T>& lhs, const pin_info& rhs)
	{
		return !(rhs == lhs);
	}

	//*************************************************************************

	inline bool operator==(const pin_info& lhs, const pin_info& rhs)
	{
		return lhs.type_hash == rhs.type_hash &&
			lhs.type_name == rhs.type_name;
	}

	//*************************************************************************

	inline bool operator!=(const pin_info& lhs, const pin_info& rhs)
	{
		return !(lhs == rhs);
	}

	//*************************************************************************

	inline bool is_input_pin(const IPin& pin)
	{
		return pin.get_connection_type() == IPin::connection_type::input;
	}

	//*************************************************************************

	inline bool is_output_pin(const IPin& pin)
	{
		return pin.get_connection_type() == IPin::connection_type::output;
	}

	//*************************************************************************

	inline bool is_receiver_pin(const IPin& pin)
	{
		return pin.get_behavior_type() == IPin::behavior_type::receiver;
	}

	//*************************************************************************

	inline bool is_provider_pin(const IPin& pin)
	{
		return pin.get_behavior_type() == IPin::behavior_type::provider;
	}

	//*************************************************************************

} // namespace node_editor

#endif // !__NODE_EDITOR_PIN_H__
