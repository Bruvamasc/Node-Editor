//#include "chat_bot.h"
//
//#include <iostream>
//
//int main()
//{
//	try
//	{
//		bot::Chat_bot twitch_bot{};
//
//		twitch_bot.run();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << '\n';
//	}
//	catch (...)
//	{
//		std::cerr << "Unknow exception!\n";
//	}
//
//	return 0;
//}






//#include "Test_program.h"
//
//#include <iostream>
//
//int main()
//{
//	try
//	{
//		bot::Test_program twitch_bot{};
//
//		twitch_bot.run();
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << '\n';
//	}
//	catch (...)
//	{
//		std::cerr << "Unknow exception!\n";
//	}
//
//	return 0;
//}





#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "gui/gui_support_functions.h"


#include <vector>
#include <algorithm>
#include <iostream>


std::string find_word(std::string_view data)
{
    std::string result;
    result.reserve(data.size());

    for (auto&& elem : data)
    {
        if (elem != ' ' && elem != '\0')
        {
            result.push_back(elem);
        }
        else
        {
            break;
        }
    }

    result.shrink_to_fit();

    return result;
}


struct Giveaway_item
{
    std::string name;
    std::size_t id{ 1 };
    std::size_t count{ 1 };
    sf::Texture texture;
    std::string file_name;
};

struct Giveaway_member
{
    std::string name;
    std::size_t id;
};

struct Giveaway_winner
{
    std::string name;
    std::size_t id;
    std::size_t reward_id{ 0 };
};

struct Giveaway
{
    std::string name;
    std::string description;
    std::size_t id;

    std::string key_word;
    std::string start_message;
    std::string stop_message;
    std::string winner_message;
    std::string member_message;
    
    std::vector<Giveaway_member> members;
    std::vector<Giveaway_item> rewards;
    std::vector<Giveaway_winner> winners;

    Giveaway_item* active_reward{ nullptr };

    bool is_active{ false };
    bool is_completed{ false };

    void add_member(const Giveaway_member& member)
    {
        members.push_back(member);
    }

    auto&& remove_member(const Giveaway_member& member)
    {
        using std::begin;
        using std::end;

        auto&& founder = [&member](auto&& s_member)
            {
                return s_member.id == member.id;
            };

        auto&& it = std::remove_if(begin(members), end(members), founder);

        return members.erase(it, end(members));
    }

    void add_item(const Giveaway_item& item)
    {
        static std::size_t item_id = 1;

        rewards.push_back(item);
        rewards.back().id = item_id;
        ++item_id;
    }

    auto&& remove_item(const Giveaway_item& item)
    {
        using std::begin;
        using std::end;

        auto&& founder = [&item](auto&& s_item)
            {
                return s_item.name == item.name;
            };

        if (active_reward && active_reward == &item)
        {
            active_reward = nullptr;
        }

        auto&& it = std::remove_if(begin(rewards), end(rewards), founder);

        return rewards.erase(it, end(rewards));
    }

    void add_to_winners(const Giveaway_member& member, Giveaway_item& reward)
    {
        using std::begin;
        using std::end;

        auto&& founder = [&member](auto&& s_mem)
            {
                return s_mem.id == member.id;
            };

        auto&& it = std::find_if(begin(members), end(members), founder);

        if (it != end(members))
        {
            Giveaway_winner winner
            {
                it->name,
                it->id,
                reward.id
            };

            winners.push_back(winner);

            reward.count -= 1;
        }

        it = std::remove_if(it, end(members), founder);

        members.erase(it, end(members));
    }
};

struct Giveaways_manager
{
    std::vector<Giveaway> active_giveaways;
    std::vector<Giveaway> archive_giveaways;

    Giveaway* active_giveaway{ nullptr };

    void add_giveaway(const Giveaway& giveaway)
    {
        active_giveaways.push_back(giveaway);
    }

    auto&& remove_giveaway(const Giveaway& giveaway)
    {
        using std::begin;
        using std::end;

        auto&& founder = [&giveaway](auto&& s_giveaway)
			{
                return s_giveaway.id == giveaway.id;
			};

        auto&& it = std::remove_if(begin(active_giveaways), end(active_giveaways), founder);

        if (active_giveaway && active_giveaway->id == giveaway.id)
        {
            active_giveaway = nullptr;
        }

        return active_giveaways.erase(it, end(active_giveaways));
    }

    void add_to_archive(const Giveaway& giveaway)
    {
        archive_giveaways.push_back(giveaway);
    }

    auto&& remove_from_archive(const Giveaway& giveaway)
    {
        using std::begin;
        using std::end;

        auto&& founder = [&giveaway](auto&& s_giveaway)
            {
                return s_giveaway.id == giveaway.id;
            };

        auto&& it = std::remove_if(begin(archive_giveaways), end(archive_giveaways), founder);

        return archive_giveaways.erase(it, end(archive_giveaways));
    }

    
};

struct Item_editor_data
{
    std::string item_name_buffer;
    int item_count{ 1 };
    std::string item_file_name;
    sf::Texture item_image;
    unsigned int texture_id;
};

struct Item_viewer_data
{
    ImVec2 padding{ 5.f, 5.f };
    ImVec2 image_size{ 64.f, 64.f };
    ImVec4 background_color{ 0.3f, 0.3f, 0.3f, 1.f };
    float rounding{ 2.f };
};


