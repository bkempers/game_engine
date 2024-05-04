//
//  cube.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/18/24.
//

#ifndef CUBE_HPP
#define CUBE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

void Cube_Systems(flecs::world& ecs) {
    ecs.system<Component::VertexCube, Component::ShaderComponent, Component::ShaderTransformations>("cubeInitialization")
        .kind(flecs::OnStart)
        .each([](flecs::entity cube, Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st){
            //GPU buffer for larger set of vertices
            glGenVertexArrays(1, &vc.VAO);
            glGenBuffers(1, &vc.VBO);
            
            glBindBuffer(GL_ARRAY_BUFFER, vc.VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vc.vertices), vc.vertices, GL_STATIC_DRAW);
            
            glBindVertexArray(vc.VAO);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        });
    
    ecs.system<Component::VertexCube, Component::ShaderComponent, Component::ShaderTransformations>("renderCubeObject")
        .kind(flecs::OnStore)
        .each([&ecs](flecs::entity cube, Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st){
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
            // be sure to activate shader when setting uniforms/drawing objects
            sc.shading.use();

            if (!sc.light_source) {
                sc.shading.setVec3("objectColor", st.object_color.x, st.object_color.y, st.object_color.z);
                sc.shading.setVec3("lightColor", st.light_color.x, st.light_color.y, st.light_color.z);
                sc.shading.setVec3("lightPos", sc.light_pos);
                sc.shading.setVec3("viewPos", camera_component->Position);
            }
            
            glm::mat4 projection_temp = glm::perspective(glm::radians(camera_component->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view_temp = glm::lookAt(camera_component->Position, camera_component->Position + camera_component->Front, camera_component->Up);
            
            sc.shading.setMat4("projection", projection_temp);
            sc.shading.setMat4("view", view_temp);
            
            //world transformations
            sc.shading.setMat4("model", st.model);
            
            // render the cube
            glBindVertexArray(vc.VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        });
    
//    ecs.system<Component::VertexCube, Component::ShaderComponent, Component::ShaderTransformations, Component::PixelInfo, Component::Picking>("cubePickingInitialization")
//        .kind(flecs::OnStart)
//        .each([](flecs::entity cube, Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st, Component::PixelInfo& pixel, Component::Picking& pick){
//            // Create the FBO
//            glGenFramebuffers(1, &pick.fbo);
//            glBindFramebuffer(GL_FRAMEBUFFER, pick.fbo);
//
//            // Create the texture object for the primitive information buffer
//            glGenTextures(1, &pick.picking_texture);
//            glBindTexture(GL_TEXTURE_2D, pick.picking_texture);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT,
//                        0, GL_RGB, GL_FLOAT, NULL);
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
//                        pick.picking_texture, 0);
//
//            // Create the texture object for the depth buffer
//            glGenTextures(1, &pick.depth_texture);
//            glBindTexture(GL_TEXTURE_2D, pick.depth_texture);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT,
//                        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
//                        pick.depth_texture, 0);
//
//            // Disable reading to avoid problems with older GPUs
//            glReadBuffer(GL_NONE);
//
//            glDrawBuffer(GL_COLOR_ATTACHMENT0);
//
//            // Verify that the FBO is correct
//            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//            if (Status != GL_FRAMEBUFFER_COMPLETE) {
//                printf("FB error, status: 0x%x\n", Status);
//            }
//
//            // Restore the default framebuffer
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        });
//    
//    ecs.system<Component::VertexCube, Component::ShaderComponent, Component::ShaderTransformations, Component::PixelInfo, Component::Picking>("pickingObjectPhase")
//        .kind(flecs::PreStore)
//        .each([&ecs](flecs::entity cube, Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st, Component::PixelInfo& pixel, Component::Picking& pick){
//            flecs::entity camera_entity = ecs.lookup("camera");
//            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pick.fbo);
//
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            sc.shading.setFloat("objectID", (float)cube);
//
//            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//        });
//    
//    ecs.system<Component::VertexCube, Component::ShaderComponent, Component::ShaderTransformations, Component::PixelInfo, Component::Picking>("renderCubePickingObject")
//        .kind(flecs::OnStore)
//        .each([&ecs](flecs::entity cube, Component::VertexCube& vc, Component::ShaderComponent& sc, Component::ShaderTransformations& st, Component::PixelInfo& pixel, Component::Picking& pick){
//            flecs::entity camera_entity = ecs.lookup("camera");
//            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//                        
//            // be sure to activate shader when setting uniforms/drawing objects
//            sc.shading.use();
//
//            if (!sc.light_source) {
//                sc.shading.setVec3("objectColor", st.object_color.x, st.object_color.y, st.object_color.z);
//                sc.shading.setVec3("lightColor", st.light_color.x, st.light_color.y, st.light_color.z);
//                sc.shading.setVec3("lightPos", sc.light_pos);
//                sc.shading.setVec3("viewPos", camera_component->Position);
//            }
//            
//            glm::mat4 projection_temp = glm::perspective(glm::radians(camera_component->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//            glm::mat4 view_temp = glm::lookAt(camera_component->Position, camera_component->Position + camera_component->Front, camera_component->Up);
//            
//            sc.shading.setMat4("projection", projection_temp);
//            sc.shading.setMat4("view", view_temp);
//            
//            //world transformations
//            sc.shading.setMat4("model", st.model);
//            
//            // render the cube
//            glBindVertexArray(vc.VAO);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        });
}

#endif /* cube_hpp */
