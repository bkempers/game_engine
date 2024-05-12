//
//  voxel.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef voxel_hpp
#define voxel_hpp

#include <stdio.h>
#include <vector>
#include <string.h>
#include <set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"

//canvas is a square
const int CANVAS_SIZE = 0.5;
const float CELL_SIZE = 1.0f/CANVAS_SIZE;

// Voxel enums
enum Voxel_Type {
    DEFAULT = 0,
    GRASS,
    DIRT,
    WATER,
    STONE,
    SNOW,
    WOOD,
    SAND,
    NUM_TYPES
};

enum Voxel_Face{
    TOP = 0x01, // 0000 0001
    BOTTOM = 0x02, // 0000 0010
    LEFT_FACE = 0x04, // 0000 0100
    RIGHT_FACE = 0x08, // 0000 1000
    FRONT = 0x10, // 0001 0000
    BACK = 0x20  // 0010 0000
};

class Voxel{
public:
    bool active;
    Voxel_Type type;
    
    Voxel();
    Voxel(Voxel_Type type);
    ~Voxel();
    static void Initialization();
    static glm::vec2 getTexCoords(uint8_t pos);
    
    void loadFace(glm::vec3 position, Voxel_Face face, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices);
    glm::vec3 vectorMapping(Voxel_Face face);
    int faceToIndex(Voxel_Face face);

private:
    struct VoxelTexture {
        glm::vec2 topTexture;
        glm::vec2 sideTexture;
        glm::vec2 bottomTexture;
    };
    
    static std::unordered_map<uint8_t, VoxelTexture> voxelTextures;
};

#endif /* voxel_hpp */
