//
//  renderer.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/18/24.
//

#ifndef renderer_hpp
#define renderer_hpp

#include <stdio.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "dependencies/flecs.h"
#include "ecs/component/component.hpp"

#include "picker.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Renderer{
public:
    Picker picker;
    unsigned int VBO;
    unsigned int VAO;
    glm::vec3 lightPos = glm::vec3(1.0f, 100.0f, 1.0f);
    
    unsigned int SCREEN_WIDTH = 1450, SCREEN_HEIGHT = 800;
    
    Renderer();
    bool Setup();
    void Cleanup();
    
    void setupShaders();
    Shader cubeShader;
    Shader lightShader;

    void Render(flecs::world& world);
    
private:
};

#endif /* renderer_hpp */
