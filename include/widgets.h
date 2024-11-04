#ifndef __NODE_EDITOR_WIDGETS_H__
#define __NODE_EDITOR_WIDGETS_H__

#include "INode.h"
#include "IPin.h"

#include <string>
#include <string_view>

namespace node_editor
{
	void draw_node(INode& node);
	void draw_pin(IPin& pin);

} // namespace node_editor

//*****************************************************************************

namespace node_editor { namespace detail {

	void draw_node_name(std::string_view name);

	void begin_node(std::size_t id);
	void end_node();

	void begin_pin()


} } // namespace node_editor::detail

#endif // !__NODE_EDITOR_WIDGETS_H__
