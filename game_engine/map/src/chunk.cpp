//
//  new_chunk.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#include "chunk.hpp"

Chunk::Chunk(){
    
}

int Chunk::getIndex(glm::vec3 _p){
  //Return the Correct Index
  if(glm::all(glm::lessThan(_p, glm::vec3(CHUNK_SIZE)))){
    return _p.x*CHUNK_SIZE*CHUNK_SIZE+_p.y*CHUNK_SIZE+_p.z;
  }
  return 0;
}

//3D Index
int Chunk::getIndex(glm::vec3 p, glm::vec3 s){
  if(glm::all(glm::lessThan(p, s)) && glm::all(glm::greaterThanEqual(p, glm::vec3(0)))){
    return p.x*s.y*s.z+p.y*s.z+p.z;
  }
  return -1;
}


void Chunk::fromOctree(Octree &octree, glm::vec3 offset){
  int LOD = 0;

  //If we are the lowest level we wish to dive, or can't dive further...
  if(octree.depth == LOD || octree.subTree.empty()){

    //Add the Values
    for(int i = 0; i < 1<<octree.depth; i++)
      for(int j = 0; j < 1<<octree.depth; j++)
        for(int k = 0; k < 1<<octree.depth; k++)
          data[getIndex(offset + glm::vec3(i, j, k), glm::vec3(chunk_size))] = octree.type;

    return;
  }

  //Iterate over all subtrees
  for(auto &_subtree:octree.subTree)
    fromOctree(_subtree, offset + glm::vec3(1<<(octree.depth-1))*octree.getPos(_subtree.index)); //Add data from the subtrees too
}

void Chunk::setPosition(glm::vec3 _p, Voxel_Type _type){
  data[getIndex(_p)].type = _type;
}

Voxel_Type Chunk::getPosition(glm::vec3 _p){
  return data[getIndex(_p)].type;
}
