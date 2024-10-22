//
//  window.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/17/24.
//

#ifndef window_hpp
#define window_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/ext.hpp"

#include <math.h>
#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

#include "logger/logger.hpp"
#include "logger/quill_wrapper.h"

void Window_Systems(flecs::world& ecs) {
    ecs.system<Component::Window>("windowKeyboardCallback")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity window, Component::Window& w){
            //LOG_INFO("Entity: Initializing Window");

            flecs::entity camera_entity = ecs.lookup("camera");

            // close window
            if (glfwGetKey(w.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(w.window, GLFW_TRUE);
            // disable mouse camera movement
            if (glfwGetKey(w.window, GLFW_KEY_COMMA) == GLFW_PRESS)
            {
                camera_entity.set([w](Component::Camera& c) {
                    c.settings.mouse_enable = !c.settings.mouse_enable;
                    c.mouse.scroll_enable = !c.mouse.scroll_enable;
                    
                    if (!c.settings.mouse_enable){
                        glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    } else {
                        c.settings.first_mouse = true;
                        glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    }
                });
            }
            //wireframe mode
            if (glfwGetKey(w.window, GLFW_KEY_PERIOD) == GLFW_PRESS)
            {
                GLint polygonMode;
                glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
                if (polygonMode == GL_LINE)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                else if (polygonMode == GL_FILL)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        });
}

#endif /* window_hpp */
