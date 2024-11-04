#pragma once

#include "INode.h"
#include "Pin.h"
#include "Pin_link.h"

#include <string>
#include <string_view>
#include <array>

namespace node_editor
{
	//*************************************************************************
	//									NODE VALUE
	//*************************************************************************

	template<typename T>
	class Node_value final : public INode
	{
	public:
		using value_type = T;

		Node_value(std::size_t id);

		template<typename U>
		Node_value(std::size_t id, U&& value);

		Node_value(const Node_value&) = default;
		Node_value& operator=(const Node_value&) = default;

		Node_value(Node_value&&) = default;
		Node_value& operator=(Node_value&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<value_type> m_input_pin{ 3, *this };
		Pin<value_type> m_output_pin{ 4, *this };

		mutable std::array<IPin*, 2> m_input_pins{ &m_enter_pin, &m_input_pin };
		mutable std::array<IPin*, 2> m_output_pins{ &m_exit_pin, &m_output_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE OPERATOR BRANCH
	//*************************************************************************

	template<typename T, typename OpT>
	class Node_operator final : public INode
	{
	public:
		using value_type = T;
		using operator_type = std::remove_cv_t<OpT>;

		Node_operator(std::size_t id);

		Node_operator(const Node_operator&) = default;
		Node_operator& operator=(const Node_operator&) = default;

		Node_operator(Node_operator&&) = default;
		Node_operator& operator=(Node_operator&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;

		operator_type m_operator;

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<value_type> m_lhs_input_pin{ 3, *this };
		Pin<value_type> m_rhs_input_pin{ 4, *this };
		Pin<value_type> m_output_pin{ 5, *this };

		mutable std::array<IPin*, 3> m_input_pins{ &m_enter_pin,
			&m_lhs_input_pin, &m_rhs_input_pin };
		mutable std::array<IPin*, 2> m_output_pins{ &m_exit_pin, 
			&m_output_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE TO STRING
	//*************************************************************************

	class Node_to_string final : public INode
	{
	public:
		Node_to_string(std::size_t id);

		Node_to_string(const Node_to_string&) = default;
		Node_to_string& operator=(const Node_to_string&) = default;

		Node_to_string(Node_to_string&&) = default;
		Node_to_string& operator=(Node_to_string&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<any> m_input_pin{ 3, *this };
		Pin<std::string> m_output_pin{ 4, *this };

		mutable std::array<IPin*, 2> m_input_pins{ &m_enter_pin, 
			&m_input_pin };
		mutable std::array<IPin*, 2> m_output_pins{ &m_exit_pin, 
			&m_output_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE BRANCH
	//*************************************************************************

	class Node_branch final : public INode
	{
	public:
		Node_branch(std::size_t id);

		Node_branch(const Node_branch&) = default;
		Node_branch& operator=(const Node_branch&) = default;

		Node_branch(Node_branch&&) = default;
		Node_branch& operator=(Node_branch&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_true_pin{ 2, *this };
		Pin<flow> m_exit_false_pin{ 3, *this };

		Pin<bool> m_input_pin{ 4, *this };

		mutable std::array<IPin*, 2> m_input_pins{ &m_enter_pin, &m_input_pin };
		mutable std::array<IPin*, 2> m_output_pins{ &m_exit_true_pin,
			&m_exit_false_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE PRINT BRANCH
	//*************************************************************************

	class Node_print final : public INode
	{
	public:
		Node_print(std::size_t id);

		Node_print(const Node_print&) = default;
		Node_print& operator=(const Node_print&) = default;

		Node_print(Node_print&&) = default;
		Node_print& operator=(Node_print&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<std::string> m_input_pin{ 3, *this };

		mutable std::array<IPin*, 2> m_input_pins{ &m_enter_pin, &m_input_pin };
		mutable std::array<IPin*, 1> m_output_pins{ &m_exit_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE DO ONCE BRANCH
	//*************************************************************************

	class Node_do_once final : public INode
	{
	public:
		Node_do_once(std::size_t id);

		Node_do_once(const Node_do_once&) = default;
		Node_do_once& operator=(const Node_do_once&) = default;

		Node_do_once(Node_do_once&&) = default;
		Node_do_once& operator=(Node_do_once&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;
		bool m_is_completed{ false };

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		mutable std::array<IPin*, 1> m_input_pins{ &m_enter_pin };
		mutable std::array<IPin*, 1> m_output_pins{ &m_exit_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE DO N BRANCH
	//*************************************************************************

	class Node_do_n final : public INode
	{
	public:
		Node_do_n(std::size_t id);

		Node_do_n(const Node_do_n&) = default;
		Node_do_n& operator=(const Node_do_n&) = default;

		Node_do_n(Node_do_n&&) = default;
		Node_do_n& operator=(Node_do_n&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;
		bool m_is_completed{ false };
		int m_count{ 0 };

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<int> m_input_pin{ 3, *this };

		mutable std::array<IPin*, 2> m_input_pins{ &m_enter_pin, &m_input_pin };
		mutable std::array<IPin*, 1> m_output_pins{ &m_exit_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//								NODE FOR LOOP INDEX BRANCH
	//*************************************************************************

	class Node_for_loop_index final : public INode
	{
	public:
		Node_for_loop_index(std::size_t id);

		Node_for_loop_index(const Node_for_loop_index&) = default;
		Node_for_loop_index& operator=(const Node_for_loop_index&) = default;

		Node_for_loop_index(Node_for_loop_index&&) = default;
		Node_for_loop_index& operator=(Node_for_loop_index&&) = default;

		Pin<flow>* execute(IExecutor& visitor) override;

		void evaluate_pin(const IExecutor& visitor,
			const IPin& pin) const override;

		void set_id(std::size_t id) override;
		std::size_t get_id() const override;

		void set_name(std::string_view name) override;
		std::string_view get_name() const override;

		void set_description(std::string_view description) override;
		std::string_view get_description() const override;

		Pin_link_result is_can_link_pins(const IPin& receiver,
			const IPin& provider) const override;

		Pin_link_result is_can_unlink_pins(const IPin& receiver,
			const IPin& provider) const override;

		std::span<IPin*> get_input_pins() override;
		std::span<const IPin* const> get_input_pins() const override;

		std::span<IPin*> get_output_pins() override;
		std::span<const IPin* const> get_output_pins() const override;

		std::unique_ptr<INode> clone() const override;

	private:
		std::size_t m_id;
		std::string m_name;
		std::string m_description;
		bool m_is_first{ true };
		int m_index{ 0 };

		Pin<flow> m_enter_pin{ 1, *this };
		Pin<flow> m_exit_pin{ 2, *this };

		Pin<int> m_start_index_pin{ 3, *this };
		Pin<int> m_end_index_pin{ 4, *this };

		Pin<flow> m_loop_body_pin{ 5, *this };
		Pin<int> m_current_index_pin{ 6, *this };

		mutable std::array<IPin*, 3> m_input_pins{ &m_enter_pin,
			&m_start_index_pin, &m_end_index_pin };
		mutable std::array<IPin*, 3> m_output_pins{ &m_loop_body_pin,
			&m_current_index_pin, &m_exit_pin };
	};

	//*************************************************************************



	//*************************************************************************
	//							NODE VALUE IMPLEMENTATION
	//*************************************************************************

	template<typename T>
	inline Node_value<T>::Node_value(std::size_t id)
		:m_id{ id },
		m_name{ "Value" },
		m_description{ "Value node" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}
	}

	//*************************************************************************

	template<typename T>
	template<typename U>
	inline Node_value<T>::Node_value(std::size_t id, U&& value)
		:m_id{ id },
		m_name{ "Value" },
		m_description{ "Value node" },
		m_input_pin{ 3, *this, std::forward<U>(value) },
		m_output_pin{ 4, *this, m_input_pin.get_value() }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}
	}

	//*************************************************************************

	template<typename T>
	inline Pin<flow>* Node_value<T>::execute(IExecutor& visitor)
	{
		auto&& pin_value = m_input_pin.get_value();

		m_output_pin.set_value(pin_value);

		return &m_exit_pin;
	}

	//*************************************************************************

	template<typename T>
	inline void Node_value<T>::evaluate_pin(const IExecutor& visitor,
		const IPin& pin) const
	{
		if (&pin != &m_output_pin
			|| !m_input_pin.is_has_links()) return;

		auto&& link_pin = m_input_pin.get_links().front();

		if (!link_pin->get_owner_node()) return;

		auto&& owner_node = link_pin->get_owner_node();

		owner_node->evaluate_pin(visitor, *link_pin);
	}

	//*************************************************************************

	template<typename T>
	inline void Node_value<T>::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	template<typename T>
	inline std::size_t Node_value<T>::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	template<typename T>
	inline void Node_value<T>::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	template<typename T>
	inline std::string_view Node_value<T>::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	template<typename T>
	inline void Node_value<T>::set_description(std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	template<typename T>
	inline std::string_view Node_value<T>::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	template<typename T>
	inline Pin_link_result Node_value<T>::is_can_link_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	template<typename T>
	inline Pin_link_result Node_value<T>::is_can_unlink_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	template<typename T>
	inline std::span<IPin*> Node_value<T>::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	template<typename T>
	inline std::span<const IPin* const> Node_value<T>::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	template<typename T>
	inline std::span<IPin*> Node_value<T>::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	template<typename T>
	inline std::span<const IPin* const> Node_value<T>::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	template<typename T>
	inline std::unique_ptr<INode> Node_value<T>::clone() const
	{
		return std::make_unique<Node_value<T>>(*this);
	}

	//*************************************************************************



	//*************************************************************************
	//							NODE OPERATOR IMPLEMENTATION
	//*************************************************************************

	template<typename T, typename OpT>
	inline Node_operator<T, OpT>::Node_operator(std::size_t id)
		:m_id{ id },
		m_name{ "Operator" },
		m_description{ "Operator node" }
	{
		for (auto&& pin : m_input_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::receiver);
			pin->set_connection_type(IPin::connection_type::input);
		}

		for (auto&& pin : m_output_pins)
		{
			pin->set_behavior_type(IPin::behavior_type::provider);
			pin->set_connection_type(IPin::connection_type::output);
		}
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline Pin<flow>* Node_operator<T, OpT>::execute(
		IExecutor& visitor)
	{
		auto&& lhs_pin_value = m_lhs_input_pin.get_value();
		auto&& rhs_pin_value = m_rhs_input_pin.get_value();

		auto&& value = m_operator(lhs_pin_value, rhs_pin_value);

		m_output_pin.set_value(value);

		return &m_exit_pin;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline void Node_operator<T, OpT>::evaluate_pin(
		const IExecutor& visitor, const IPin& pin) const
	{
		if (&pin != &m_output_pin) return;

		if (m_lhs_input_pin.is_has_link_pin())
		{
			auto&& link_pin = m_lhs_input_pin.get_link_pin();

			if (!link_pin->get_owner_node()) return;

			auto&& owner_node = link_pin->get_owner_node();

			owner_node->evaluate_pin(visitor, *link_pin);
		}

		if (m_rhs_input_pin.is_has_link_pin())
		{
			auto&& link_pin = m_rhs_input_pin.get_link_pin();

			if (!link_pin->get_owner_node()) return;

			auto&& owner_node = link_pin->get_owner_node();

			owner_node->evaluate_pin(visitor, *link_pin);
		}
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline void Node_operator<T, OpT>::set_id(std::size_t id)
	{
		m_id = id;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::size_t Node_operator<T, OpT>::get_id() const
	{
		return m_id;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline void Node_operator<T, OpT>::set_name(std::string_view name)
	{
		m_name = name;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::string_view Node_operator<T, OpT>::get_name() const
	{
		return m_name;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline void Node_operator<T, OpT>::set_description(
		std::string_view description)
	{
		m_description = description;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::string_view Node_operator<T, OpT>::get_description() const
	{
		return m_description;
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline Pin_link_result Node_operator<T, OpT>::is_can_link_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline Pin_link_result Node_operator<T, OpT>::is_can_unlink_pins(
		const IPin& receiver, const IPin& provider) const
	{
		return can_pins_link(receiver, provider);
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::span<IPin*> Node_operator<T, OpT>::get_input_pins()
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::span<const IPin* const>
		Node_operator<T, OpT>::get_input_pins() const
	{
		return std::span{ m_input_pins };
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::span<IPin*> Node_operator<T, OpT>::get_output_pins()
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::span<const IPin* const>
		Node_operator<T, OpT>::get_output_pins() const
	{
		return std::span{ m_output_pins };
	}

	//*************************************************************************

	template<typename T, typename OpT>
	inline std::unique_ptr<INode> Node_operator<T, OpT>::clone() const
	{
		return std::make_unique<Node_operator<T, OpT>>(*this);
	}

	//*************************************************************************

} // namespace node_editor