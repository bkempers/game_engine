//
//  drop.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/17/24.
//

#ifndef drop_hpp
#define drop_hpp

#include <stdio.h>
#include <algorithm>
#include <functional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Drop{
public:
  //Construct Particle at Position
  Drop(glm::vec2 _pos){ pos = _pos; }
  Drop(glm::vec2 _p, glm::ivec2 dim, double v){
    pos = _p;
    int index = _p.x*dim.y+_p.y;
    volume = v;
  }

  //Properties
  int index;
  glm::vec2 pos;
  glm::vec2 speed = glm::vec2(0.0);
  double volume = 1.0;   //This will vary in time
  double sediment = 0.0; //Sediment concentration

  //Parameters
  const float dt = 1.2;
  const double density = 2.0;  //This gives varying amounts of inertia and stuff...
  const double evapRate = 0.01;
  const double depositionRate = 0.1;
  const double minVol = 0.01;
  const double friction = 0.1;
  const double volumeFactor = 100.0; //"Water Deposition Rate"

  //Sedimenation Process
  void descend(double* h, double* path, double* pool, bool* track, double* pd, glm::ivec2 dim, double scale);
  void flood(double* h, double* pool, glm::ivec2 dim);
    
 static glm::vec3 surfaceNormal(int index, double* h, glm::ivec2 dim, double scale);
    
private:
    
};

#endif /* drop_hpp */
