//
//  camera.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/10/24.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

#include "logger/logger.hpp"
#include "logger/quill_wrapper.h"

// Default camera values
const float CAMERA_YAW         = -90.0f;
const float CAMERA_PITCH       =  0.0f;
const float CAMERA_SPEED       =  15.0f;
const float CAMERA_SENSITIVITY =  0.10f;
const float CAMERA_ZOOM        =  45.0f;

void Camera_Systems(flecs::world& ecs) {
    ecs.system<Component::Camera>("mouseCallback")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity camera, Component::Camera& c){
            //LOG_INFO("Entity: Initializing Camera");

            flecs::entity window_entity = ecs.lookup("main_window");
            const Component::Window* window_component = window_entity.get<Component::Window>();
            double xpos, ypos;
            glfwGetCursorPos(window_component->window, &xpos, &ypos);
            
            float xpos_in = static_cast<float>(xpos);
            float ypos_in = static_cast<float>(ypos);
            
            if (c.settings.mouse_enable) {
                if (c.settings.first_mouse)
                {
                    c.settings.last_x = xpos_in;
                    c.settings.last_y = ypos_in;
                    c.settings.first_mouse = false;
                }

                float x_offset = xpos_in - c.settings.last_x;
                float y_offset = c.settings.last_y - ypos_in; // reversed since y-coordinates go from bottom to top

                c.settings.last_x = xpos_in;
                c.settings.last_y = ypos_in;
                
                x_offset *= c.settings.mouse_sensitivity;
                y_offset *= c.settings.mouse_sensitivity;
                
                c.yaw   += x_offset;
                c.pitch += y_offset;

                // make sure that when pitch is out of bounds, screen doesn't get flipped
                if (true)
                {
                    if (c.pitch > 89.0f)
                        c.pitch = 89.0f;
                    if (c.pitch < -89.0f)
                        c.pitch = -89.0f;
                }
                
                // calculate the new Front vector
                glm::vec3 front;
                front.x = cos(glm::radians(c.yaw)) * cos(glm::radians(c.pitch));
                front.y = sin(glm::radians(c.pitch));
                front.z = sin(glm::radians(c.yaw)) * cos(glm::radians(c.pitch));
                c.front = glm::normalize(front);
                // also re-calculate the Right and Up vector
                c.right = glm::normalize(glm::cross(c.front, c.world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
                c.up    = glm::normalize(glm::cross(c.right, c.front));
            }
        });
    
    ecs.system<Component::Camera> ("processMouseScroll")
        .kind(flecs::OnLoad)
        .each([](flecs::entity camera, Component::Camera& c) {
            // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
            c.zoom -= c.mouse.y_offset;
            if (c.zoom < 1.0f)
                c.zoom = 1.0f;
            if (c.zoom > 45.0f)
                c.zoom = 45.0f;
        });
    
    ecs.system<Component::Camera>("processKeyboard")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity camera, Component::Camera& c){
            flecs::entity window_entity = ecs.lookup("main_window");
            const Component::Window* window_component = window_entity.get<Component::Window>();
            const Component::Frame* window_frame = window_entity.get<Component::Frame>();
            float velocity = c.settings.movement_speed * window_frame->delta_time;
            if (glfwGetKey(window_component->window, GLFW_KEY_W) == GLFW_PRESS) {
                c.position += c.front * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_S) == GLFW_PRESS) {
                c.position -= c.front * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_A) == GLFW_PRESS) {
                c.position -= c.right * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_D) == GLFW_PRESS) {
                c.position += c.right * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                c.position += c.up * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                c.position -= c.up * velocity;
            }
        });
}

#endif /* CAMERA_HPP */
