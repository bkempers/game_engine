//
//  chunk.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef chunk_hpp
#define chunk_hpp

#include <stdio.h>
#include <vector>
#include <string.h>
#include <set>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "voxel.hpp"
#include "model.hpp"
#include "../perlin_noise.hpp"
#include "dependencies/libnoise2/include/noise/noise.h"

class Chunk{
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_HEIGHT = 64;
    static constexpr int CHUNK_RENDER_RADIUS = 8;
    static constexpr int SEED = 123456u;

    Voxel chunk_voxels[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    Voxel voxels[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE] = {};
    glm::vec2 position;
    glm::vec3 c_position;
    int size = 16;
    
    Chunk();
    Chunk(glm::vec2 position);
    //~Chunk();
    
    void loadVoxels();
    
    //MESH CREATION
    Mesh chunk_mesh;
    void createMesh();
    void createCube(int x, int y, int z);
    
    //MODEL CREATION (GREEDY)
    Model model;
    void fromChunkGreedy();

    //Models for Different Block-Types
    void addIrregular(glm::vec3 pos, Voxel_Type type);
    void addCube(glm::vec3 pos, Voxel_Type type);
    
    int getPosition(glm::vec3 _pos);
    
    //generation
    siv::PerlinNoise perlin{ SEED };
    
    Voxel_Type generateTerrain(int x, int y, int z);
    Voxel_Type biome(float e, float m);
    
private:
    
};

#endif /* chunk_hpp */
