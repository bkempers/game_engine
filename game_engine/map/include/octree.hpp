//
//  octree.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#ifndef octree_hpp
#define octree_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "voxel.hpp"
//#include "chunk.hpp"
#include "vector"

class Chunk;

class Octree{
public:
  //Needs to be set initially.
  Voxel_Type type = VOXEL_AIR;
  std::vector<Octree> subTree;  //Sparse SubTree
  unsigned char depth = 4;                    //Current Depth
  unsigned char index = 0;                    //Current Index

  //Back and forth to chunks
  void fromChunk(Chunk &chunk);

  //Filling and Combining Operations for Subtrees
  bool trySimplify();
  bool contains(glm::vec3 _pos);

  //Index Functions
  glm::vec3 getPos(int index);
  int getIndex(glm::ivec3 _pos);

  //We need setters, getters, volume setters, etc.
  bool setPosition(glm::vec3 _pos, Voxel_Type _type);
  Voxel_Type getPosition(glm::vec3 _pos, int LOD);
};

#endif /* octree_hpp */
