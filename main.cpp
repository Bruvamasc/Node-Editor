#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

int main() 
{
    try
    {
        sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML = <3");
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window, true);

        /*auto&& IO = ImGui::GetIO();

        auto&& font = IO.Fonts->AddFontFromFileTTF("arial.ttf", 16.f, NULL,
            IO.Fonts->GetGlyphRangesCyrillic());
        ImGui::SFML::UpdateFontTexture();*/

        float value_1{ 0.f };

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
            auto&& iasd = ImGui::GetCurrentContext();

            ImGui::Begin("Test");

            auto&& pos = ImGui::GetCursorScreenPos();

            auto&& draw_list = ImGui::GetWindowDrawList();

            //auto&& w = ImGui::GetCurrentWindow();
            draw_list->ChannelsSplit(2);

            draw_list->ChannelsSetCurrent(1);

            ImGui::BeginGroup();

            ImGui::Text("Test text");
            ImGui::SameLine();
            ImGui::InputFloat("##sdad", &value_1);

            ImGui::Text("asdasdasd");
            ImGui::Dummy(ImVec2{ 50.f, 30.f });
            ImGui::Text("ASDJWA91i293i29dksam");

            ImGui::EndGroup();

            auto&& rect = ImGui::GetItemRectSize();
            ImVec2 max_pos
            {
                pos.x + rect.x,
                pos.y + rect.y
            };

            

            draw_list->ChannelsSetCurrent(0);

            draw_list->AddRectFilled(pos, max_pos, ImGui::ColorConvertFloat4ToU32(ImVec4{ 0.5f, 0.5f, 0.5f, 1.f }), 5.f);
            
            draw_list->ChannelsMerge();

            ImGui::End();

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