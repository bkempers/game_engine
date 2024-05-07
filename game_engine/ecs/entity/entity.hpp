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
#include "render.hpp"
#include "world.hpp"
//#include "model.hpp"
//#include "mesh.hpp"
#include "window.hpp"
#include "gui.hpp"
#include "camera.hpp"
//#include "cube.hpp"
#include "../component/component.hpp"

void setup_flecs_entities(flecs::world& world, GLFWwindow* window) {
    //flecs window entity
    world.entity("main_window")
        .set([window](Component::Window& w, Component::Frame& f){
            //window
            w.window = window;
            w.src_width = static_cast<int>(SCR_WIDTH);
            w.src_height = static_cast<int>(SCR_HEIGHT);
            
            //frame
            f.delta_time = 0.0f;
            f.last_frame = 0.0f;
        });
    Window_Systems(world);
    
    //flecs camera entity
    world.entity("camera")
        .set([](Component::Camera& c){
            //camera
            c.position = glm::vec3(0.0f, 80.0f, 0.0);
            c.front = glm::vec3(0.0f, 0.0f, -1.0f);
            c.up = glm::vec3(0.0f, -1.0f, 0.0f);
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
            //gui.selected_entity = "cubeObject";
        });
    GUI_Systems(world);
    
    //flecs renderer entity
    world.entity("renderer")
        .set([](Component::Renderer& r){
            r.shader.setupShader("/Users/benkempers/Developer/Projects/game_engine/game_engine/shader/basic_lighting.vs", "/Users/benkempers/Developer/Projects/game_engine/game_engine/shader/basic_lighting.fs");
            
            r.light_shader.setupShader("/Users/benkempers/Developer/Projects/game_engine/game_engine/shader/light_cube.vs", "/Users/benkempers/Developer/Projects/game_engine/game_engine/shader/light_cube.fs");
            r.light_pos = glm::vec3(10.0f, 100.0f, 20.0f);
        });
    Renderer(world);
    
    // flecs world entity
    world.entity("world")
        .set([](Component::World& world){
            
        });
    World_Systems(world);
}

#endif /* entity_hpp */


//    for (int x = 0; x < 8; x++) {
//        for (int y = 0; y < 8; y++) {
//            for (int z = 0; z < 8; z++) {
//                world.entity()
//                    .set([x, y, z](Component::VertexCube& vc, Component::ComponentShader& shader, Component::Transformation& transform){
//                        //shader initialization
//                        shader.shader.setupShader("/Users/benkempers/Developer/game_engine/game_engine/shader/basic_lighting.vs", "/Users/benkempers/Developer/game_engine/game_engine/shader/basic_lighting.fs");
//                        shader.light_shader = false;
//
//                        //object transformations
//                        transform.model = glm::mat4(1.0f);
//                        transform.translate = glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
//                        transform.model = glm::translate(transform.model, transform.translate);
//                        transform.color = glm::vec3(1.0f, 0.5f, 0.31f);
//                        transform.light_color = glm::vec3(1.0f, 1.0f, 1.0f);
//                        transform.light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
//                    });
//            }
//        }
//    }
        
//    world.entity("lightCubeObject")
//        .set([](Component::VertexCube& vc, Component::ComponentShader& shader, Component::Transformation& transform){
//            //shader initialization
//            shader.shader.setupShader("/Users/benkempers/Developer/game_engine/game_engine/shader/light_cube.vs", "/Users/benkempers/Developer/game_engine/game_engine/shader/light_cube.fs");
//            shader.light_shader = true;
//
//            //model
//            transform.model = glm::mat4(1.0f);
//            transform.light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
//            transform.translate = transform.light_pos;
//            transform.scale = glm::vec3(0.2f);
//            transform.model = glm::translate(transform.model, transform.translate);
//            transform.model = glm::scale(transform.model, transform.scale); // a smaller cube
//        });
    
    //Cube_Systems(world);
    
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
