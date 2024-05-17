//
//  plant.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/17/24.
//

#ifndef plant_hpp
#define plant_hpp

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Plant{
public:
  Plant(int i, glm::ivec2 d){
    index = i;
    pos = glm::vec2(i/d.y, i%d.y);
  };

  Plant(glm::vec2 p, glm::ivec2 d){
    pos = p;
    index = (int)p.x*d.y+(int)p.y;
  };

  glm::vec2 pos;
  int index;
  float size = 0.5;
  const float maxsize = 1.0;
  const float rate = 0.05;

  void grow();
  void root(double* density, glm::ivec2 dim, double factor);

  Plant& operator=(const Plant& o){
    if(this != &o){  //Self Check
      pos = o.pos;
      index = o.index;
      size = o.size;
    }
    return *this;
  };
    
private:
    
};

#endif /* plant_hpp */
