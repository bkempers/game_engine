//
//  new_chunk.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#ifndef chunk_hpp
#define chunk_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "voxel.hpp"
#include "octree.hpp"

enum BiomeType{
  BIOME_VOID = 0,
  BIOME_DESERT = 1,
  BIOME_FOREST = 2,
};

#define CHUNK_SIZE 16

class Chunk{
public:
    
    //Position information and size information
    glm::ivec3 position;
    bool remesh = false;
    int chunk_size = 16;
    BiomeType biome;
    
    //Voxel data structure
    Voxel data[16 * 16 * 16];
    
    Chunk();

    //From Octree Datastructure
    void fromOctree(Octree &octree, glm::vec3 offset);

    //Get the Flat-Array Index
    static int getIndex(glm::vec3 _p);
    //3D Index
    static int getIndex(glm::vec3 p, glm::vec3 s);
    
    void setPosition(glm::vec3 _p, Voxel_Type _type);
    Voxel_Type getPosition(glm::vec3 _p);
};

#endif /* chunk_hpp */
