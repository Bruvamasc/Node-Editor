#include "Node_editor.h"

#include "utility.h"

namespace node_editor
{
	Node_editor::Node_editor()
	{
		set_node_editor(this);
	}

	//*************************************************************************

	Context_data* Node_editor::get_context_data()
	{
		return &m_context_data;
	}

	//*************************************************************************

	const Context_data* Node_editor::get_context_data() const
	{
		return &m_context_data;
	}

	//*************************************************************************

	Render_context* Node_editor::get_render_context()
	{
		return &m_render_context;
	}

	//*************************************************************************

	const Render_context* Node_editor::get_render_context() const
	{
		return &m_render_context;
	}

	//*************************************************************************

} // namespace node_editor