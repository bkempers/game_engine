//
//  render.hpp
//  vrge_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef RENDER_HPP
#define RENDER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../../map/include/chunk.hpp"
#include "../component/component.hpp"
#include "../flecs.h"
#include "gui.hpp"

void Renderer(flecs::world& ecs) {
        ecs.system<Component::Renderer>("rendererInit")
            .kind(flecs::OnStart)
            .each([](flecs::entity renderer, Component::Renderer& render){
                
            });

    ecs.system<Component::Renderer>("preRender")
        .kind(flecs::PreStore)
        .each([&ecs](flecs::entity renderer, Component::Renderer& render){
            
        });
    
    ecs.system<Component::Renderer>("render")
        .kind(flecs::OnStore)
        .each([&ecs](flecs::entity renderer, Component::Renderer& render){
            flecs::entity world_entity = ecs.lookup("world");
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            const Component::World* world_component = world_entity.get<Component::World>();
            
            render.view.updateChunkModels(world_component->world);
            render.view.render(world_component->world);
            
            //render gui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        });
}

#endif /* render_hpp */
