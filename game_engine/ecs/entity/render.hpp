//
//  render.hpp
//  vrge_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef RENDER_HPP
#define RENDER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../../map/include/chunk.hpp"
#include "../component/component.hpp"
#include "../flecs.h"
#include "gui.hpp"

//Cube information
float vertices[216] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void Renderer(flecs::world& ecs) {    
        ecs.system<Component::Renderer>("rendererInit")
            .kind(flecs::OnStart)
            .each([](flecs::entity cube, Component::Renderer& render){
                //GPU buffer for larger set of vertices
                glGenVertexArrays(1, &render.VAO);
                glGenBuffers(1, &render.VBO);
    
                glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
                glBindVertexArray(render.VAO);
    
                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
    
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            });

    ecs.system<Component::Renderer>("preRender")
        .kind(flecs::PreStore)
        .each([&ecs](flecs::entity cube, Component::Renderer& render){
            
        });
    
    ecs.system<Component::Renderer>("render")
        .kind(flecs::OnStore)
        .each([&ecs](flecs::entity cube, Component::Renderer& render){
            flecs::entity world_entity = ecs.lookup("world");
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
            //matrices
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera_component->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
            glm::mat4 view_matrix = glm::lookAt(camera_component->position, camera_component->position + camera_component->front, glm::vec3(0.0f, 1.0f, 0.0f));
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
//            //light space matrix
//            glm::vec3 lightInvDir = glm::vec3(0.5f, 1, 1);
//            glm::vec3 lightPos = camera_component->position + (lightInvDir * glm::vec3(10));
//            // Compute the MVP matrix from the light's point of view
//            glm::mat4 depthProjectionMatrix = glm::ortho<float>(-50, 50, -50, 50, -10, 50);
//            glm::mat4 depthViewMatrix = glm::lookAt(lightPos, camera_component->position, glm::vec3(0, 1, 0));
//            glm::mat4 depthModelMatrix = glm::mat4(1.0);
//            glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
//
//
//            render.light_shader.use();
//            render.light_shader.setMat4("lightSpaceMatrix", depthMVP);
//            glViewport(0, 0, render.light_shader.shadowWidth, render.light_shader.shadowHeight);
//            glBindFramebuffer(GL_FRAMEBUFFER, render.light_shader.depthMapFBO);
//            glClear(GL_DEPTH_BUFFER_BIT);
//
//            // render to shadowmap
//            const Component::World* world_component = world_entity.get<Component::World>();
//            for(Chunk chunk : world_component->map){
//                chunk.chunk_mesh.draw();
//            }
//
//            render.shader.use();
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            glClear(GL_DEPTH_BUFFER_BIT);
//            //glCullFace(GL_FRONT);
//            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//            render.shader.setInt("shadowMap", 1);
//            render.shader.setInt("texture1", 0);
//            render.shader.setMat4("view", view_matrix);
//            render.shader.setMat4("projection", projection_matrix);
//            render.shader.setVec3("lightPos", lightPos);
//            render.shader.setVec3("lightDir", -lightInvDir);
//            render.shader.setMat4("lightSpaceMatrix", depthMVP);
//
//            //render to screen now
//            world_component = world_entity.get<Component::World>();
//            for(Chunk chunk : world_component->map){
//                chunk.chunk_mesh.draw();
//            }
            
            // render voxel light
            render.light_shader.use();
            render.light_shader.setMat4("projection", projection_matrix);
            render.light_shader.setMat4("view", view_matrix);

            glBindVertexArray(render.VAO);

            glm::mat4 model = glm::mat4(1.0f);
            //glm::vec3 scale = glm::vec3(0.2f);
            model = glm::translate(model, render.light_pos);
            //model = glm::scale(model, scale); // a smaller cube
            render.light_shader.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            // render voxel chunks
            render.shader.use();
            //render.shader.setInt("texture1", 0);
            render.shader.setMat4("projection", projection_matrix);
            render.shader.setMat4("view", view_matrix);
            render.shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            render.shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            render.shader.setVec3("lightPos", render.light_pos);
            render.shader.setVec3("viewPos", camera_component->position);
            
            const Component::World* world_component = world_entity.get<Component::World>();
            for(Chunk chunk : world_component->map){
//                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.position.x, 0, chunk.position.y));
//                render.shader.setMat4("model", model);
                render.shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
                chunk.chunk_mesh.draw();
            }
            
            //render gui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        });
}

#endif /* render_hpp */
