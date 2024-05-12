//
//  gui.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/19/24.
//

#ifndef GUI_HPP
#define GUI_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../gui/imgui/imgui.h"
#include "../../gui/imgui/imgui_impl_glfw.h"
#include "../../gui/imgui/imgui_impl_opengl3.h"

#include <string>
#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

void GUI_Systems(flecs::world& ecs){
    ecs.system<Component::GUI>("guiInit")
        .kind(flecs::OnStart)
        .each([&ecs](flecs::entity gui, Component::GUI& g){
            flecs::entity window_entity = ecs.lookup("main_window");
            const Component::Window* window_component = window_entity.get<Component::Window>();
            
            // ImGUI Initialization
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window_component->window, true);
            ImGui_ImplOpenGL3_Init();
        });
    
    ecs.system<Component::GUI>("setupGUI")
        .kind(flecs::PreStore)
        .each([&ecs](flecs::entity gui, Component::GUI& g){
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowSize(ImVec2(700, 350));
            bool detail_active = true;
            //flecs::query entity_query = ecs.query<>();
            if(ImGui::Begin("Entity Details", &detail_active, ImGuiWindowFlags_MenuBar)){
                flecs::entity cube_entity = ecs.lookup(g.selected_entity);
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                        if (ImGui::MenuItem("Close", "Ctrl+W"))  { detail_active = false; }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                
                ImGui::Text("%f ms/frame", g.fps);
                
                // Entities columns
                {
                    ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
                    for (auto& name : g.entity_names)
                     {
                         // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                         char label[128];
                         sprintf(label, "%s", name);
                         if (ImGui::Selectable(label, g.selected_entity == name))
                             g.selected_entity = name;
                     }
                    ImGui::EndChild();
                }
                ImGui::SameLine();

                // Entity details right
                {
                    ImGui::BeginGroup();
                    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                    ImGui::Text("Entity: %s", g.selected_entity);
                    ImGui::Separator();
                    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                    {
                        if (ImGui::BeginTabItem("Details"))
                        {
                            if(cube_entity) {
                                cube_entity.each([&](flecs::id id) {
                                    if(id.is_entity()){
                                        Component::ReadComponent(ecs, cube_entity, id);
                                    }
                                });
                            }
                            ImGui::EndTabItem();
                        }
                        
                        if (ImGui::BeginTabItem("Manipulate"))
                        {
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                    ImGui::EndChild();
                    ImGui::EndGroup();
                }
            }

            ImGui::End();
        });
    
//    ecs.system<Component::GUI>("renderGUI")
//        .kind(flecs::OnStore)
//        .each([&ecs](flecs::entity gui, Component::GUI& g){
//            ImGui::Render();
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//        });
}

#endif /* gui_hpp */
