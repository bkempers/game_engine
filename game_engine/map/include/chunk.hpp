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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "voxel.hpp"
#include "../perlin_noise.hpp"

class Chunk{
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int CHUNK_HEIGHT = 32;
    static constexpr int CHUNK_RENDER_RADIUS = 8;
    static constexpr int SEED = 123456u;

    //generation
    siv::PerlinNoise perlin{ SEED };

    Voxel chunk_voxels[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    Mesh chunk_mesh;
    glm::vec2 position;
    
    Chunk();
    Chunk(glm::vec2 position);
    //~Chunk();
    void loadVoxels();
    void createMesh();
    void createCube(int x, int y, int z);
    Voxel_Type generateTerrain(int x, int y, int z);
    
private:
    
};

#endif /* chunk_hpp */
