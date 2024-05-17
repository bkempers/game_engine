//
//  model.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#include "model.hpp"
#include "../map/include/chunk.hpp"

void Model::setup(){
  //Reset the Model Matrix
  reset();

  //Setup VAO and VBOs
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(3, vbo);
  glGenBuffers(1, &ibo);

  //Positions Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Color Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  //Normal Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Model::update(){
  glBindVertexArray(vao);
  //Positions Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Color Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  //Normal Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Model::cleanup(){
  glDisableVertexAttribArray(0);
  glDeleteBuffers(3, vbo);
  glDeleteVertexArrays(1, &vao);
}

//Position Functions
void Model::reset(){
  //Construct a unit matrix
  model = glm::mat4(1.0f);
}

void Model::translate(const glm::vec3 &axis){
  //Construct the Translation Matrix
  pos += axis;
  model = glm::translate(model, axis);
}

void Model::rotate(const glm::vec3 &axis, float angle){
  //Add the rotation operation to the model matrix
  model = glm::translate(glm::rotate(glm::translate(model, -pos), angle, axis), pos);
}

void Model::render(){
  //Render the Object, using the shader it has!
  glBindVertexArray(vao);
  //Bind the Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::fromChunkGreedy(Chunk chunk){
  //Clear the Containers
  positions.clear();
  colors.clear();
  normals.clear();
  indices.clear();

  cpos = chunk.position;

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
    std::array<Voxel_Type, 16*16> mask = {VOXEL_AIR};

    //Loop over the Positions
    for(x[u] = 0; x[u] < chunk.chunk_size; x[u]++){

      //Loop over a slice... (prepare mask)
      for(x[v] = 0; x[v] < chunk.chunk_size; x[v]++){
        for(x[w] = 0; x[w] < chunk.chunk_size; x[w]++){

          int s = x[w] + x[v]*chunk.chunk_size;
          Voxel_Type atPos = chunk.getPosition(glm::vec3(x[0],x[1],x[2]));
          mask[s] = VOXEL_AIR;

          //Skip Non-Cubic Elements in the Chunk! (every iteration)
          if(!Voxel::isCubic(atPos)){
            continue;
          }

          //Basically, we are facing out of the chunk, so we do take over the surface.
          if(x[u] + q[u] < 0 || x[u] + q[u] >= chunk.chunk_size){
            mask[s] = atPos;
            continue;
          }

          //Now see if we should remove this mask element or not, i.e. not visible!
          Voxel_Type facing = chunk.getPosition(glm::vec3(x[0]+q[0],x[1]+q[1],x[2]+q[2]));
          //Make sure that the facing block can be air or non-cubic!
          if(facing == VOXEL_AIR || !Voxel::isCubic(facing)){
            mask[s] = atPos;
          }
        }
      }

      //Loop AGAIN over the slice (evaluate mask)
      for(x[v] = 0; x[v] < chunk.chunk_size; x[v]++){
        for(x[w] = 0; x[w] < chunk.chunk_size; x[w]++){

          int width = 1;
          int height = 1;

          //Get the index of the current slice object we are observing!
          int s = x[w] + x[v]*chunk.chunk_size;

          //Get current blocktype, ignore air.
          Voxel_Type current = mask[s];

          //Ignore this surface if it is not part of the mask.
          if(current == VOXEL_AIR) continue;

          //Compute the Width
          while(mask[s+width] == current && x[w] + width < chunk.chunk_size) width++;

          bool done = false;
          for(height = 1; x[v] + height < chunk.chunk_size; height++) {
            //Loop over the width guy
            for(int k = 0; k < width; k++) {
              if(mask[s+k+height*chunk.chunk_size] != current) {
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
              mask[k+l*chunk.chunk_size] = VOXEL_AIR;
            }
          }

          int du[3] = {0}; du[v] = height;
          int dv[3] = {0}; dv[w] = width;

          if(n < 0){
            //Add Indices
            indices.push_back(positions.size()/3+0);
            indices.push_back(positions.size()/3+1);
            indices.push_back(positions.size()/3+2);
            indices.push_back(positions.size()/3+2);
            indices.push_back(positions.size()/3+3);
            indices.push_back(positions.size()/3+0);
            //Some need to go clock-wise, others need to go counterclockwise.
            positions.push_back(x[0]-0.5);
            positions.push_back(x[1]-0.5);
            positions.push_back(x[2]-0.5);
            //Vertex 2
            positions.push_back(x[0]+du[0]-0.5);
            positions.push_back(x[1]+du[1]-0.5);
            positions.push_back(x[2]+du[2]-0.5);
            //Vertex 3
            positions.push_back(x[0]+du[0]+dv[0]-0.5);
            positions.push_back(x[1]+du[1]+dv[1]-0.5);
            positions.push_back(x[2]+du[2]+dv[2]-0.5);
            //Vertex 4
            positions.push_back(x[0]+dv[0]-0.5);
            positions.push_back(x[1]+dv[1]-0.5);
            positions.push_back(x[2]+dv[2]-0.5);
          }
          else{
            //Add Indices
            indices.push_back(positions.size()/3+0);
            indices.push_back(positions.size()/3+1);
            indices.push_back(positions.size()/3+2);
            indices.push_back(positions.size()/3+1);
            indices.push_back(positions.size()/3+0);
            indices.push_back(positions.size()/3+3);

            //Vertex 0
            positions.push_back(x[0]-0.5+y[0]);
            positions.push_back(x[1]-0.5+y[1]);
            positions.push_back(x[2]-0.5+y[2]);
            //Vertex 1
            positions.push_back(x[0]+du[0]+dv[0]-0.5+y[0]);
            positions.push_back(x[1]+du[1]+dv[1]-0.5+y[1]);
            positions.push_back(x[2]+du[2]+dv[2]-0.5+y[2]);
            //Vertex 2
            positions.push_back(x[0]+du[0]-0.5+y[0]);
            positions.push_back(x[1]+du[1]-0.5+y[1]);
            positions.push_back(x[2]+du[2]-0.5+y[2]);
            //Vertex 3
            positions.push_back(x[0]+dv[0]-0.5+y[0]);
            positions.push_back(x[1]+dv[1]-0.5+y[1]);
            positions.push_back(x[2]+dv[2]-0.5+y[2]);
          }

          //Add Colors and Normals to all vertices.
          glm::vec4 _color = Voxel::getColor(current, color.hashrand(Chunk::getIndex(glm::vec3(x[0], x[1], x[2]), glm::vec3(16))));

          for(int m = 0; m < 4; m++){
            colors.push_back(_color.x);
            colors.push_back(_color.y);
            colors.push_back(_color.z);
            colors.push_back(_color.w);

            //Add Normals
            normals.push_back(q[0]);
            normals.push_back(q[1]);
            normals.push_back(q[2]);
          }
          //Next Quad
        }
      }
      //Next Slice
    }
    //Next Surface Orientation
  }

  //Finish!
}