struct Giveaway_viewer_data
{
    ImVec2 padding{ 5.f, 5.f };
    ImVec2 button_size{ 64.f, 64.f };
    ImVec4 background_color{ 0.3f, 0.3f, 0.3f, 1.f };
    float rounding{ 2.f };
};

struct Giveaway_editor_data
{
    std::string name_buffer;
    std::string description_buffer;
    int id;
    std::string key_word_buffer;
    std::string start_message_buffer;
    std::string stop_message_buffer;
    std::string winner_message_buffer;
    std::string member_message_buffer;

    Giveaway giveaway_buffer;
};

class Giveaway_widget final
{
public:
    Giveaway_widget(Giveaways_manager& manager)
        :m_giveaways{ manager }
    {
        init();
    }

    void draw(float dt) const
    {
        begin_widget();
        toolbar();
        giveaway_edit();
        item_edit();
        body();
        end_widget();
    }

private:
    void begin_widget() const
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
        ImGui::Begin("Giveaways");
    }

    void end_widget() const
    {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void begin_toolbar() const
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoScrollbar;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoNav;
        flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

        auto&& window_width = ImGui::GetWindowWidth();
        ImVec2 button_size = m_toolbar_button_size;
        ImVec2 paddings = m_toolbar_padding;
        ImVec2 toolbar_size = ImVec2{ window_width, button_size.y + paddings.y * 3.f };

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, paddings);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, m_toolbar_bg_color);
        ImGui::BeginChild("##toolbar", toolbar_size, false, flags);
    }

    void end_toolbar() const
    {
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void begin_workspace() const
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoNav;
        flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

        auto window_width = ImGui::GetWindowWidth();
        auto window_height = ImGui::GetContentRegionAvail().y;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_giveaway_viewer_data.padding);
        ImGui::BeginChild("##items_space", ImVec2{ window_width, window_height }, false, flags);
    }

    void end_workspace() const
    {
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    bool item_block(float width, const Giveaway_item& item) const
    {
        bool result = false;

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        auto&& padding = m_item_viewer_data.padding;
        auto&& image_size = m_item_viewer_data.image_size;
        auto count = std::to_string(item.count);

        auto name_text_size = ImGui::CalcTextSize(item.name.c_str(),
            item.name.c_str() + item.name.size());
        auto count_text_size = ImGui::CalcTextSize(count.c_str(),
            count.c_str() + count.size());

        auto block_height = padding.y * 3.f + name_text_size.y +
            count_text_size.y;

        block_height = std::max(block_height, image_size.y + padding.y * 2.f);

        auto start_pos = ImGui::GetCursorScreenPos();

        ImVec2 size
        {
            width,
            block_height
        };

        ImVec2 max_pos
        {
            start_pos.x + size.x,
            start_pos.y + size.y
        };

        ImGui::BeginGroup();

        ImGui::ItemSize(size, 0.f);
        if (!ImGui::ItemAdd(ImRect{ start_pos, max_pos }, 0))
        {
            ImGui::EndGroup();

            return false;
        }

        auto&& bg_color = m_item_viewer_data.background_color;
        auto&& rounding = m_item_viewer_data.rounding;

        window->DrawList->AddRectFilled(start_pos, max_pos,
            ImGui::ColorConvertFloat4ToU32(bg_color), rounding);

        auto pos = start_pos;

        pos.x += padding.x;
        pos.y += padding.y;

        ImGui::SetCursorScreenPos(pos);

        ImGui::Image(item.texture.getNativeHandle(), image_size);

        pos = start_pos;

        pos.x += padding.x * 2.f + image_size.x;
        pos.y += padding.y;

        ImGui::SetCursorScreenPos(pos);

        ImGui::Text(item.name.c_str());

        pos = start_pos;

        pos.x += padding.x * 2.f + image_size.x;
        pos.y += padding.y * 2.f + name_text_size.y;

        ImGui::SetCursorScreenPos(pos);

        ImGui::Text(count.c_str());

        ImGui::EndGroup();

        if (ImGui::IsItemHovered())
        {
            
        }

        pos = start_pos;

        pos.x += size.x - padding.x - image_size.x;

        ImGui::SetCursorScreenPos(pos);

        if (ImGui::Button("Del", image_size))
        {
            /*if (m_giveaways.active_giveaway)
            {
                m_giveaways.active_giveaway->remove_item(item);
            }*/

            result = true;
        }

        return result;
    }

    bool giveaway_block(float width, const Giveaway& giveaway) const
    {
        bool result = false;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        auto&& padding = m_giveaway_viewer_data.padding;
        auto&& button_size = m_giveaway_viewer_data.button_size;

        auto start_pos = ImGui::GetCursorScreenPos();

        ImVec2 size
        {
            width,
            button_size.y + padding.y * 2.f
        };

        ImVec2 max_pos
        {
            start_pos.x + size.x,
            start_pos.y + size.y
        };

        ImGui::BeginGroup();

        ImGui::ItemSize(size, 0.f);
        if (!ImGui::ItemAdd(ImRect{ start_pos, max_pos }, 0))
        {
            ImGui::EndGroup();

            return false;
        }

        auto&& bg_color = m_giveaway_viewer_data.background_color;
        auto&& rounding = m_giveaway_viewer_data.rounding;

        window->DrawList->AddRectFilled(start_pos, max_pos,
            ImGui::ColorConvertFloat4ToU32(bg_color), rounding);

        auto pos = start_pos;

        pos.x += padding.x;
        pos.y += padding.y;

        ImGui::SetCursorScreenPos(pos);

        ImGui::Text(giveaway.name.c_str());

        auto&& style = ImGui::GetStyle();

        auto button_offset = width - padding.x - button_size.x * 2.f - style.ItemSpacing.x * 1.f;

        pos.x = start_pos.x + button_offset;
        pos.y = start_pos.y + padding.y;

        ImGui::SetCursorScreenPos(pos);

        if(ImGui::Button("Ed", button_size))
        {
            m_giveaways.active_giveaway = const_cast<Giveaway*>(&giveaway);

            show_giveaway_editor();
        }

        ImGui::SameLine();

        if (ImGui::Button("Del", button_size))
        {
            /*if (m_is_active_giveaways_view)
            {
                m_giveaways.remove_giveaway(giveaway);
            }
            else if (m_is_archive_giveaways_view)
            {
                m_giveaways.remove_from_archive(giveaway);
            }*/

            result = true;
        }

        ImGui::EndGroup();

        return result;
    }

    void toolbar() const
    {
        if (m_is_giveaway_edit_view || m_is_item_edit_view) return;

        begin_toolbar();

        if (ImGui::Button("+", m_toolbar_button_size))
        {
            show_giveaway_editor();
        }

        end_toolbar();

        if (m_is_giveaway_edit_view || m_is_item_edit_view) return;

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("##giveaways_types", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Активные"))
            {
                show_active_giveaways();
        
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Архив"))
            {
                show_archive_giveaways();
        
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    void body() const
    {
        using std::begin;
        using std::end;

        if (m_is_giveaway_edit_view || m_is_item_edit_view) return;

        begin_workspace();

        auto&& style = ImGui::GetStyle();

        auto width = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.f - style.ScrollbarSize;

        if (m_is_active_giveaways_view)
        {
            for (auto&& it = begin(m_giveaways.active_giveaways), 
                end_it = end(m_giveaways.active_giveaways);
                it != end_it;)
            {
                if (giveaway_block(width, *it))
                {
                    it = m_giveaways.remove_giveaway(*it);

                    end_it = end(m_giveaways.active_giveaways);
                }
                else
                {
                    ++it;
                }

                ImGui::Spacing();
            }
        }
        else if (m_is_archive_giveaways_view)
        {
            for (auto&& it = begin(m_giveaways.archive_giveaways),
                end_it = end(m_giveaways.archive_giveaways);
                it != end_it; ++it)
            {
                if (giveaway_block(width, *it))
                {
                    it = m_giveaways.remove_giveaway(*it);

                    end_it = end(m_giveaways.archive_giveaways);
                }
                else
                {
                    ++it;
                }

                ImGui::Spacing();
            }
        }

        end_workspace();
    }

    void item_edit() const
    {
        if (!m_is_item_edit_view) return;

        static std::string image_filter = "Image Files\0*.jpg;*.jpeg;*.png\0All Files\0*.*\0";
        static bool is_inited = false;

        auto&& active_giveaway = m_giveaways.active_giveaway;

        if (!active_giveaway) return;

        auto&& active_item = active_giveaway->active_reward;

        auto&& editor = m_item_editor_data;

        if (active_item && !is_inited)
        {
            editor.texture_id = active_item->texture.getNativeHandle();
            editor.item_name_buffer = active_item->name;
            editor.item_count = active_item->count;
            editor.item_file_name = active_item->file_name;

            is_inited = true;
        }
        else if (!active_item && !is_inited)
        {
            editor.item_name_buffer.resize(100, '\0');

            is_inited = true;
        }

        begin_workspace();

        ImGui::BeginGroup();

        ImGui::InputText("##item_name", editor.item_name_buffer.data(), editor.item_name_buffer.size());

        ImGui::Spacing();

        if (ImGui::InputInt("##item_count", &editor.item_count))
        {
            if (editor.item_count < 0)
            {
                editor.item_count = 0;
            }
        }

        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();

        ImVec2 image_size
        {
            ImGui::GetContentRegionAvail().x,
            ImGui::GetContentRegionAvail().x
        };

        ImGui::Image(editor.texture_id, image_size);

        ImGui::Spacing();

        if (ImGui::Button("Выбрать изображение", ImVec2{ image_size.x, 0.f }))
        {
            auto file_name = gui::open_file_dialog(image_filter);

            if (!file_name.empty() && editor.item_image.loadFromFile(file_name))
            {
                editor.item_file_name = file_name;
                editor.texture_id = editor.item_image.getNativeHandle();
            }
        }

        ImGui::EndGroup();

        std::string button_text = active_item ? "Сохранить" : "Создать";

        if (ImGui::Button(button_text.c_str()))
        {
            if (editor.item_count < 1) std::cout << "Item count are less than 1\n";
            if (editor.item_file_name.empty()) std::cout << "Item file name are empty\n";
            if (editor.item_name_buffer.empty()) std::cout << "Item name are empty\n";

            if (active_item)
            {
                active_item->name = find_word(editor.item_name_buffer);
                active_item->count = editor.item_count;

                if (editor.item_file_name != active_item->file_name)
                {
                    active_item->texture = editor.item_image;
                }
            }
            else
            {
                Giveaway_item new_item
                {
                    find_word(editor.item_name_buffer),
                    1,
                    static_cast<std::size_t>(editor.item_count),
                    editor.item_image,
                    editor.item_file_name
                };

                active_giveaway->add_item(std::move(new_item));
            }

            editor.item_name_buffer.clear();
            editor.item_file_name.clear();
            editor.item_count = 0;
            is_inited = false;
            editor.item_image = sf::Texture{};

            show_giveaway_editor();

            active_giveaway->active_reward = nullptr;

            std::cout << "Item was created/saved!\n";
        }

        ImGui::Spacing();

        if (ImGui::Button("Отмена"))
        {
            editor.item_name_buffer.clear();
            editor.item_file_name.clear();
            editor.item_count = 0;
            is_inited = false;
            editor.item_image = sf::Texture{};

            show_giveaway_editor();

            active_giveaway->active_reward = nullptr;
        }

        //"JPG - JPEG File\0*.JPG\0PNG File\0*.PNG\0BMP - Bitmat File\0*.BMP\0\0"



        end_workspace();
    }

    void giveaway_edit() const
    {
        if (!m_is_giveaway_edit_view) return;

        static bool is_inited = false;
        auto&& active_giveaway = m_giveaways.active_giveaway;

        auto&& editor = m_giveaway_editor_data;

        if (active_giveaway && !is_inited)
        {
            editor.name_buffer = active_giveaway->name;
            editor.description_buffer = active_giveaway->description;
            editor.key_word_buffer = active_giveaway->key_word;
            editor.start_message_buffer = active_giveaway->start_message;
            editor.stop_message_buffer = active_giveaway->stop_message;
            editor.winner_message_buffer = active_giveaway->winner_message;
            editor.member_message_buffer = active_giveaway->member_message;
            editor.giveaway_buffer.rewards = active_giveaway->rewards;

            is_inited = true;
        }
        else if (!active_giveaway && !is_inited)
        {
            m_giveaways.active_giveaway = &editor.giveaway_buffer;

            is_inited = true;
        }

        begin_workspace();

        ImGui::BeginGroup();

        ImGui::Text("Название:");
        ImGui::SameLine();
        ImGui::InputText("##name_input", editor.name_buffer.data(), editor.name_buffer.size());

        ImGui::Text("Описание:");
        ImGui::SameLine();
        ImGui::InputText("##description_input", editor.description_buffer.data(), editor.description_buffer.size());

        ImGui::Text("Слово-ключ:");
        ImGui::SameLine();
        ImGui::InputText("##keyword_input", editor.key_word_buffer.data(), editor.key_word_buffer.size());
        
        ImGui::Text("Старт сообщение:");
        ImGui::SameLine();
        ImGui::InputText("##start_message_input", editor.start_message_buffer.data(), editor.start_message_buffer.size());

        ImGui::Text("Стоп сообщение:");
        ImGui::SameLine();
        ImGui::InputText("##stop_message_input", editor.stop_message_buffer.data(), editor.stop_message_buffer.size());

        ImGui::Text("Победное сообщение:");
        ImGui::SameLine();
        ImGui::InputText("##winner_message_input", editor.winner_message_buffer.data(), editor.winner_message_buffer.size());

        ImGui::Text("Сообщение участия:");
        ImGui::SameLine();
        ImGui::InputText("##member_message_input", editor.member_message_buffer.data(), editor.member_message_buffer.size());

        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();

        ImGui::Text("Участники");
        ImGui::SameLine();
        ImGui::Text("Победители");

        if (ImGui::BeginListBox("##members"))
        {
            if (active_giveaway)
            {
                for (auto&& member : active_giveaway->members)
                {
                    ImGui::Text(member.name.c_str());
                }
            }

            ImGui::EndListBox();
        }

        ImGui::SameLine();

        if (ImGui::BeginListBox("##winners"))
        {
            if (active_giveaway)
            {
                for (auto&& winner : active_giveaway->winners)
                {
                    ImGui::Text(winner.name.c_str());
                }
            }

            ImGui::EndListBox();
        }

        ImGui::EndGroup();

        ImGui::BeginGroup();

        if (ImGui::BeginListBox("##rewards"))
        {
            float width = 150.f;

            if (active_giveaway)
            {
                using std::begin;
                using std::end;

                for (auto&& it = begin(active_giveaway->rewards),
                    end_it = end(active_giveaway->rewards);
                    it != end_it;)
                {
                    if (item_block(width, *it))
                    {
                        it = active_giveaway->remove_item(*it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            ImGui::EndListBox();
        }

        if (ImGui::Button("+"))
        {
            show_item_editor();
        }

        ImGui::EndGroup();

        std::string button_text = active_giveaway ? "Сохранить" : "Создать";

        if (ImGui::Button(button_text.c_str()))
        {
            if (active_giveaway && active_giveaway != &editor.giveaway_buffer)
            {
                active_giveaway->name = find_word(editor.name_buffer);
                active_giveaway->description = find_word(editor.description_buffer);
                active_giveaway->key_word = find_word(editor.key_word_buffer);
                active_giveaway->start_message = find_word(editor.start_message_buffer);
                active_giveaway->stop_message = find_word(editor.stop_message_buffer);
                active_giveaway->winner_message = find_word(editor.winner_message_buffer);
                active_giveaway->member_message = find_word(editor.member_message_buffer);
                active_giveaway->rewards = editor.giveaway_buffer.rewards;

                editor.giveaway_buffer.rewards.clear();

                editor.name_buffer.clear();
                editor.description_buffer.clear();
                editor.key_word_buffer.clear();
                editor.start_message_buffer.clear();
                editor.stop_message_buffer.clear();
                editor.winner_message_buffer.clear();
                editor.member_message_buffer.clear();

                editor.name_buffer.resize(250, '\0');
                editor.description_buffer.resize(250, '\0');
                editor.key_word_buffer.resize(100, '\0');
                editor.start_message_buffer.resize(250, '\0');
                editor.stop_message_buffer.resize(250, '\0');
                editor.winner_message_buffer.resize(250, '\0');
                editor.member_message_buffer.resize(250, '\0');

                m_giveaways.active_giveaway = nullptr;

                is_inited = false;

                show_active_giveaways();
            }
            else if (active_giveaway == &editor.giveaway_buffer)
            {
                Giveaway result
                {
                    find_word(editor.name_buffer),
                    find_word(editor.description_buffer),
                    1,
                    find_word(editor.key_word_buffer),
                    find_word(editor.start_message_buffer),
                    find_word(editor.stop_message_buffer),
                    find_word(editor.winner_message_buffer),
                    find_word(editor.member_message_buffer),
                    {},
                    editor.giveaway_buffer.rewards
                };

                m_giveaways.add_giveaway(std::move(result));

                editor.giveaway_buffer.rewards.clear();

                editor.name_buffer.clear();
                editor.description_buffer.clear();
                editor.key_word_buffer.clear();
                editor.start_message_buffer.clear();
                editor.stop_message_buffer.clear();
                editor.winner_message_buffer.clear();
                editor.member_message_buffer.clear();

                editor.name_buffer.resize(250, '\0');
                editor.description_buffer.resize(250, '\0');
                editor.key_word_buffer.resize(100, '\0');
                editor.start_message_buffer.resize(250, '\0');
                editor.stop_message_buffer.resize(250, '\0');
                editor.winner_message_buffer.resize(250, '\0');
                editor.member_message_buffer.resize(250, '\0');

                m_giveaways.active_giveaway = nullptr;

                is_inited = false;

                show_active_giveaways();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Отмена"))
        {
            editor.giveaway_buffer.rewards.clear();

            editor.name_buffer.clear();
            editor.description_buffer.clear();
            editor.key_word_buffer.clear();
            editor.start_message_buffer.clear();
            editor.stop_message_buffer.clear();
            editor.winner_message_buffer.clear();
            editor.member_message_buffer.clear();

            editor.name_buffer.resize(250, '\0');
            editor.description_buffer.resize(250, '\0');
            editor.key_word_buffer.resize(100, '\0');
            editor.start_message_buffer.resize(250, '\0');
            editor.stop_message_buffer.resize(250, '\0');
            editor.winner_message_buffer.resize(250, '\0');
            editor.member_message_buffer.resize(250, '\0');

            m_giveaways.active_giveaway = nullptr;

            is_inited = false;

            show_active_giveaways();
        }

        if (active_giveaway && active_giveaway != &editor.giveaway_buffer)
        {
            std::string a_text = active_giveaway->is_active ? "Остановить" : "Начать";
            std::string c_text = "Закрыть";

            if (ImGui::Button(a_text.c_str()))
            {
                active_giveaway->is_active = !active_giveaway->is_active;

                m_giveaways.active_giveaway = nullptr;
            }

            if (ImGui::Button(c_text.c_str()))
            {
                active_giveaway->is_completed = !active_giveaway->is_completed;

                if (active_giveaway->is_completed)
                {
                    std::cout << "To archive\n";
                    //m_giveaways.add_to_archive(*active_giveaway);
                }

                m_giveaways.active_giveaway = nullptr;
            }
        }

        end_workspace();
    }

    void show_active_giveaways() const
    {
        m_is_active_giveaways_view = true;
        m_is_archive_giveaways_view = false;
        m_is_giveaway_edit_view = false;
        m_is_item_edit_view = false;
    }

    void show_archive_giveaways() const
    {
        m_is_active_giveaways_view = false;
        m_is_archive_giveaways_view = true;
        m_is_giveaway_edit_view = false;
        m_is_item_edit_view = false;
    }

    void show_giveaway_editor() const
    {
        m_is_active_giveaways_view = false;
        m_is_archive_giveaways_view = false;
        m_is_giveaway_edit_view = true;
        m_is_item_edit_view = false;
    }

    void show_item_editor() const
    {
        m_is_active_giveaways_view = false;
        m_is_archive_giveaways_view = false;
        m_is_giveaway_edit_view = false;
        m_is_item_edit_view = true;
    }

    void init()
    {
        auto&& editor = m_giveaway_editor_data;

        editor.name_buffer.resize(250, '\0');
        editor.description_buffer.resize(250, '\0');
        editor.key_word_buffer.resize(100, '\0');
        editor.start_message_buffer.resize(250, '\0');
        editor.stop_message_buffer.resize(250, '\0');
        editor.winner_message_buffer.resize(250, '\0');
        editor.member_message_buffer.resize(250, '\0');

        auto&& item_editor = m_item_editor_data;

        item_editor.item_name_buffer.resize(250, '\0');
    }

private:
    mutable Giveaway_viewer_data m_giveaway_viewer_data;
    mutable Item_viewer_data m_item_viewer_data;
    mutable Item_editor_data m_item_editor_data;
    mutable Giveaway_editor_data m_giveaway_editor_data;

    ImVec2 m_toolbar_button_size{ 30.f, 30.f };
    ImVec4 m_toolbar_bg_color{ 0.5f, 0.5f, 0.5f, 1.f };
    ImVec2 m_toolbar_padding{ 5.f, 5.f };

    Giveaways_manager& m_giveaways;
    
    mutable bool m_is_active_giveaways_view{ true };
    mutable bool m_is_archive_giveaways_view{ false };
    mutable bool m_is_giveaway_edit_view{ false };
    mutable bool m_is_item_edit_view{ false };
};







int main() 
{
    


    try
    {
        sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML = <3");
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window, false);

        auto&& IO = ImGui::GetIO();

        auto&& font = IO.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, NULL,
            IO.Fonts->GetGlyphRangesCyrillic());
        ImGui::SFML::UpdateFontTexture();

        //ImGuiContext* g = ImGui::GetCurrentContext();

        /*std::vector<Twitch_message_block> m_messages
        {
            Twitch_message_block{"[col=0000ff]Bruvamasc[col]: SAIidm 129i9 admsakdm ksmdk m di1mi msd [col=0000ff]1ojds[col] 91jidskd!"},
            Twitch_message_block{"[col=0000ff]Lolosha[col]: ASJDkjsak djksa12j kjkds kadjksajdk jss 19jd 1902ufdi sadkj [col=0000ff]813rh[col] dkjklsjdksajd83hdfksd"},
            Twitch_message_block{"[col=0000ff]RObin12_sd1A[col]: Veritasio s;adl 10okd okdlk alivBAN [col=0000ff]alivBAN[col] alivBAN"}
        };*/

        /*for (auto&& msg : m_messages)
        {
            msg.set_rounding(5.f);
            msg.set_text_color(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            msg.set_background_color(ImVec4{ 35.f / 255.f, 35.f / 255.f, 35.f / 255.f, 255.f / 255.f });
        }*/

        //sf::Texture icons_texture;

        /*if (!icons_texture.loadFromFile("../resources/plugins/Chat logger/icons.png"))
        {
            ImGui::SFML::Shutdown();

            throw std::exception{ "Icons texture are not loaded!" };
        }*/

        std::vector<std::string> messages
        {
            "27.10.2024_05:06:39 Bruvamasc: @Jikaty0 -2 по мск. Если судить по Белграду!",
            "27.10.2024_05:12:39 Lolosha: ASJDkjsak djksa12j kjkds kadjksajdk jss 19jd 1902ufdi sadkj 813rh dkjklsjdksajd83hdfksd",
            "27.10.2024_09:06:39 RObin12_sd1A: Veritasio s;adl 10okd okdlk alivBAN alivBAN alivBAN"
        };


        bool is_save_system_msg = true;
        bool is_save_bot_msg = true;
        bool is_save_user_msg = true;
        bool is_save_chat_system_msg = true;

        float offset = 0.f;

        //Logger_data chat_data;
        //chat_data.button_texture_uid = icons_texture.getNativeHandle();

        //Giveaways_data general_data;

        Giveaways_manager giveaways_manager;

        Giveaway_widget widget{ giveaways_manager };

        sf::Clock deltaClock;
        while (window.isOpen())
        {
            sf::Event event;

            while (window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::ShowDemoWindow();

            auto&& style = ImGui::GetStyle();

            widget.draw(deltaClock.getElapsedTime().asSeconds());

            /*begin_workspace();
            toolbar(general_data);
            items(general_data);
            item_edit(general_data, general_data.active_item);
            end_workspace();*/


            //begin_chat();
            //toolbar(chat_data);
            //settings(chat_data);

            //float message_width = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.f - style.ScrollbarSize;
            //messages_block(messages, message_width, chat_data);
            //end_chat();

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }

        ImGui::SFML::Shutdown();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknow exception!\n";
    }

    

    return 0;
}



//
//
//
//
//
//void begin_giveaway_window()
//{
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
//    ImGui::Begin("Giveaways");
//}
//
//void end_giveaway_window()
//{
//    ImGui::End();
//    ImGui::PopStyleVar();
//}
//
//void begin_toolbar(const Giveaway_data& data)
//{
//    ImGuiWindowFlags flags = 0;
//    flags |= ImGuiWindowFlags_NoTitleBar;
//    flags |= ImGuiWindowFlags_NoScrollbar;
//    flags |= ImGuiWindowFlags_NoMove;
//    //flags |= ImGuiWindowFlags_NoResize;
//    flags |= ImGuiWindowFlags_NoCollapse;
//    flags |= ImGuiWindowFlags_NoNav;
//    flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
//
//    auto&& window_width = ImGui::GetWindowWidth();
//    ImVec2 button_size = data.button_size;
//    ImVec2 paddings = data.padding;
//    ImVec2 toolbar_size = ImVec2{ window_width, button_size.y + paddings.y * 3.f };
//
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, paddings);
//    ImGui::PushStyleColor(ImGuiCol_ChildBg, data.toolbar_bg_color);
//    ImGui::BeginChild("##toolbar", toolbar_size, false, flags);
//}
//
//void end_toolbar()
//{
//    ImGui::EndChild();
//    ImGui::PopStyleColor();
//    ImGui::PopStyleVar();
//}
//
//void begin_giveaway_body(const Giveaway_data& data)
//{
//    ImGuiWindowFlags flags = 0;
//    flags |= ImGuiWindowFlags_NoTitleBar;
//    flags |= ImGuiWindowFlags_NoMove;
//    //flags |= ImGuiWindowFlags_NoResize;
//    flags |= ImGuiWindowFlags_NoCollapse;
//    flags |= ImGuiWindowFlags_NoNav;
//    flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
//
//    auto&& window_width = ImGui::GetWindowWidth();
//    auto&& height = ImGui::GetContentRegionAvail().y;
//
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, data.padding);
//    //ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
//    ImGui::BeginChild("##giveaways_body", ImVec2{ window_width, 0 }, false, flags);
//}
//
//void end_giveaway_body()
//{
//    ImGui::EndChild();
//    ImGui::PopStyleVar();
//}
//
//void toolbar(Giveaway_data& data)
//{
//    if (data.is_giveaway_view || data.is_item_view) return;
//
//    begin_toolbar(data);
//
//    if (ImGui::Button("+", data.button_size) && data.is_active_giveaways_view)
//    {
//        data.set_giveaway();
//    }
//
//    end_toolbar();
//
//    
//}
//
//void giveaway_rect(float width, Giveaway& giveaway, std::size_t index, Giveaway_manager& giveaways, Giveaway_data& data)
//{
//    auto window = ImGui::GetCurrentWindow();
//
//    if (window->SkipItems)
//    {
//        return;
//    }
//
//    auto&& style = ImGui::GetStyle();
//
//    auto&& start_pause_text_size = ImGui::CalcTextSize(">");
//    auto&& archive_text_size = ImGui::CalcTextSize("Arch");
//    auto&& delete_text_size = ImGui::CalcTextSize("Del");
//
//    float buttons_space = start_pause_text_size.x +
//        archive_text_size.x + delete_text_size.x + style.FramePadding.x * 6.f +
//        style.ItemSpacing.x * 2.f;
//
//    float wrap_width = width - data.padding.x * 3.f;
//
//    auto&& name = giveaway.name;
//    std::string status = giveaway.is_active ? "Активный" : "Неактивный";
//
//    auto&& name_text_size = ImGui::CalcTextSize(name.c_str(), name.c_str() + name.size(), false, wrap_width);
//    auto&& status_text_size = ImGui::CalcTextSize(status.c_str(), status.c_str() + status.size(), false, wrap_width);
//
//    auto start_pos = ImGui::GetCursorScreenPos();
//
//    auto background_color = data.giveaway_rect_bg_color;
//    auto background_rounding = data.rounding;
//
//    ImVec2 rect_size
//    {
//        width,
//        data.padding.y * 2.f + name_text_size.y + style.ItemSpacing.y + status_text_size.y
//    };
//
//    ImVec2 rect_max_pos
//    {
//       start_pos.x + rect_size.x,
//       start_pos.y + rect_size.y
//    };
//
//    ImGui::BeginGroup();
//
//    ImGui::ItemSize(rect_size, 0.f);
//    if (!ImGui::ItemAdd(ImRect{ start_pos, rect_max_pos }, 0))
//    {
//        ImGui::EndGroup();
//        return;
//    }
//
//    window->DrawList->AddRectFilled(start_pos, rect_max_pos,
//        ImGui::ColorConvertFloat4ToU32(background_color), background_rounding);
//
//    start_pos.x += data.padding.x;
//    start_pos.y += data.padding.y;
//
//    auto old_pos = start_pos;
//
//    ImGui::SetCursorScreenPos(start_pos);
//
//    ImGui::PushTextWrapPos(wrap_width);
//
//    ImGui::Text(giveaway.name.c_str());
//
//    start_pos.y = old_pos.y + name_text_size.y + style.ItemSpacing.y;
//
//    ImGui::SetCursorScreenPos(start_pos);
//
//    ImGui::Text(status.c_str());
//
//    ImGui::PopTextWrapPos();
//
//    start_pos = old_pos;
//    start_pos.x += width - data.padding.x - buttons_space;
//
//    auto button_height = rect_size.y - data.padding.y * 2.f;
//
//    if (giveaway.is_active)
//    {
//        if (ImGui::Button("S", ImVec2{ 0.f, button_height }))
//        {
//            giveaway.is_active = false;
//        }
//    }
//    else
//    {
//        if (ImGui::Button(">", ImVec2{ 0.f, button_height }))
//        {
//            giveaway.is_active = true;
//        }
//    }
//
//    ImGui::SameLine();
//
//    bool is_need_archive = false;
//    bool is_need_delete = false;
//
//    if (ImGui::Button("Arch", ImVec2{ 0.f, button_height }) && !giveaway.is_active)
//    {
//        is_need_archive = true;
//    }
//
//    if (ImGui::Button("Del", ImVec2{ 0.f, button_height }) && !giveaway.is_active)
//    {
//        is_need_delete = true;
//    }
//
//    if (is_need_archive && !is_need_delete)
//    {
//        giveaways.archive_giveaways.push_back(std::move(giv))
//    }
//    else if (is_need_delete)
//    {
//
//    }
//
//    ImGui::EndGroup();
//}
//
//void giveaways(Giveaway_manager& giveaways, Giveaway_data& data)
//{
//    if (data.is_giveaway_view || data.is_item_view) return;
//
//    begin_giveaway_body(data);
//
//    auto window = ImGui::GetCurrentWindow();
//    if (window->Sk)
//
//        end_giveaway_body();
//}
//
//void giveaway(Giveaway_manager& giveaways, Giveaway_data& data)
//{
//
//}
//
//void item(Giveaway_manager& giveaways, Giveaway_data& data)
//{
//
//}















//#include <iostream>
//#include <string>
//#include <thread>
//#include <chrono>
//#include <ctime>
//
//#include "twitch_irc/twitch_irc_message.h"
//#include "twitch_irc/twitch_irc_utility.h"
//
//#include "support/support_classes.h"
//#include "IRC_Client.h"
//
//std::string get_time()
//{
//	// Получаем текущее время с помощью <chrono>
//	auto now = std::chrono::system_clock::now();
//
//	// Преобразуем время в формат, подходящий для вывода
//	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
//
//	// Получаем структуру tm из текущего времени
//	std::tm* now_tm = std::localtime(&now_c);
//
//	// Определяем буфер для форматирования времени
//	char buffer[80]{ '\0' };
//
//	// Форматируем время в строку
//	std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", now_tm);
//
//	//std::put_time(now_tm, "%d.%m.%Y %H:%M:%S")ж
//
//	return std::string{ buffer };
//}
//
//std::string get_print_time()
//{
//	std::string result{ "[ " };
//	result += get_time();
//	result += " ] - ";
//
//	return result;
//}
//
//class Test_programm
//{
//public:
//
//	~Test_programm()
//	{
//		if (m_client.is_in_channel())
//		{
//			m_client.leave();
//		}
//
//		if (m_client.is_connected())
//		{
//			m_client.disconnect();
//		}
//
//		if (m_irc_thread.joinable())
//		{
//			m_irc_thread.join();
//		}
//	}
//
//	void start()
//	{
//		std::string username = "bruvamasc";
//		//std::string oauth_token = "oauth:q53glixoe1qblzt7ukl2han2l8a9";
//		std::string oauth_token = "oauth:q53glixoe1qblzt7ukl2han2l8awp9";
//		std::string channel = "bruvamasc";
//
//		auto&& on_connected = [this](const bot::Event<std::string, std::string>& in_event)
//			{
//				auto&& server = in_event.get_member<0>();
//				auto&& port = in_event.get_member<1>();
//
//				auto&& loger = support::Logs::get_instance();
//				
//				loger << get_print_time() << std::string_view{ "Connected to " } << server << ':' << port << '\n';
//			};
//
//		auto&& on_successful_oauth = [this](const bot::Event<> in_event)
//			{
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << std::string_view{ "Successful oauth!" } << '\n';
//			};
//
//		auto&& on_disconnected = [this, channel](const bot::Event<std::string, std::string>& in_event)
//			{
//				auto&& server = in_event.get_member<0>();
//				auto&& port = in_event.get_member<1>();
//
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << std::string_view{ "Disconnected from " } << server << ':' << port << '\n';
//			};
//
//		auto&& loger_functor = [this](const bot::Event<twitch::irc::Message>& in_event)
//			{
//				static int index{ 0 };
//				
//				auto&& message = in_event.get_member<0>().get_raw_message();
//
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << message << '\n';
//
//				++index;
//			};
//
//		auto&& on_join_channel = [this](const bot::Event<std::string>& in_event)
//			{
//				auto&& channel_name = in_event.get_member<0>();
//
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << std::string_view{ "Join to " } << channel_name << std::string_view{" channel"} << '\n';
//			};
//
//		auto&& on_leave_channel = [this](const bot::Event<std::string>& in_event)
//			{
//				auto&& channel_name = in_event.get_member<0>();
//
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << std::string_view{ "Leave from " } << channel_name << std::string_view{ " channel" } << '\n';
//			};
//
//		auto&& on_error = [this](const bot::Event<std::string>& in_event)
//			{
//				auto&& error_text = in_event.get_member<0>();
//
//				auto&& loger = support::Logs::get_instance();
//
//				loger << get_print_time() << std::string_view{ "Error:  " } << error_text << '\n';
//			};
//
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_connected), bot::make_handler(on_connected));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_disconnected), bot::make_handler(on_disconnected));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_successful_oauth), bot::make_handler(on_successful_oauth));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_join), bot::make_handler(on_join_channel));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_leave), bot::make_handler(on_leave_channel));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_receive_message), bot::make_handler(loger_functor));
//		m_client.get_events_manager().subscribe(bot::to_string(bot::IRC_Client::event_type::on_error), bot::make_handler(on_error));
//
//		m_irc_thread = std::thread{ [this]()
//			{
//				//m_connection.connect_to("irc.chat.twitch.tv", "6667");
//				//m_connection.start();
//				m_client.connect("irc.chat.twitch.tv", "6667", m_oauth_token, m_bot_name);
//				//m_client.join(m_channel_name);
//			} };
//	}
//
//	void join()
//	{
//		m_client.join(m_channel_name);
//	}
//
//	void part()
//	{
//		m_client.leave();
//	}
//
//private:
//	bot::IRC_Client m_client;
//
//	std::string m_bot_name{ "bruvamasc" };
//	//std::string m_oauth_token{ "oauth:q53glixoe1qblzt7ukl2han2lawp9" };
//	std::string m_oauth_token{ "oauth:q53glixoe1qblzt7ukl2han2l8awp9" };
//	std::string m_channel_name{ "bruvamasc" };
//
//	std::thread m_irc_thread;
//};
//
//int main()
//{
//	try
//	{
//		Test_programm prog;
//
//		prog.start();
//
//		std::string input;
//
//		while (std::cin >> input)
//		{
//			if (input == "leave")
//			{
//				prog.part();
//			}
//			else if (input == "join")
//			{
//				prog.join();
//			}
//			else if (input == "exit")
//			{
//				break;
//			}
//		}
//
//		/*std::thread wait_thread{ []()
//			{
//				static int index = 0;
//			while (index <= 120 * 1000)
//		{
//			++index;
//		}
//		} };
//
//		wait_thread.join();*/
//	}
//	catch (const std::exception& e)
//	{
//		std::cerr << "Exception: " << e.what() << '\n';
//	}
//
//	return 0;
//}