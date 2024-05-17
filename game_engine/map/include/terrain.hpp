//
//  terrain.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#ifndef terrain_hpp
#define terrain_hpp

#include <stdio.h>
#include <algorithm>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../../deps/libnoise2/include/noise/noise.h>

//#include "world.hpp"
#include "plant.hpp"
#include "drop.hpp"
#include "../perlin_noise.hpp"
#include "../../helpers/ease.h"

class World;
class Terrain{
public:
  Terrain(World* _w){
    w = _w;
  };

  World* w;                             //World Pointer
    Ease ease;

  glm::ivec2 dim = glm::vec2(256, 256);    //Size of the heightmap array
  glm::ivec3 rescale = glm::ivec3(4, 3, 4);  //Scale to World-Size

  double scale = 80.0;                  //"Physical" Height scaling of the map
  double heightmap[256*256] = {0.0};    //Flat Array
  double waterpath[256*256] = {0.0};    //Water Path Storage (Rivers)
  double waterpool[256*256] = {0.0};    //Water Pool Storage (Lakes / Ponds)
  float steepness = 0.6;

  std::vector<Plant> trees;
  double plantdensity[256*256] = {0.0}; //Density for Plants

  void generate();

  //
  void perlin();
  void flat();

  //
  void erode(int cycles);               //Erode with N Particles
  void grow();
};

#endif /* terrain_hpp */
