//
//  entity.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/19/24.
//

#ifndef entity_hpp
#define entity_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "../flecs.h"
#include "model.hpp"
#include "mesh.hpp"
#include "window.hpp"
#include "gui.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "../component/component.hpp"

void setup_flecs_entities(flecs::world& world, GLFWwindow* window) {
    //flecs window entity
    world.entity("main_window")
        .set<Component::Window>({
            window,
            static_cast<int>(SCR_WIDTH),
            static_cast<int>(SCR_HEIGHT),
        })
        .set<Component::Frame>({
            0.0f,
            0.0f
        });
    Window_Systems(world);
    
    //flecs camera entity
    world.entity("camera")
        .set([](Component::Camera& c){
            //camera
            c.position = glm::vec3(0.0f, 1.5f, 5.0f);
            c.front = glm::vec3(0.0f, 0.0f, -1.0f);
            c.up = glm::vec3(0.0f, 1.0f, 0.0f);
            c.right = glm::vec3(0.0f, 0.0f, 0.0f);
            c.world_up = glm::vec3(0.0f, 1.0f, 0.0f);
            c.yaw = CAMERA_YAW;
            c.pitch = CAMERA_PITCH;
            c.zoom = CAMERA_ZOOM;
            
            //settings
            c.settings.movement_speed = CAMERA_SPEED;
            c.settings.mouse_sensitivity = CAMERA_SENSITIVITY;
            c.settings.mouse_enable = true;
            c.settings.first_mouse = true;
            c.settings.last_x = SCR_WIDTH / 2.0f;
            c.settings.last_y = SCR_HEIGHT / 2.0f;
            
            //mouse
            c.mouse.scroll_enable = true;
        });
    Camera_Systems(world);
        
    // flecs gui entity
    world.entity("gui")
        .set([](Component::GUI& gui){
            gui.selected_entity = "cubeObject";
        });
    GUI_Systems(world);
    
    world.entity("cubeObject")
        .set([](Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st){
            //shader initialization
            sc.shading.setupShader("/Users/benkempers/Developer/game_engine/game_engine/shader/basic_lighting.vs", "/Users/benkempers/Developer/game_engine/game_engine/shader/basic_lighting.fs");
            sc.light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
            sc.light_source = false;
            
            //object transformations
            st.model = glm::mat4(1.0f);
            
            st.object_color = glm::vec3(1.0f, 0.5f, 0.31f);
            st.light_color = glm::vec3(1.0f, 1.0f, 1.0f);
        });
        
    world.entity("lightCubeObject")
        .set([](Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st){
            //shader initialization
            sc.shading.setupShader("/Users/benkempers/Developer/game_engine/game_engine/shader/light_cube.vs", "/Users/benkempers/Developer/game_engine/game_engine/shader/light_cube.fs");
            sc.light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
            sc.light_source = true;
            
            //model
            st.model = glm::mat4(1.0f);
            st.translate = sc.light_pos;
            st.scale = glm::vec3(0.2f);
            st.model = glm::translate(st.model, st.translate);
            st.model = glm::scale(st.model, st.scale); // a smaller cube
        });
    
    Cube_Systems(world);
    
//    world.entity("modelObject")
//        .set([](Component::Model& model, Component::ShaderTransformations& st){
//            //shader initialization
//            model.shader.setupShader("/Users/benkempers/Developer/game_engine/game_engine/shader/model_loading.vs", "/Users/benkempers/Developer/game_engine/game_engine/shader/model_loading.fs");
//            
//            // model object path
//            model.path = "/Users/benkempers/Developer/game_engine/game_engine/resources/backpack/backpack.obj";
//            model.directory = model.path.substr(0, model.path.find_last_of('/'));
//
//            // render the loaded model
//            st.model = glm::mat4(1.0f);
//            st.model = glm::translate(st.model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
//            st.model = glm::scale(st.model, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
//        });
//    Model_Systems(world);
//    Mesh_Systems(world);
}

#endif /* entity_hpp */
