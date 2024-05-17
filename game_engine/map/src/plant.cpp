//
//  plant.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/17/24.
//

#include "plant.hpp"

void Plant::grow(){
  size += rate*(maxsize-size);
};

void Plant::root(double* density, glm::ivec2 dim, double f){
  //Can always do this one
  density[index]       += f*1.0;

  if(pos.x > 0){
    //
    density[index - 256] += f*0.6;      //(-1, 0)

    if(pos.y > 0)
      density[index - 257] += f*0.4;    //(-1, -1)

    if(pos.y < 256-1)
      density[index - 255] += f*0.4;    //(-1, 1)
  }

  if(pos.x < 256-1){
    //
    density[index + 256] += f*0.6;    //(1, 0)

    if(pos.y > 0)
      density[index + 255] += f*0.4;    //(1, -1)

    if(pos.y < 256-1)
      density[index + 257] += f*0.4;    //(1, 1)
  }

  if(pos.y > 0)
    density[index - 1]   += f*0.6;    //(0, -1)

  if(pos.y < 256-1)
    density[index + 1]   += f*0.6;    //(0, 1)
}
