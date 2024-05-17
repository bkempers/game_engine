//
//  world.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef WORLD_HPP
#define WORLD_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../../map/include/chunk.hpp"
#include "../component/component.hpp"
#include "../flecs.h"
#include "../../map/include/voxel.hpp"

void World_Systems(flecs::world& ecs) {
        ecs.system<Component::World>("worldInit")
            .kind(flecs::OnStart)
            .each([](flecs::entity cube, Component::World& world){
                

            });

    ecs.system<Component::World>("worldChunkUpdater")
        .kind(flecs::PreStore)
        .each([&ecs](flecs::entity cube, Component::World& world){
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
        });
}

#endif /* WORLD_HPP */
