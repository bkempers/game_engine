//
//  world.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/25/24.
//

#include "world.hpp"
#include "ecs/component/component.hpp"

#include "logger/logger.hpp"
#include "logger/quill_wrapper.h"

World::World()
{
    //idk setup
}

void World::initialize()
{
    //initialize block dictionaries
    Voxel::Initialization();
    LOG_INFO("Initializing World class");
}

void World::worldUpdater(flecs::world& world)
{
    flecs::entity camera_entity = world.lookup("camera");
    const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
    
    //chunk manager updater
    int xpos = camera_component->position.x / Chunk::CHUNK_SIZE;
    int ypos = camera_component->position.z / Chunk::CHUNK_SIZE;
    chunkUpdater(world, xpos, ypos);
    chunkClearer(world, xpos, ypos);
}

void World::chunkUpdater(flecs::world &world, int xpos, int ypos)
{
    if (glfwGetTime() - lastChunkUpdate > 0.05)
    {
        lastChunkUpdate = glfwGetTime();
        
        bool chunkAdded = false;
        for (int x = -Chunk::CHUNK_RENDER_RADIUS + xpos; x <= Chunk::CHUNK_RENDER_RADIUS + xpos; x++) {
            for (int y = -Chunk::CHUNK_RENDER_RADIUS + ypos; y <= Chunk::CHUNK_RENDER_RADIUS + ypos; y++) {
                bool found = false;
                if(map.size() > 0) {
                    for (auto& chunk : map) {
                        if (chunk.position.x == x && chunk.position.y == y){
                            chunk.model.update();
                            found = true;

                            //Old chunks need to be translated too. Translate according to player position.
    //                        glm::vec3 axis = (glm::vec3)(world.map[i].c_position)*(float)world.chunkSize-viewPos;
    //
    //                        //Translate the guy
    //                        chunk.model.reset();
    //                        chunk.model.translate(axis);
                        }
                    }
                }

                //add chunk & render, if it doesnt exist yet
                if (!found && !chunkAdded) {
                    Chunk chunk = Chunk(glm::vec2(x, y));
                    chunk.loadVoxels();
                    chunk.fromChunkGreedy();
                    chunk.model.setup();
//                    chunk.createMesh();
                    map.push_back(chunk);
                    chunkAdded = true;
                    break;
                }
            }
        }
    }
}

void World::chunkClearer(flecs::world &world, int xpos, int ypos)
{
    for (auto it = map.begin(); it != map.end();) {
        if (it->position.x < xpos - Chunk::CHUNK_RENDER_RADIUS || it->position.x > xpos + Chunk::CHUNK_RENDER_RADIUS ||
            it->position.y < ypos - Chunk::CHUNK_RENDER_RADIUS || it->position.y > ypos + Chunk::CHUNK_RENDER_RADIUS) {
            it = map.erase(it);
        }
        else {
            // Move to the next element
            ++it;
        }
    }
}
