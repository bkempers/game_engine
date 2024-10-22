//
//  chunk.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#include "../include/chunk.hpp"

Chunk::Chunk(){
    //chunk_mesh = Mesh();
    model.setup();
    position = glm::vec2(0, 0);
//            for (int i = 0; i < CHUNK_SIZE; i++) {
//                chunk_voxels[i] = new Voxel * [CHUNK_SIZE];
//                for (int j = 0; j < CHUNK_SIZE; j++) {
//                    chunk_voxels[i][j] = new Voxel[CHUNK_SIZE];
//                }
//            }
};

Chunk::Chunk(glm::vec2 chunk_position){
    //chunk_mesh = Mesh();
    
    //model.setup();
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

void Chunk::fromChunkGreedy(){
  //Clear the Containers
  model.positions.clear();
  model.colors.clear();
  model.normals.clear();
  model.indices.clear();

  //Loop over all 6 possible orientations...
  for(int d = 0; d < 6; d++){

    //Our actual dimensional indices
    int u = (d/2+0)%3;  //u = 0, 0, 1, 1, 2, 2
    int v = (d/2+1)%3;  //v = 1, 1, 2, 2, 0, 0
    int w = (d/2+2)%3;  //w = 2, 2, 0, 0, 1, 1

    int x[3] = {0};
    int q[3] = {0};
    int y[3] = {0};

    int n = 2*(d%2)-1;  //Normal Direction
    q[u] = n;           //Normal Vector
    y[u] = 1;           //Simple Vector

    //Slice Mask
    std::array<Voxel_Type, 16*16> mask = {AIR};

    //Loop over the Positions
    for(x[u] = 0; x[u] < CHUNK_SIZE; x[u]++){

      //Loop over a slice... (prepare mask)
      for(x[v] = 0; x[v] < CHUNK_SIZE; x[v]++){
        for(x[w] = 0; x[w] < CHUNK_SIZE; x[w]++){

          int s = x[w] + x[v]*CHUNK_SIZE;
          Voxel_Type atPos = voxels[getPosition(glm::vec3(x[0],x[1],x[2]))].type;
          mask[s] = AIR;

//          //Skip Non-Cubic Elements in the Chunk! (every iteration)
//          if(!block::isCubic(atPos)){
//            //If it is our first pass, also add it to the meshing editBuffer.
//            if(d == 0) temp.addEditBuffer(glm::vec3(x[0], x[1], x[2]), atPos, false);
//            continue;
//          }

          //Basically, we are facing out of the chunk, so we do take over the surface.
          if(x[u] + q[u] < 0 || x[u] + q[u] >= CHUNK_SIZE){
            mask[s] = atPos;
            continue;
          }

          //Now see if we should remove this mask element or not, i.e. not visible!
          Voxel_Type facing = voxels[getPosition(glm::vec3(x[0]+q[0],x[1]+q[1],x[2]+q[2]))].type;
          //Make sure that the facing block can be air or non-cubic!
          if(facing == AIR){
            mask[s] = atPos;
          }
        }
      }

      //Loop AGAIN over the slice (evaluate mask)
      for(x[v] = 0; x[v] < CHUNK_SIZE; x[v]++){
        for(x[w] = 0; x[w] < CHUNK_SIZE; x[w]++){

          int width = 1;
          int height = 1;

          //Get the index of the current slice object we are observing!
          int s = x[w] + x[v]*CHUNK_SIZE;

          //Get current blocktype, ignore air.
          Voxel_Type current = mask[s];

          //Ignore this surface if it is not part of the mask.
          if(current == AIR) continue;

          //Compute the Width
          while(mask[s+width] == current && x[w] + width < CHUNK_SIZE) width++;

          bool done = false;
          for(height = 1; x[v] + height < CHUNK_SIZE; height++) {
            //Loop over the width guy
            for(int k = 0; k < width; k++) {
              if(mask[s+k+height*CHUNK_SIZE] != current) {
                done = true;
                break;
              }
            }
            if(done) {
              break;
            }
          }

          //Zero Mask
          for(int l = x[v]; l < x[v] + height; l++){
            for(int k = x[w]; k < x[w] + width; k++){
              mask[k+l*CHUNK_SIZE] = AIR;
            }
          }

          int du[3] = {0}; du[v] = height;
          int dv[3] = {0}; dv[w] = width;

          if(n < 0){
              //Add Indices
              model.indices.push_back(model.positions.size()/3+0);
              model.indices.push_back(model.positions.size()/3+1);
              model.indices.push_back(model.positions.size()/3+2);
              model.indices.push_back(model.positions.size()/3+2);
              model.indices.push_back(model.positions.size()/3+3);
              model.indices.push_back(model.positions.size()/3+0);
              //Some need to go clock-wise, others need to go counterclockwise.
              model.positions.push_back(x[0]-0.5);
              model.positions.push_back(x[1]-0.5);
              model.positions.push_back(x[2]-0.5);
              //Vertex 2
              model.positions.push_back(x[0]+du[0]-0.5);
              model.positions.push_back(x[1]+du[1]-0.5);
              model.positions.push_back(x[2]+du[2]-0.5);
              //Vertex 3
              model.positions.push_back(x[0]+du[0]+dv[0]-0.5);
              model.positions.push_back(x[1]+du[1]+dv[1]-0.5);
              model.positions.push_back(x[2]+du[2]+dv[2]-0.5);
              //Vertex 4
              model.positions.push_back(x[0]+dv[0]-0.5);
              model.positions.push_back(x[1]+dv[1]-0.5);
              model.positions.push_back(x[2]+dv[2]-0.5);
          }
          else{
              //Add Indices
              model.indices.push_back(model.positions.size()/3+0);
              model.indices.push_back(model.positions.size()/3+1);
              model.indices.push_back(model.positions.size()/3+2);
              model.indices.push_back(model.positions.size()/3+1);
              model.indices.push_back(model.positions.size()/3+0);
              model.indices.push_back(model.positions.size()/3+3);

            //Vertex 0
              model.positions.push_back(x[0]-0.5+y[0]);
              model.positions.push_back(x[1]-0.5+y[1]);
              model.positions.push_back(x[2]-0.5+y[2]);
            //Vertex 1
              model.positions.push_back(x[0]+du[0]+dv[0]-0.5+y[0]);
              model.positions.push_back(x[1]+du[1]+dv[1]-0.5+y[1]);
              model.positions.push_back(x[2]+du[2]+dv[2]-0.5+y[2]);
            //Vertex 2
              model.positions.push_back(x[0]+du[0]-0.5+y[0]);
              model.positions.push_back(x[1]+du[1]-0.5+y[1]);
              model.positions.push_back(x[2]+du[2]-0.5+y[2]);
            //Vertex 3
              model.positions.push_back(x[0]+dv[0]-0.5+y[0]);
              model.positions.push_back(x[1]+dv[1]-0.5+y[1]);
              model.positions.push_back(x[2]+dv[2]-0.5+y[2]);
          }

          //Add Colors and Normals to all vertices.
          glm::vec4 _color = Voxel::getColor(current);

          for(int m = 0; m < 4; m++){
              model.colors.push_back(_color.x);
              model.colors.push_back(_color.y);
              model.colors.push_back(_color.z);
              model.colors.push_back(_color.w);

            //Add Normals
              model.normals.push_back(q[0]);
              model.normals.push_back(q[1]);
              model.normals.push_back(q[2]);
          }
          //Next Quad
        }
      }
      //Next Slice
    }
    //Next Surface Orientation
  }

//  //Mesh all the guys that could not be meshed greedily with large quads.
//  while(!temp.editBuffer.empty()){
//    //For now, we add cubes
//    addIrregular(temp.editBuffer.back().pos, temp.editBuffer.back().type);
//    temp.editBuffer.pop_back();
//  }
  //Finish!
}

/*
===============================================================================
                            MODEL CONSTRUCTION
===============================================================================
*/

void Chunk::addCube(glm::vec3 pos, Voxel_Type type){
  //No-Mesh
  if(type == DEFAULT || type == AIR) return;

  GLfloat front[12] = { -0.5,  0.5, 0.5,
                         0.5,  0.5, 0.5,
                         0.5, -0.5, 0.5,
                        -0.5, -0.5, 0.5};

  GLfloat back[12] = {  -0.5,  0.5, -0.5,
                         0.5, -0.5, -0.5,
                         0.5,  0.5, -0.5,
                        -0.5, -0.5, -0.5};

  GLfloat top[12] = {   -0.5,  0.5,  0.5,
                         0.5,  0.5, -0.5,
                         0.5,  0.5,  0.5,
                        -0.5,  0.5, -0.5};

  GLfloat bottom[12] = {-0.5, -0.5,  0.5,
                         0.5, -0.5,  0.5,
                         0.5, -0.5, -0.5,
                        -0.5, -0.5, -0.5};

  GLfloat right[12] = {  0.5, -0.5,  0.5,
                         0.5,  0.5,  0.5,
                         0.5,  0.5, -0.5,
                         0.5, -0.5, -0.5};

   GLfloat left[12] = { -0.5, -0.5,  0.5,
                        -0.5,  0.5, -0.5,
                        -0.5,  0.5,  0.5,
                        -0.5, -0.5, -0.5};

  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((front[l*3]+pos.x));
      model.positions.push_back((front[l*3+1]+pos.y));
      model.positions.push_back((front[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(0);
      model.normals.push_back(1);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((back[l*3]+pos.x));
      model.positions.push_back((back[l*3+1]+pos.y));
      model.positions.push_back((back[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(0);
      model.normals.push_back(-1);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((top[l*3]+pos.x));
      model.positions.push_back((top[l*3+1]+pos.y));
      model.positions.push_back((top[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(1);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((bottom[l*3]+pos.x));
      model.positions.push_back((bottom[l*3+1]+pos.y));
      model.positions.push_back((bottom[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(-1);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((right[l*3]+pos.x));
      model.positions.push_back((right[l*3+1]+pos.y));
      model.positions.push_back((right[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(1);
      model.normals.push_back(0);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((left[l*3]+pos.x));
      model.positions.push_back((left[l*3+1]+pos.y));
      model.positions.push_back((left[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(-1);
      model.normals.push_back(0);
      model.normals.push_back(0);
  }

  //Get the Color
  glm::vec4 _color = Voxel::getColor(type);

  //Do this 6 Times, for all points for each face (6 Faces)!
  for(int i = 0; i < 4*6; i++){
      model.colors.push_back(_color.x);
      model.colors.push_back(_color.y);
      model.colors.push_back(_color.z);
      model.colors.push_back(_color.w);
  }
}

//Add an Irregular BlockType!
void Chunk::addIrregular(glm::vec3 pos, Voxel_Type type){
  //Load a Model or something from a file?

  /*
  We are gonna make little cactus flowers.
  */

  //No-Mesh
  if(type == DEFAULT || type == AIR) return;

  GLfloat front[12] = { -0.25,  0.0, 0.25,
                         0.25,  0.0, 0.25,
                         0.25, -0.5, 0.25,
                        -0.25, -0.5, 0.25};

  GLfloat back[12] = {  -0.25,  0.0, -0.25,
                         0.25, -0.5, -0.25,
                         0.25,  0.0, -0.25,
                        -0.25, -0.5, -0.25};

  GLfloat top[12] = {   -0.25,  0.0,  0.25,
                         0.25,  0.0, -0.25,
                         0.25,  0.0,  0.25,
                        -0.25,  0.0, -0.25};

  GLfloat bottom[12] = {-0.25, -0.5,  0.25,
                         0.25, -0.5,  0.25,
                         0.25, -0.5, -0.25,
                        -0.25, -0.5, -0.25};

  GLfloat right[12] = {  0.25, -0.5,  0.25,
                         0.25,  0.0,  0.25,
                         0.25,  0.0, -0.25,
                         0.25, -0.5, -0.25};

   GLfloat left[12] = { -0.25, -0.5,  0.25,
                        -0.25,  0.0, -0.25,
                        -0.25,  0.0,  0.25,
                        -0.25, -0.5, -0.25};

  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((front[l*3]+pos.x));
      model.positions.push_back((front[l*3+1]+pos.y));
      model.positions.push_back((front[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(0);
      model.normals.push_back(1);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((back[l*3]+pos.x));
      model.positions.push_back((back[l*3+1]+pos.y));
      model.positions.push_back((back[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(0);
      model.normals.push_back(-1);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((top[l*3]+pos.x));
      model.positions.push_back((top[l*3+1]+pos.y));
      model.positions.push_back((top[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(1);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((bottom[l*3]+pos.x));
      model.positions.push_back((bottom[l*3+1]+pos.y));
      model.positions.push_back((bottom[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(0);
      model.normals.push_back(-1);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+3);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((right[l*3]+pos.x));
      model.positions.push_back((right[l*3+1]+pos.y));
      model.positions.push_back((right[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(1);
      model.normals.push_back(0);
      model.normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+1);
    model.indices.push_back(model.positions.size()/3+2);
    model.indices.push_back(model.positions.size()/3+0);
    model.indices.push_back(model.positions.size()/3+3);
    model.indices.push_back(model.positions.size()/3+1);
  for(int l = 0; l < 4; l++){
      model.positions.push_back((left[l*3]+pos.x));
      model.positions.push_back((left[l*3+1]+pos.y));
      model.positions.push_back((left[l*3+2]+pos.z));

    //Add the Appropriate Normals
      model.normals.push_back(-1);
      model.normals.push_back(0);
      model.normals.push_back(0);
  }

  //Do this 6 Times, for all points for each face (6 Faces)!
  for(int i = 0; i < 4*6; i++){
      model.colors.push_back(0.75);
      model.colors.push_back(0.15);
      model.colors.push_back(0.5);
      model.colors.push_back(1.0);
  }
}

int Chunk::getPosition(glm::vec3 _pos)
{
  //Return the Correct Index
  if(glm::all(glm::lessThan(_pos, glm::vec3(size)))){
    return _pos.x*size*size+_pos.y*size+_pos.z;
  }
  return 0;
}

//====================================
// TERRAIN GENERATION
//===================================

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

Voxel_Type Chunk::biome(float e, float m)
{
    // these thresholds will need tuning to match your generator
//    if (e < 0.1) return WATER;
//    if (e < 0.12) return SAND;
//    
//    if (e > 0.8) {
//      if (m < 0.1) return SCORCHED;
//      if (m < 0.2) return BARE;
//      if (m < 0.5) return TUNDRA;
//      return SNOW;
//    }
//
//    if (e > 0.6) {
//      if (m < 0.33) return TEMPERATE_DESERT;
//      if (m < 0.66) return SHRUBLAND;
//      return TAIGA;
//    }
//
//    if (e > 0.3) {
//      if (m < 0.16) return TEMPERATE_DESERT;
//      if (m < 0.50) return GRASSLAND;
//      if (m < 0.83) return TEMPERATE_DECIDUOUS_FOREST;
//      return TEMPERATE_RAIN_FOREST;
//    }
//
//    if (m < 0.16) return SUBTROPICAL_DESERT;
//    if (m < 0.33) return GRASSLAND;
//    if (m < 0.66) return TROPICAL_SEASONAL_FOREST;
//    return TROPICAL_RAIN_FOREST;
    return GRASS;
};
