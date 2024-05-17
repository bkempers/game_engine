//
//  terrain.cpp
//  game_engine
//  These Functions are all relevant to the world generation.
//
//  Created by Ben Kempers on 5/12/24.
//

#include "terrain.hpp"
#include "world.hpp"

//Generate the terrain
void Terrain::generate(){
  //Initialize
  printf("Initializing Terrain");
  if(w->SEED == 0) w->SEED = time(NULL);
  srand(w->SEED);

  printf("Seed: %d", w->SEED);

  //Perlin Noise Initial Terrain
  perlin();

  //Do a number of cycles...
  printf("Executing Erosion and Growth Steps");
  int ncycles = 1500;
  for(int i = 0; i < ncycles; i++){
    erode(250);
    grow();
    //printf(i, ncycles);
  }

  //Generate the World from this!
  printf("Processing Height and Pool Data");
  for(int x = 0; x < rescale.x*dim.x; x++){
    for(int y = 0; y < rescale.z*dim.y; y++){

      int i = x/(int)rescale.x;
      int k = y/(int)rescale.z;

      //Interpolate Values
      double height, pool;
      float sx = (float)(x%rescale.x)/rescale.x;
      float sy = (float)(y%rescale.z)/rescale.z;
      if(i < dim.x && i < dim.y){
        height =  (1.0-sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k];
        height += (1.0-sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k];
        height += (sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k+1];
        height += (sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k+1];
        pool =  (1.0-sy)*(1.0-sx)*rescale.y*scale*waterpool[i*dim.y+k];
        pool += (1.0-sy)*(sx)*rescale.y*scale*waterpool[(i+1)*dim.y+k];
        pool += (sy)*(1.0-sx)*rescale.y*scale*waterpool[i*dim.y+k+1];
        pool += (sy)*(sx)*rescale.y*scale*waterpool[(i+1)*dim.y+k+1];
      }
      else{
        height = rescale.y*scale*heightmap[i*dim.y+k];
        pool = rescale.y*scale*waterpool[i*dim.y+k];
      }

      bool _pool = (pool > 0.0);

      //Get the Surface Normal
      glm::vec3 n = Drop::surfaceNormal(i*dim.y+k, heightmap, dim, scale);
      bool rock = (n.y < steepness);

        
        //w->blueprint.addEditBuffer(glm::vec3(x, j, y), VOXEL_STONE, false)
      //Below Surface
      for(int j = 0; j < (int)height-1; j++){
        if(rock) w->chunks[1].data[Chunk::getIndex(glm::vec3(x,j,y))].type = VOXEL_STONE;
        else w->chunks[1].data[Chunk::getIndex(glm::vec3(x,j,y))].type = VOXEL_DIRT;
      }

        //w->blueprint.addEditBuffer(glm::vec3(x, (int)height-1, y), VOXEL_STONE, false);
      //Surface
      if(rock) w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-1,y))].type = VOXEL_STONE;
      else if(!_pool) w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-1,y))].type = VOXEL_GRASS;

      //Water on Surface
      if(_pool){
          w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-2,y))].type = VOXEL_GRAVEL;
        for(int j = (int)height-1; j < (int)(height+pool); j++)
            w->chunks[1].data[Chunk::getIndex(glm::vec3(x,j,y))].type = VOXEL_WATER;
      }
    }
  }

  printf("Processing Stream Data");
  for(int x = 0; x < rescale.x*dim.x; x++){
    for(int y = 0; y < rescale.z*dim.y; y++){

      int i = x/(int)rescale.x;
      int k = y/(int)rescale.z;

      //Interpolate Values
      double height, stream;
      float sx = (float)(x%rescale.x)/rescale.x;
      float sy = (float)(y%rescale.z)/rescale.z;
      if(i < dim.x && i < dim.y){
        height =  (1.0-sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k];
        height += (1.0-sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k];
        height += (sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k+1];
        height += (sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k+1];
        stream =  (1.0-sy)*(1.0-sx)*waterpath[i*dim.y+k];
        stream += (1.0-sy)*(sx)*waterpath[(i+1)*dim.y+k];
        stream += (sy)*(1.0-sx)*waterpath[i*dim.y+k+1];
        stream += (sy)*(sx)*waterpath[(i+1)*dim.y+k+1];
      }
      else{
        height = rescale.y*scale*heightmap[i*dim.y+k];
        stream = waterpath[i*dim.y+k];
      }

      if(ease.langmuir(stream, 5.0) > 0.4){
        if(ease.langmuir(stream, 5.0) > 0.6){
            w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-1,y))].type = VOXEL_AIR;
            w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-2,y))].type = VOXEL_WATER;
            w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-3,y))].type = VOXEL_GRAVEL;
        }
        else w->chunks[1].data[Chunk::getIndex(glm::vec3(x,(int)height-1,y))].type = VOXEL_GRAVEL;
      }

    }
  }

  printf("Processing Tree Data");

  //Add trees according to the tree number and density
  for(unsigned int n = 0; n < trees.size()*rescale.x*rescale.z; n++){

    int p[2] = {rand()%(int)(rescale.x*dim.x), rand()%(int)(rescale.z*dim.y)};
    int i = p[0]/(int)rescale.x;
    int k = p[1]/(int)rescale.z;

    //Check for Tree Density
    if((float)(rand()%1000)/1000.0 > plantdensity[i*dim.y+k]) continue;

    //Interpolate Values
    double height;
    float sx = (float)(p[0]%rescale.x)/rescale.x;
    float sy = (float)(p[1]%rescale.z)/rescale.z;
    if(i < dim.x && i < dim.y){
      height =  (1.0-sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k];
      height += (1.0-sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k];
      height += (sy)*(1.0-sx)*rescale.y*scale*heightmap[i*dim.y+k+1];
      height += (sy)*(sx)*rescale.y*scale*heightmap[(i+1)*dim.y+k+1];
    }
    else height = rescale.y*scale*heightmap[i*dim.y+k];

  }

}

