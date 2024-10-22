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
#include <vector>

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
#include "map/include/model.hpp"
#include "map/include/world.hpp"

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

    void Render(World *map, flecs::world& world);

    
private:
};


//void Renderer::loadChunkModels(World &world, flecs::world& e_world)
//{
//    flecs::entity camera_entity = e_world.lookup("camera");
//    const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//    glm::vec3 viewPos = camera_component->position;
//    
//    //Loop over all chunks
//    for(unsigned int i = 0; i < world.map.size(); i++){
//
//      //If we are at capacity, add a new item (i.e. only the newly added chunks are fully remodeled)
//      if(i == models.size()){
//        Model model;
//        model.cpos = world.map[i].c_position;
//        model.fromChunkGreedy(world.map[i]);
//        model.setup();
//        models.push_back(model);
//      }
//
//      //Old chunks need to be translated too. Translate according to player position.
//      glm::vec3 axis = (glm::vec3)(world.map[i].c_position)*(float)world.chunkSize-viewPos;
//
//      //Translate the guy
//      models[i].reset();
//      models[i].translate(axis);
//        
//      models[i].update();
//    }
//    //Make sure updateLOD is false
//    //updateLOD = false;
//}

#endif /* renderer_hpp */
