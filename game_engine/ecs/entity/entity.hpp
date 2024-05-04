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
        .set<Component::Camera>({
            glm::vec3(0.0f, 1.5f, 5.0f), //Position
            glm::vec3(0.0f, 0.0f, -1.0f), //Front
            glm::vec3(0.0f, 1.0f, 0.0f), //Up
            glm::vec3(0.0f, 0.0f, 0.0f), //Right
            glm::vec3(0.0f, 1.0f, 0.0f), //WorldUp
            CAMERA_YAW, //Yaw
            CAMERA_PITCH, //Pitch
            CAMERA_SPEED, //MovementSpeed
            CAMERA_SENSITIVITY, //MouseSensitivity
            CAMERA_ZOOM //Zoom
        })
//        .set<Component::MouseSettings>({
//            true,
//            true,
//            true
//        })
        .set<Component::CameraSettings>({
            true, //MouseEnable
            true, //FirstMouse
            SCR_WIDTH / 2.0f, //LastX
            SCR_HEIGHT / 2.0f, //LastY
            0, //XOffset
            0, //YOffset
            0, //XPosIn
            0 //YPosIn
        })
        .set<Component::Keyboard>({
            0, //Key
            0, //Action
            0, //Scancode
            0, //Mods
            0, //DeltaTime
        })
        .set<Component::MouseScroll>({
            0, //XOffset
            0, //YOffset
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
