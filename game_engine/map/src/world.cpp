//
//  world.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#include "world.hpp"
#include "../../render/include/view.hpp"

/*
===================================================
 WORLD GENERATING FUNCTIONS
===================================================
*/

void World::generate(){
    printf("Generating New World");

    //Generate the Blank Region Files
    blank();

    //Generate Height
    Terrain terrain(this);
    terrain.generate();
}

void World::blank(){
    //Loop over all Chunks in the World
    for(int i = 0; i < dim.x; i++){
        for(int j = 0; j < dim.y; j++){
            for(int k = 0; k < dim.z; k++){
                //Generate a new chunk at a specific location
                Chunk chunk;
                chunk.biome = BIOME_VOID;
                chunk.position = glm::vec3(i, j, k);
            }
        }
    }
}

/*
===================================================
 MOVEMENT RELATED FUNCTIONS
===================================================
*/

Voxel_Type World::getBlock(glm::vec3 _pos){
    glm::vec3 c = glm::floor(_pos/glm::vec3(chunkSize));
    glm::vec3 p = glm::mod(_pos, glm::vec3(chunkSize));
    int ind = Chunk::getIndex(c, dim);

    //Get the Block with Error Handling
    if(!(glm::all(glm::greaterThanEqual(c, min)) && glm::all(glm::lessThanEqual(c, max)))) return VOXEL_VOID;
        return chunks[chunk_order[ind]].data[chunks[chunk_order[ind]].getIndex(p)].type;
}

void World::setBlock(glm::vec3 _pos, Voxel_Type _type, bool fullremesh){
    glm::vec3 c = glm::floor(_pos/glm::vec3(chunkSize));
    glm::vec3 p = glm::mod(_pos, glm::vec3(chunkSize));
    int ind = Chunk::getIndex(c, dim);

    if(!(glm::all(glm::greaterThanEqual(c, min)) && glm::all(glm::lessThanEqual(c, max)))) return;
    chunks[chunk_order[ind]].setPosition(p, _type);
}

//Get the Top-Free space position in the x-z position
glm::vec3 World::getTop(glm::vec2 _pos){
    //Highest Block you can Stand O
    int max = 0;
    Voxel_Type floor;

    //Loop over the height
    for(int i = 1; i < dim.y*chunkSize; i++){
    floor = getBlock(glm::vec3(_pos.x, i-1, _pos.y));
    //Check if we satisfy the conditions
        if(getBlock(glm::vec3(_pos.x, i, _pos.y)) == VOXEL_AIR && Voxel::isSpawnable(floor)){
            if(i > max){
                    max = i;
            }
        }
    }
    return glm::vec3(_pos.x, max, _pos.y);
}
