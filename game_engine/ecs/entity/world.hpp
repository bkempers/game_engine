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
#include "../../map/include/chunk.hpp"
#include "../component/component.hpp"
#include "../flecs.h"
#include "../../map/include/voxel.hpp"

void chunkRenderer(flecs::world& ecs, Component::World& world, int xpos, int ypos){
    flecs::entity world_entity = ecs.lookup("world");
    if (glfwGetTime() - world.lastChunkUpdate > 0.05)
    {
        world_entity.set([](Component::World& world) {
            world.lastChunkUpdate = glfwGetTime();
        });
        bool chunkAdded = false;
        for (int x = -Chunk::CHUNK_RENDER_RADIUS + xpos; x <= Chunk::CHUNK_RENDER_RADIUS + xpos; x++) {
            for (int y = -Chunk::CHUNK_RENDER_RADIUS + ypos; y <= Chunk::CHUNK_RENDER_RADIUS + ypos; y++) {
                bool found = false;
                for (const auto& chunk : world.map) {
                    if (chunk.position.x == x && chunk.position.y == y)
                        found = true;
                }

                //add chunk & render, if it doesnt exist yet
                if (!found && !chunkAdded) {
                    Chunk chunk = Chunk(glm::vec2(x, y));
                    chunk.loadVoxels();
                    chunk.createMesh();
                    world_entity.set([chunk](Component::World& world) {
                        world.map.push_back(chunk);
                    });
                    chunkAdded = true;
                    break;
                }
            }
        }
    }
}

void chunkClearer(flecs::world& ecs, Component::World& world, int xpos, int ypos){
    flecs::entity world_entity = ecs.lookup("world");
    for (auto it = world.map.begin(); it != world.map.end();) {
        if (it->position.x < xpos - Chunk::CHUNK_RENDER_RADIUS || it->position.x > xpos + Chunk::CHUNK_RENDER_RADIUS ||
            it->position.y < ypos - Chunk::CHUNK_RENDER_RADIUS || it->position.y > ypos + Chunk::CHUNK_RENDER_RADIUS) {

            world_entity.set([&it](Component::World& world) {
                it = world.map.erase(it);
            });
        }
        else {
            // Move to the next element
            ++it;
        }
    }
}

void World_Systems(flecs::world& ecs) {
        ecs.system<Component::World>("worldInit")
            .kind(flecs::OnStart)
            .each([](flecs::entity cube, Component::World& world){
                //initialize block dictionaries
                Voxel::Initialization();

            });

    ecs.system<Component::World>("worldChunkUpdater")
        .kind(flecs::PreStore)
        .each([&ecs](flecs::entity cube, Component::World& world){
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
            //chunk manager updater
            int xpos = camera_component->position.x / Chunk::CHUNK_SIZE;
            int ypos = camera_component->position.z / Chunk::CHUNK_SIZE;
            chunkRenderer(ecs, world, xpos, ypos);
            chunkClearer(ecs, world, xpos, ypos);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
}

#endif /* WORLD_HPP */
