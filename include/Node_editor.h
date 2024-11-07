#pragma once

//#include "my/Node_pin.h"
//#include "my/Node.h"
//#include "my/Execution_context.h"
//#include "my/Blueprint.h"
//#include "my/widgets/Menu_bar.h"
//#include "my/Renderer.h"
//#include "my/Renderer_utility.h"
//#include "my/INode_factory.h"
//#include "my/widgets/IWidget.h"
//#include "my/widgets/Panel.h"
//
//#include "support_utility.h"
//
//#include <imgui_node_editor.h>
//
//#include <map>
//#include <string>
//#include <string_view>
//#include <memory>

#include "fwd.h"

#include "Renderer.h"

namespace node_editor
{
	class Node_editor final //: public std::enable_shared_from_this<Node_editor>
	{
	public:
		Node_editor();

		Context_data* get_context_data();
		const Context_data* get_context_data() const;

		Render_context* get_render_context();
		const Render_context* get_render_context() const;

	private:
		Context_data m_context_data;
		Render_context m_render_context;
	};

} // namespace node_editor