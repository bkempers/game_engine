//
//  model.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/22/24.
//

#ifndef model_hpp
#define model_hpp

#include <stdio.h>
#include <vector>
#include <string.h>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "chunk.hpp"

class Model{
public:
  //Model Information
  std::vector<GLfloat> positions;
  std::vector<GLfloat> normals;
  std::vector<GLfloat> colors;
  std::vector<GLuint> indices;

  // Create variables for storing the ID of our VAO and VBO
  GLuint vbo[3], vao, ibo;

  //Model Matrix (helps translate vertices from model space to real space)
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 pos = glm::vec3(0.0f);  //Chunkspace Position!
  glm::vec3 cpos = glm::vec3(0);

  //Read File Functions
  void setup();
  void update();
  void cleanup();

  //Position Handling Functions
  void reset();
  void translate(const glm::vec3 &axis);
  void rotate(const glm::vec3 &axis, float angle);

  //Render Function to Render Stuff with the shader, so we can do this to each model individually
  void render();
};

#endif /* model_hpp */
