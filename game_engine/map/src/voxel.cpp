//
//  voxel.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#include "voxel.hpp"

glm::vec3 unitVertices[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)
};

// Define the vertex indices for each face (two triangles per face)
unsigned int faceVertices[6][4] = {
    {3, 2, 6, 7},
    {1, 0, 4, 5},
    {4, 0, 3, 7},
    {1, 5, 6, 2},
    // FRONT (0, 1, 2, 3)
    {0, 1, 2, 3},
    // BACK (5, 4, 7, 6)
    {5, 4, 7, 6}
};

Voxel::Voxel(){
    
};

Voxel::Voxel(Voxel_Type voxel_type){
    type = voxel_type;
};

Voxel::~Voxel(){
    
};

std::unordered_map<uint8_t, Voxel::VoxelTexture> Voxel::voxelTextures;
void Voxel::Initialization(){
    voxelTextures[GRASS] = { getTexCoords(0), getTexCoords(3), getTexCoords(2) }; // Grass
    voxelTextures[DIRT] = { getTexCoords(2), getTexCoords(2), getTexCoords(2) }; // Dirt
    voxelTextures[STONE] = { getTexCoords(1), getTexCoords(1), getTexCoords(1) }; // Stone
    voxelTextures[SNOW] = { getTexCoords(66), getTexCoords(66), getTexCoords(66) }; //Snow Block
}

glm::vec2 Voxel::getTexCoords(uint8_t pos) {
    int x = pos % CANVAS_SIZE;
    int y = pos / CANVAS_SIZE;
    return glm::vec2(x * CELL_SIZE, y * CELL_SIZE);
}

void Voxel::loadFace(glm::vec3 position, Voxel_Face face, std::vector<Mesh::Vertex>& vertices, std::vector<unsigned int>& indices){
    if (type == DEFAULT) // Air block check
        return;
    
    glm::vec2 offset = glm::vec2(0.0f);
    if (face == TOP) {
        offset = voxelTextures[type].topTexture;
    }
    else if (face == BOTTOM) {
        offset = voxelTextures[type].bottomTexture;
    }
    else {
        offset = voxelTextures[type].sideTexture;
    }

    glm::vec2 uvCoordinates[4] = {
        offset + glm::vec2(0.0f, CELL_SIZE),         // Bottom Left
        offset + glm::vec2(CELL_SIZE, CELL_SIZE),   // Bottom Right
        offset + glm::vec2(CELL_SIZE, 0.0f),         // Top Right
        offset + glm::vec2(0.0f, 0.0f)              // Top Left
    };

    unsigned int indexOffset = vertices.size();
    // Append vertices for the specified face
    for (int i = 0; i < 4; i++) {
        unsigned int vIndex = faceVertices[faceToIndex(face)][i];
        Mesh::Vertex v;
        v.position = unitVertices[vIndex] + position;
        v.normal = vectorMapping(face);
        v.tex_coords = uvCoordinates[i];
        vertices.push_back(v);
    }

    // Append indices for the specified face (two triangles)
    indices.push_back(indexOffset);
    indices.push_back(indexOffset + 1);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset);
    indices.push_back(indexOffset + 2);
    indices.push_back(indexOffset + 3);
};

glm::vec3 Voxel::vectorMapping(Voxel_Face face){
    switch(face){
        case TOP: return glm::vec3(0, 1, 0);
        case BOTTOM: return glm::vec3(0, -1, 0);
        case LEFT_FACE: return glm::vec3(-1, 0, 0);
        case RIGHT_FACE: return glm::vec3(1, 0, 0);
        case FRONT: return glm::vec3(0, 0, -1);
        case BACK: return glm::vec3(0, 0, 1);
            
        default: return glm::vec3(0, 0, 0); // Or some error code
    }
};

int Voxel::faceToIndex(Voxel_Face face) {
    switch (face) {
        case TOP: return 0;
        case BOTTOM: return 1;
        case LEFT_FACE: return 2;
        case RIGHT_FACE: return 3;
        case FRONT: return 4;
        case BACK: return 5;
        default: return -1; // Or some error code
    }
};

glm::vec4 Voxel::getColor(Voxel_Type type){
  switch(type)
  {
    case STONE:
        return glm::vec4(0.39f, 0.39f, 0.36f, 1.0f);
        break;
    case DIRT:
        return glm::vec4(0.74f, 0.5f, 0.36f, 1.0f);
        break;
    case WATER:
        return glm::vec4(0.3f, 0.57f, 0.67f, 0.8f); //Semi Transparent!
        break;
    case WOOD:
        return glm::vec4(0.6f, 0.375f, 0.14f, 1.0f);
        break;
    default:
        return glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
        break;
  }
};

