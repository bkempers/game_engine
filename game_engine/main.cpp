//
//  main.cpp
//  game_engine
//
//  Created by Ben Kempers on 3/5/24.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shader.hpp"
#include "texture/stb_image.h"

#include "gui/imgui/imgui.h"
#include "ecs/flecs.h"
#include "ecs/entity/entity.hpp"
#include "ecs/component/component.hpp"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Flecs ECS Initialization
flecs::world world;

flecs::query<Component::Camera> cameraQuery = world.query<Component::Camera>();

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float xoff = static_cast<float>(xoffset);
    float yoff = static_cast<float>(yoffset);
    
    flecs::entity camera_entity = world.lookup("camera");
    camera_entity.set([xoff, yoff](Component::Camera& c) {
        if(!c.mouse.scroll_enable){
            c.mouse.x_offset = xoff;
            c.mouse.y_offset = yoff;
        }
    });
}

// Define main function
int main()
{
    //Output the associated GLFW error
    glfwSetErrorCallback(error_callback);

    // Define version and compatibility settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create OpenGL window and context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "game_engine v0.0.1", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Check for window creation failure
    if (!window)
    {
        // Terminate GLFW
        glfwTerminate();
        return 1;
    }
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Set action callbacks
    glfwSetScrollCallback(window, scroll_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    
    // build and compile our shader program
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    // enable face culling
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CW);
        
    //setup flecs entity
    setup_flecs_entities(world, window);
    flecs::entity window_entity = world.lookup("main_window");
    flecs::entity gui_entity = world.lookup("gui");

    // Event loop
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    while(!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        nbFrames++;
        const Component::Frame* window_frame = window_entity.get<Component::Frame>();
        window_entity.set<Component::Frame>({
            currentFrame - window_frame->last_frame,
            currentFrame
        });
        
        if ( currentFrame - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // update gui and reset timer
            gui_entity.set([&nbFrames](Component::GUI& gui){
                    gui.fps = 1000.0/double(nbFrames);
                });
            nbFrames = 0;
            lastTime += 1.0;
        }

        // initial render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // flecs update
        world.each([&gui_entity](flecs::entity e, Component::VertexCube& vc) {
            gui_entity.set([e](Component::GUI& gui) {
                gui.entity_names.insert(e.name().c_str());
            });
        });
        world.progress();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // de-allocate all resources once they've outlived their purpose:
    flecs::query<Component::Renderer> deallocateRenderer = world.query<Component::Renderer>();
    deallocateRenderer.each([](flecs::entity e, Component::Renderer& render) {
        glDeleteVertexArrays(1, &render.VAO);
        glDeleteBuffers(1, &render.VBO);
    });
    
    flecs::query<Component::GUI> deallocateGUI = world.query<Component::GUI>();
    deallocateGUI.each([](flecs::entity gui, Component::GUI& g) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    });
    
    //gui.Remove();
    
    // Terminate GLFW
    glfwTerminate(); return 0;
}
