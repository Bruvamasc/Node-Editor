#ifndef __NODE_EDITOR_UTILITY_H__
#define __NODE_EDITOR_UTILITY_H__

#include "fwd.h"

namespace node_editor { namespace detail {

	extern node_editor::Node_editor* node_editor;
	extern node_editor::Context_data* context_data;
	extern node_editor::Render_context* render_context;

} } // namespace node_editor::detail

namespace node_editor
{
	void set_node_editor(Node_editor* rhs);
	Node_editor* get_node_editor();

	void set_context_data(Context_data* rhs);
	Context_data* get_context_data();

	void set_render_context(Render_context* rhs);
	Render_context* get_render_context();

} // namespace node_editor

#endif // !__NODE_EDITOR_UTILITY_H__
