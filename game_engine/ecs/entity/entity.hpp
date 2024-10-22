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
//#include "render.hpp"
#include "world.hpp"
#include "window.hpp"
#include "gui.hpp"
#include "camera.hpp"
#include "../component/component.hpp"

#include "logger/logger.hpp"
#include "logger/quill_wrapper.h"

void setup_flecs_entities(flecs::world& world, GLFWwindow* window) {
    LOG_INFO("Setting up FLECS entities");
    
    //flecs window entity
    world.entity("main_window")
        .set([window](Component::Window& w, Component::Frame& f){
            //window
            w.window = window;
            w.src_width = static_cast<int>(1450);
            w.src_height = static_cast<int>(800);
            
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
            c.settings.last_x = 1450 / 2.0f;
            c.settings.last_y = 800 / 2.0f;
            
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

    // flecs world entity
    world.entity("world")
        .set([](Component::World_Entity& world){
            world.world = new World();
            world.world->initialize();
        });
    World_Systems(world);
}

#endif /* entity_hpp */