/*
===============================================================================
                            MODEL CONSTRUCTION
===============================================================================
*/

void Model::addCube(glm::vec3 pos, Voxel_Type type){
  //No-Mesh
  if(!Voxel::isVisible(type)) return;

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
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((front[l*3]+pos.x));
    positions.push_back((front[l*3+1]+pos.y));
    positions.push_back((front[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(1);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((back[l*3]+pos.x));
    positions.push_back((back[l*3+1]+pos.y));
    positions.push_back((back[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(-1);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((top[l*3]+pos.x));
    positions.push_back((top[l*3+1]+pos.y));
    positions.push_back((top[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(1);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((bottom[l*3]+pos.x));
    positions.push_back((bottom[l*3+1]+pos.y));
    positions.push_back((bottom[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(-1);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((right[l*3]+pos.x));
    positions.push_back((right[l*3+1]+pos.y));
    positions.push_back((right[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(1);
    normals.push_back(0);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((left[l*3]+pos.x));
    positions.push_back((left[l*3+1]+pos.y));
    positions.push_back((left[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(-1);
    normals.push_back(0);
    normals.push_back(0);
  }

  //Get the Color
  glm::vec4 _color = Voxel::getColor(type, color.hashrand(Chunk::getIndex(pos, glm::vec3(16))));

  //Do this 6 Times, for all points for each face (6 Faces)!
  for(int i = 0; i < 4*6; i++){
    colors.push_back(_color.x);
    colors.push_back(_color.y);
    colors.push_back(_color.z);
    colors.push_back(_color.w);
  }
}

//Add an Irregular BlockType!
void Model::addIrregular(glm::vec3 pos, Voxel_Type type){
  //Load a Model or something from a file?

  /*
  We are gonna make little cactus flowers.
  */

  //No-Mesh
  if(!Voxel::isVisible(type)) return;

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
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((front[l*3]+pos.x));
    positions.push_back((front[l*3+1]+pos.y));
    positions.push_back((front[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(1);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((back[l*3]+pos.x));
    positions.push_back((back[l*3+1]+pos.y));
    positions.push_back((back[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(0);
    normals.push_back(-1);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((top[l*3]+pos.x));
    positions.push_back((top[l*3+1]+pos.y));
    positions.push_back((top[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(1);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((bottom[l*3]+pos.x));
    positions.push_back((bottom[l*3+1]+pos.y));
    positions.push_back((bottom[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(0);
    normals.push_back(-1);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+3);
  for(int l = 0; l < 4; l++){
    positions.push_back((right[l*3]+pos.x));
    positions.push_back((right[l*3+1]+pos.y));
    positions.push_back((right[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(1);
    normals.push_back(0);
    normals.push_back(0);
  }
  //Add Indices to these guys... (naive version)
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+1);
  indices.push_back(positions.size()/3+2);
  indices.push_back(positions.size()/3+0);
  indices.push_back(positions.size()/3+3);
  indices.push_back(positions.size()/3+1);
  for(int l = 0; l < 4; l++){
    positions.push_back((left[l*3]+pos.x));
    positions.push_back((left[l*3+1]+pos.y));
    positions.push_back((left[l*3+2]+pos.z));

    //Add the Appropriate Normals
    normals.push_back(-1);
    normals.push_back(0);
    normals.push_back(0);
  }

  //Do this 6 Times, for all points for each face (6 Faces)!
  for(int i = 0; i < 4*6; i++){
    colors.push_back(0.75);
    colors.push_back(0.15);
    colors.push_back(0.5);
    colors.push_back(1.0);
  }
}
