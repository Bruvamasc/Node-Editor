#include "utility.h"

#include "Node_editor.h"
#include "Renderer.h"

namespace node_editor { namespace detail {

	node_editor::Node_editor* node_editor = nullptr;
	node_editor::Context_data* context_data = nullptr;
	node_editor::Render_context* render_context = nullptr;
} }

namespace node_editor
{
	void set_node_editor(Node_editor* rhs)
	{
		detail::node_editor = rhs;
	}

	//*************************************************************************

	Node_editor* get_node_editor()
	{
		return detail::node_editor;
	}

	//*************************************************************************

	void set_context_data(Context_data* rhs)
	{

	}

	//*************************************************************************

	Context_data* get_context_data()
	{
		return nullptr;
		//return detail::node_editor ? detail::node_editor->get_context_data() :
		//	nullptr;
	}

	//*************************************************************************

	void set_render_context(Render_context* rhs)
	{

	}

	//*************************************************************************

	Render_context* get_render_context()
	{
		return nullptr;
		//return detail::node_editor ? 
		//	detail::node_editor->get_render_context() : nullptr;
	}

	//*************************************************************************

} // namespace node_editor