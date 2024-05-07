//
//  chunk.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#include "../include/chunk.hpp"

Chunk::Chunk(){
    chunk_mesh = Mesh();
    position = glm::vec2(0, 0);
//            for (int i = 0; i < CHUNK_SIZE; i++) {
//                chunk_voxels[i] = new Voxel * [CHUNK_SIZE];
//                for (int j = 0; j < CHUNK_SIZE; j++) {
//                    chunk_voxels[i][j] = new Voxel[CHUNK_SIZE];
//                }
//            }
};

Chunk::Chunk(glm::vec2 chunk_position){
    chunk_mesh = Mesh();
    position = chunk_position;
};
    
//Chunk::~Chunk(){
//    //std::cout << "Chunk destroyed: " << this << std::endl;
////            for (int i = 0; i < CHUNK_SIZE; ++i) {
////                for (int j = 0; j < CHUNK_SIZE; ++j) {
////                    delete[] chunk_voxels[i][j];
////             }
////                delete[] chunk_voxels[i];
////            }
////                delete[] chunk_voxels;
//};
    
void Chunk::loadVoxels(){
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int actualX = x + CHUNK_SIZE * position.x;
                int actualY = z + CHUNK_SIZE * position.y;
                
                Voxel_Type type = generateTerrain(actualX, y, actualY);
                chunk_voxels[x][y][z] = Voxel(type);
            }
        }
    }
};
    
void Chunk::createMesh(){
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
//                if (chunk_voxels[x][y][z].active == false) { // Don't create triangle data for inactive blocks
//                    continue;
//                } else {
                    createCube(x, y, z);
//                }
            }
        }
    }
    
    //setup chunk mesh for rendering
    chunk_mesh.setupMesh();
};
    
void Chunk::createCube(int x, int y,  int z){
    glm::vec3 pos(x + position.x * CHUNK_SIZE, y, z + position.y * CHUNK_SIZE);
    //test all faces (occlusion culling)
    //std::cout << "CHUNKS::Render ID: " << blocks[x][y][z].ID << std::endl;
    //right face
    if (x + 1 >= CHUNK_SIZE || chunk_voxels[x + 1][y][z].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, RIGHT_FACE, chunk_mesh.vertices, chunk_mesh.indices);
    }
    //left face
    if (x - 1 < 0 || chunk_voxels[x - 1][y][z].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, LEFT_FACE, chunk_mesh.vertices, chunk_mesh.indices);
    }
    //top face
    if (y + 1 >= CHUNK_SIZE || chunk_voxels[x][y + 1][z].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, TOP, chunk_mesh.vertices, chunk_mesh.indices);
    }
    //bottom face
    if (y - 1 < 0 || chunk_voxels[x][y - 1][z].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, BOTTOM, chunk_mesh.vertices, chunk_mesh.indices);
    }
    //back face
    if (z + 1 >= CHUNK_SIZE || chunk_voxels[x][y][z + 1].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, BACK, chunk_mesh.vertices, chunk_mesh.indices);
    }
    //front face
    if (z - 1 < 0 || chunk_voxels[x][y][z - 1].type == DEFAULT){
        chunk_voxels[x][y][z].loadFace(pos, FRONT, chunk_mesh.vertices, chunk_mesh.indices);
    }
};

Voxel_Type Chunk::generateTerrain(int x, int y, int z){
    double noise = perlin.octave2D_01((x * 0.01), (z * 0.01), 4);
    noise = pow(noise, 3);
    int height = noise * 70;
    if (y > height) {
        return DEFAULT;
    }
    else if (y == height) {
        //snow
        int variance = ((float)rand() / RAND_MAX) * 7; //between 0-7
        if (y > 20 + variance)
            return STONE;    //snow
        return GRASS;        //grassK
    }
    else {
        return DIRT;        //dirt
    }
};