void Terrain::perlin(){
    noise::module::Perlin perlin;
    perlin.SetOctaveCount(8);
    perlin.SetFrequency(1.0);
    perlin.SetPersistence(0.5);

    double min, max = 0.0;
    for(int i = 0; i < dim.x*dim.y; i++){
        heightmap[i] = perlin.GetValue((i/dim.y)*(1.0/dim.x), (i%dim.y)*(1.0/dim.y), w->SEED);
        if(heightmap[i] > max) max = heightmap[i];
        if(heightmap[i] < min) min = heightmap[i];
    }

    for(int i = 0; i < dim.x*dim.y; i++)
        heightmap[i] = (heightmap[i] - min)/(max - min);
}

void Terrain::flat(){

}

void Terrain::erode(int cycles){

  //Track the Movement of all Particles
  int size = dim.x*dim.y;
  bool track[size];
  memset( track, false, size*sizeof(bool) );

  //Do a series of iterations!
  for(int i = 0; i < cycles; i++){

    //Spawn New Particle
    glm::vec2 newpos = glm::vec2(rand()%(int)dim.x, rand()%(int)dim.y);
    Drop drop(newpos);


    int spill = 5;
    while(drop.volume > drop.minVol && spill != 0){

      drop.descend(heightmap, waterpath, waterpool, track, plantdensity, dim, scale);

      if(drop.volume > drop.minVol)
        drop.flood(heightmap, waterpool, dim);

      spill--;
    }
  }

  //Update Path
  double lrate = 0.01;
  for(int i = 0; i < dim.x*dim.y; i++)
    waterpath[i] = (1.0-lrate)*waterpath[i] + lrate*((track[i])?1.0:0.0);

}

void Terrain::grow(){

  //Random Position
  {
    int i = rand()%(dim.x*dim.y);
    glm::vec3 n = Drop::surfaceNormal(i, heightmap, dim, scale);

    if( waterpool[i] == 0.0 &&
        waterpath[i] < 0.2 &&
        n.y > 0.8 ){

        Plant ntree(i, dim);
        ntree.root(plantdensity, dim, 1.0);
        trees.push_back(ntree);
    }
  }

  //Loop over all Trees
  for(int i = 0; i < trees.size(); i++){

    //Grow the Tree
    trees[i].grow();

    //Spawn a new Tree!
    if(rand()%50 == 0){
      //Find New Position
      glm::vec2 npos = trees[i].pos + glm::vec2(rand()%9-4, rand()%9-4);

      //Check for Out-Of-Bounds
      if( npos.x >= 0 && npos.x < dim.x &&
          npos.y >= 0 && npos.y < dim.y ){

        Plant ntree(npos, dim);
        glm::vec3 n = Drop::surfaceNormal(ntree.index, heightmap, dim, scale);

        if( waterpool[ntree.index] == 0.0 &&
            waterpath[ntree.index] < 0.2 &&
            n.y > 0.8 &&
            (double)(rand()%1000)/1000.0 > plantdensity[ntree.index]){
              ntree.root(plantdensity, dim, 1.0);
              trees.push_back(ntree);
            }
      }
    }

    //If the tree is in a pool or in a stream, kill it
    if(waterpool[trees[i].index] > 0.0 ||
       waterpath[trees[i].index] > 0.2 ||
       rand()%1000 == 0 ){ //Random Death Chance
         trees[i].root(plantdensity, dim, -1.0);
         trees.erase(trees.begin()+i);
         i--;
       }
  }
}
