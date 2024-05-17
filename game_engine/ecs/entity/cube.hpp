//
//  cube.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/18/24.
//

#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

void Cube_Systems(flecs::world& ecs) {
    unsigned int VBO, VAO;
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
    
    //GPU buffer for larger set of vertices
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
//    ecs.system<Component::VertexCube>("cubeInitialization")
//        .kind(flecs::OnStart)
//        .each([](flecs::entity cube, Component::VertexCube& vc){
//            //GPU buffer for larger set of vertices
//            glGenVertexArrays(1, &vc.VAO);
//            glGenBuffers(1, &vc.VBO);
//            
//            glBindBuffer(GL_ARRAY_BUFFER, vc.VBO);
//            glBufferData(GL_ARRAY_BUFFER, sizeof(vc.vertices), vc.vertices, GL_STATIC_DRAW);
//            
//            glBindVertexArray(vc.VAO);
//
//            // position attribute
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//            glEnableVertexAttribArray(0);
//            // normal attribute
//            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//            glEnableVertexAttribArray(1);
//            
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            glBindVertexArray(0);
//        });

    ecs.system<Component::VertexCube, Component::ComponentShader, Component::Transformation>("renderCubeObject")
        .kind(flecs::OnStore)
        .each([&ecs, VAO](flecs::entity cube, Component::VertexCube& vc, Component::ComponentShader& shader, Component::Transformation& transform){
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
            // be sure to activate shader when setting uniforms/drawing objects
            shader.shader.use();

            if (!shader.light_shader) {
                shader.shader.setVec3("objectColor", transform.color.x, transform.color.y, transform.color.z);
                shader.shader.setVec3("lightColor", transform.light_color.x, transform.light_color.y, transform.light_color.z);
                shader.shader.setVec3("lightPos", transform.light_pos);
                shader.shader.setVec3("viewPos", camera_component->position);
            }
            
            glm::mat4 projection_temp = glm::perspective(glm::radians(camera_component->zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view_temp = glm::lookAt(camera_component->position, camera_component->position + camera_component->front, camera_component->up);
            
            shader.shader.setMat4("projection", projection_temp);
            shader.shader.setMat4("view", view_temp);
            
            //world transformations
            shader.shader.setMat4("model", transform.model);
            
            // render the cube
            //glBindVertexArray(vc.VAO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        });
}

#endif /* cube_hpp */
