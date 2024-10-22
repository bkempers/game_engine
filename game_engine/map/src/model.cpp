//
//  model.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/22/24.
//

#include "model.hpp"

void Model::setup(){
  //Reset the Model Matrix
  reset();

  //Setup VAO and VBOs
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(3, vbo);
  glGenBuffers(1, &ibo);

  //Positions Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Color Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  //Normal Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Model::update(){
  glBindVertexArray(vao);
  //Positions Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(GLfloat), &positions[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Color Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

  //Normal Buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Model::cleanup(){
  glDisableVertexAttribArray(0);
  glDeleteBuffers(3, vbo);
  glDeleteVertexArrays(1, &vao);
}

//Position Functions
void Model::reset(){
  //Construct a unit matrix
  model = glm::mat4(1.0f);
}

void Model::translate(const glm::vec3 &axis){
  //Construct the Translation Matrix
  pos += axis;
  model = glm::translate(model, axis);
}

void Model::rotate(const glm::vec3 &axis, float angle){
  //Add the rotation operation to the model matrix
  model = glm::translate(glm::rotate(glm::translate(model, -pos), angle, axis), pos);
}

void Model::render(){
  //Render the Object, using the shader it has!
  glBindVertexArray(vao);
  //Bind the Index Buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
