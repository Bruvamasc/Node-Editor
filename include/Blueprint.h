#pragma once

#include "INode.h"
#include "Executor.h"

#include <vector>
#include <map>
#include <span>

namespace node_editor
{
	class Blueprint final
	{
	public:
		using node_pointer = std::unique_ptr<INode>;
		using nodes_container = std::vector<node_pointer>;
		using nodes_container_view = std::span<node_pointer>;
		using executor_pointer = std::unique_ptr<IExecutor>;

		Blueprint() = default;

		Blueprint(const Blueprint& other) = default;
		Blueprint& operator=(const Blueprint& other) = default;

		Blueprint(Blueprint&& other) = default;
		Blueprint& operator=(Blueprint&& other) = default;

		~Blueprint() {};

		void start();
		void stop();
		void reset();

		INode* add_node(node_pointer node);
		INode* add_node(const INode& node);
		INode* add_node(INode&& node);

		void remove_node(const INode& node);
		void remove_node(std::size_t node_id);

		nodes_container& get_nodes();
		const nodes_container& get_nodes() const;

		INode* find_node(std::size_t node_id);
		const INode* find_node(std::size_t node_id) const;

		IPin* find_pin(std::size_t pin_id);
		const IPin* find_pin(std::size_t pin_id) const;

		void set_context(executor_pointer executor);
		const IExecutor* get_context() const;

	private:
		std::size_t generate_id();

		nodes_container m_nodes;
		executor_pointer m_context{ nullptr };
		std::size_t m_last_id{ 0 };
	};

} // namespace node_editor