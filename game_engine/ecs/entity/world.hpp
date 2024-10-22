//
//  world.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef WORLD_HPP
#define WORLD_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "map/include/chunk.hpp"
#include "map/include/voxel.hpp"

#include "ecs/component/component.hpp"
#include "dependencies/flecs.h"

#include "logger/logger.hpp"
#include "logger/quill_wrapper.h"

void World_Systems(flecs::world& ecs) {
        ecs.system<Component::World_Entity>("worldInit")
            .kind(flecs::OnStart)
            .each([](flecs::entity cube, Component::World_Entity& world_entity){
                LOG_INFO("Entity: Initializing World");
                
                //world_entity.world->
            });
    
        ecs.system<Component::World_Entity>("worldUpdater")
            .kind(flecs::OnUpdate)
            .each([&ecs](flecs::entity entity, Component::World_Entity& world){
                world.world->worldUpdater(ecs);
            });
}

#endif /* WORLD_HPP */
