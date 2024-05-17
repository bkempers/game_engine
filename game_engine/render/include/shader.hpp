//
//  shader.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/14/24.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <string>

#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
  public:
    //Variables
    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;

    //Set Uniforms
    void setBool(std::string name, bool value);
    void setInt(std::string name, int value);
    void setFloat(std::string name, float value);
    void setMat4(std::string name, const glm::mat4 mat);
    void setVec2(std::string name, const glm::vec2 vec);
    void setVec3(std::string name, const glm::vec3 vec);

    //Setup and Cleanup Function
    void setup(std::string vs, std::string fs);
    void addAttribute(int pos, std::string attrName);
    int addShader(std::string fileName, GLenum shaderType);  //General Shader Addition
    void cleanup();

    //Helper Functions
    std::string readGLSLFile(std::string fileName, int32_t &size); //Read File
    void compileShader(GLuint shader);  //Compile and Add File
    void linkProgram(); //Link the entire program
    void useProgram();  //Use the program
};

#endif /* shader_hpp */
