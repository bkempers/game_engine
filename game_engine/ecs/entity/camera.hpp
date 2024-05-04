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

// Default camera values
const float CAMERA_YAW         = -90.0f;
const float CAMERA_PITCH       =  0.0f;
const float CAMERA_SPEED       =  3.0f;
const float CAMERA_SENSITIVITY =  0.10f;
const float CAMERA_ZOOM        =  45.0f;

void Camera_Systems(flecs::world& ecs) {
    ecs.system<Component::Camera, Component::CameraSettings>("mouseCallback")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity camera, Component::Camera& c, Component::CameraSettings& cs){
            flecs::entity window_entity = ecs.lookup("main_window");
            const Component::Window* window_component = window_entity.get<Component::Window>();
            double xpos, ypos;
            glfwGetCursorPos(window_component->window, &xpos, &ypos);
            
            cs.XPosIn = static_cast<float>(xpos);
            cs.YPosIn = static_cast<float>(ypos);
            
            if (cs.MouseEnable) {
                if (cs.FirstMouse)
                {
                    cs.LastX = cs.XPosIn;
                    cs.LastY = cs.YPosIn;
                    cs.FirstMouse = false;
                }

                cs.XOffset = cs.XPosIn - cs.LastX;
                cs.YOffset = cs.LastY - cs.YPosIn; // reversed since y-coordinates go from bottom to top

                cs.LastX = cs.XPosIn;
                cs.LastY = cs.YPosIn;
                
                cs.XOffset *= c.MouseSensitivity;
                cs.YOffset *= c.MouseSensitivity;
                
                c.Yaw   += cs.XOffset;
                c.Pitch += cs.YOffset;

                // make sure that when pitch is out of bounds, screen doesn't get flipped
                if (true)
                {
                    if (c.Pitch > 89.0f)
                        c.Pitch = 89.0f;
                    if (c.Pitch < -89.0f)
                        c.Pitch = -89.0f;
                }
                
                // calculate the new Front vector
                glm::vec3 front;
                front.x = cos(glm::radians(c.Yaw)) * cos(glm::radians(c.Pitch));
                front.y = sin(glm::radians(c.Pitch));
                front.z = sin(glm::radians(c.Yaw)) * cos(glm::radians(c.Pitch));
                c.Front = glm::normalize(front);
                // also re-calculate the Right and Up vector
                c.Right = glm::normalize(glm::cross(c.Front, c.WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
                c.Up    = glm::normalize(glm::cross(c.Right, c.Front));
            }
        });
    
    ecs.system<Component::Camera, Component::MouseScroll> ("processMouseScroll")
        .kind(flecs::OnLoad)
        .each([](flecs::entity camera, Component::Camera& c, Component::MouseScroll& ms) {
            // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
            c.Zoom -= ms.YOffset;
            if (c.Zoom < 1.0f)
                c.Zoom = 1.0f;
            if (c.Zoom > 45.0f)
                c.Zoom = 45.0f;
        });
    
    ecs.system<Component::Camera, Component::Keyboard>("processKeyboard")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity camera, Component::Camera& c, Component::Keyboard& key){
            flecs::entity window_entity = ecs.lookup("main_window");
            const Component::Window* window_component = window_entity.get<Component::Window>();
            const Component::Frame* window_frame = window_entity.get<Component::Frame>();
            float velocity = c.MovementSpeed * window_frame->delta_time;
            if (glfwGetKey(window_component->window, GLFW_KEY_W) == GLFW_PRESS) {
                c.Position += c.Front * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_S) == GLFW_PRESS) {
                c.Position -= c.Front * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_A) == GLFW_PRESS) {
                c.Position -= c.Right * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_D) == GLFW_PRESS) {
                c.Position += c.Right * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                c.Position += c.Up * velocity;
            }
            if (glfwGetKey(window_component->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                c.Position -= c.Up * velocity;
            }
        });

    ecs.system<Component::Camera, Component::CameraSettings>("processMouseMovement")
        .kind(flecs::OnUpdate)
        .each([](flecs::entity camera, Component::Camera& c, Component::CameraSettings& cs){
            cs.XOffset *= c.MouseSensitivity;
            cs.YOffset *= c.MouseSensitivity;

            c.Yaw   += cs.XOffset;
            c.Pitch += cs.YOffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (true)
            {
                if (c.Pitch > 89.0f)
                    c.Pitch = 89.0f;
                if (c.Pitch < -89.0f)
                    c.Pitch = -89.0f;
            }
            
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(c.Yaw)) * cos(glm::radians(c.Pitch));
            front.y = sin(glm::radians(c.Pitch));
            front.z = sin(glm::radians(c.Yaw)) * cos(glm::radians(c.Pitch));
            c.Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            c.Right = glm::normalize(glm::cross(c.Front, c.WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            c.Up    = glm::normalize(glm::cross(c.Right, c.Front));
            
            //std::cout << "Camera entity: " << camera.name() << " updated\n";
        });
}

#endif /* CAMERA_HPP */
