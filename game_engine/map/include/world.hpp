//
//  world.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/25/24.
//

#ifndef world_hpp
#define world_hpp

#include <stdio.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dependencies/flecs.h"
#include "map/include/chunk.hpp"

class World{
public:
    std::vector<Chunk> map;
    int chunkSize = 16;
    double lastChunkUpdate = glfwGetTime();
    
    World();
    void initialize();
    void worldUpdater(flecs::world& world);
    void chunkUpdater(flecs::world& world, int xpos, int ypos);
    void chunkClearer(flecs::world& world, int xpos, int ypos);
    
private:

};

#endif /* world_hpp */
