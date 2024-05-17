//
//  picker.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/17/24.
//

#ifndef picker_hpp
#define picker_hpp

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

class Picker{
public:
  Shader shaderColorPick;
  glm::mat4 model = glm::mat4(1.0f);
  GLuint vbo, vao;
  void setup();
};

#endif /* picker_hpp */
