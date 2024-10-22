//
//  picker.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/17/24.
//

#include "picker.hpp"

void Picker::setup(){
  shaderColorPick.setupShader("/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/pick/colorPick.vs",
                              "/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/pick/colorPick.fs");
  shaderColorPick.addAttribute("in_Position", 0);

  //Setup the VAO and stuff
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);

  GLfloat cubewire[48] = {   -0.5,  0.5,  0.5,
                             -0.5,  0.5, -0.5,
                              0.5,  0.5, -0.5,
                              0.5,  0.5,  0.5,
                             -0.5,  0.5,  0.5,
                             -0.5, -0.5,  0.5,
                             -0.5, -0.5, -0.5,
                             -0.5,  0.5, -0.5,
                             -0.5, -0.5, -0.5,
                              0.5, -0.5, -0.5,
                              0.5,  0.5, -0.5,
                              0.5, -0.5, -0.5,
                              0.5, -0.5,  0.5,
                              0.5,  0.5,  0.5,
                              0.5, -0.5,  0.5,
                             -0.5, -0.5,  0.5};

  //Based on what is picked, draw an outline.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 48*sizeof(GLfloat), &cubewire[0], GL_STATIC_DRAW); //3*4
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
