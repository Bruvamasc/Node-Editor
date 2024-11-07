#include "widgets.h"

#include "INode.h"
#include "IPin.h"
#include "Node_editor.h"

#include "utility.h"

#include "imgui.h"

namespace node_editor
{
	void draw_node(INode& node)
	{
        auto&& render = get_render_context();

        if (!render) return;

        auto&& layers_manager = render->get_layers_manager();
        auto&& node_state = *render->get_node(node.get_id());

        auto&& cursor_pos = ImGui::GetCursorScreenPos();




        if (node_state.position.x == 0.f &&
            node_state.position.y == 0.f)
        {
            node_state.position = render->get_start_node_position();
        }

        // Позиция и размер нода
        ImVec2 node_min = node_state.position;
        ImVec2 node_max = ImVec2(node.position.x + node.size.x, node.position.y + node.size.y);

        // Рисуем прямоугольник для нода
        draw_list->AddRectFilled(node_min, node_max, IM_COL32(50, 50, 50, 255), 5.0f);
        draw_list->AddRect(node_min, node_max, IM_COL32(100, 100, 100, 255), 5.0f);

        // Рисуем текст с именем нода
        ImVec2 text_pos = node.position;
        text_pos.x += 10.0f;  // Отступ текста от края
        draw_list->AddText(text_pos, IM_COL32_WHITE, node.name.c_str());

        // Рисуем пины
        for (const auto& pin : node.pins) {
            ImVec2 pin_pos = node.position + pin.position;
            if (pin.isInput) {
                draw_list->AddCircleFilled(pin_pos, 5.0f, IM_COL32(200, 100, 100, 255));  // Входной пин - красный
            }
            else {
                draw_list->AddCircleFilled(pin_pos, 5.0f, IM_COL32(100, 200, 100, 255));  // Выходной пин - зеленый
            }
        }
	}

	//*************************************************************************

	void draw_pin(IPin& pin)
	{

	}

	//*************************************************************************

} // namespace node_editor



namespace node_editor { namespace detail {

    

    //*************************************************************************

} } // namespace node_editor::detail