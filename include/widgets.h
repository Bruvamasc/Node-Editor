#ifndef __NODE_EDITOR_WIDGETS_H__
#define __NODE_EDITOR_WIDGETS_H__

#include "INode.h"
#include "IPin.h"
#include "Renderer.h"

#include <string>
#include <string_view>

namespace node_editor
{
	void draw_node(INode& node);
	void draw_pin(IPin& pin);

} // namespace node_editor

//*****************************************************************************

namespace node_editor { namespace detail {

	void begin_node(std::size_t id);
	void end_node();

	void begin_pin(std::size_t id);
	void end_pin();

} } // namespace node_editor::detail

#endif // !__NODE_EDITOR_WIDGETS_H__
