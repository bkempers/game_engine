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
#include "obb_picking.hpp"
#include "../component/component.hpp"
#include "../flecs.h"

void Window_Systems(flecs::world& ecs) {
    ecs.system<Component::Window>("windowKeyboardCallback")
        .kind(flecs::OnLoad)
        .each([&ecs](flecs::entity window, Component::Window& w){
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
//             wireframe mode
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
    
//    ecs.system<Component::Window>("mouseClickCallback")
//        .kind(flecs::OnLoad)
//        .each([&ecs](flecs::entity window, Component::Window& w){
//            flecs::entity camera_entity = ecs.lookup("camera");
//            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//            
//            flecs::entity cube_entity = ecs.lookup("cubeObject");
//            const Component::Picking* cube_model = cube_entity.get<Component::Picking>();
//            
//            if (glfwGetMouseButton(w.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//            {
//                double xpos, ypos;
//                glfwGetCursorPos(w.window, &xpos, &ypos);
//                ypos = SCR_HEIGHT - ypos - 1;
//                
//                glBindFramebuffer(GL_READ_FRAMEBUFFER, cube_model->fbo);
//                glReadBuffer(GL_COLOR_ATTACHMENT0);
//
//                Component::PixelInfo Pixel;
//                glReadPixels(xpos, ypos, 1, 1, GL_RGB, GL_FLOAT, &Pixel);
//                
//                printf("object id: %f\n", Pixel.object_id);
//                printf("draw id: %f\n", Pixel.draw_id);
//                printf("prim id: %f\n", Pixel.prim_id);
//
//                glReadBuffer(GL_NONE);
//                glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//                
//                if(Pixel.prim_id != 0){
//                    printf("mouse left clicked on cube object1\n");
//                    // register that object was clicked??
//                    cube_entity.set([Pixel](Component::ShaderTransformations& st, Component::PixelInfo& pixel) {
//                        st.object_color = glm::vec3(0.0f, 0.5f, 0.31f);
//                        pixel.object_id = Pixel.object_id;
//                        pixel.draw_id = Pixel.draw_id;
//                        pixel.prim_id = Pixel.prim_id;
//                    });
//                }
//                else{
//                    cube_entity.set([Pixel](Component::ShaderTransformations& st, Component::PixelInfo& pixel) {
//                        st.object_color = glm::vec3(1.0f, 0.5f, 0.31f);
//                        pixel.object_id = Pixel.object_id;
//                        pixel.draw_id = Pixel.draw_id;
//                        pixel.prim_id = Pixel.prim_id;
//                    });
//                }
//            }
//        });
    
//    ecs.system<Component::Window>("mouseClickCallback")
//        .kind(flecs::OnLoad)
//        .each([&ecs](flecs::entity window, Component::Window& w){
//            flecs::entity camera_entity = ecs.lookup("camera");
//            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//            
//            flecs::entity gui_entity = ecs.lookup("gui");
//            const Component::GUI* gui_component = gui_entity.get<Component::GUI>();
//            
//            flecs::entity cube_entity = ecs.lookup(gui_component->selected_entity);
//            
//            if (glfwGetMouseButton(w.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//            {
//                if(cube_entity){
//                    const Component::ShaderTransformations* cube_model = cube_entity.get<Component::ShaderTransformations>();
//                    
//                    // Step 1 - Viewport to NDC
//                    double xpos, ypos;
//                    glfwGetCursorPos(w.window, &xpos, &ypos);
//                    
//                    float x = (2.0f * xpos) / SCR_WIDTH - 1.0f;
//                    float y = 1.0f - (2.0f * ypos) / SCR_HEIGHT;
//                    float z = 1.0f;
//                    
//                    // Step 2 - NDC to view
//                    glm::vec3 ray_nds = glm::vec3(x, y, z);
//                    double pi = 3.14159265359;
//                    float radians = (45.0f / 2.0f) * (pi / 180);
//                    float focal_length = 1.0f/tanf(radians);
//                    float ar = (float)SCR_HEIGHT / (float)SCR_WIDTH;
//                    glm::vec4 ray_clip(ray_nds.x / focal_length, (ray_nds.y * ar) / focal_length, 1.0f, 1.0f);
//                    //glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
//                    
//                    // Step 3 - intersect view vector with object Z plane (in view)
//                    //Vector4f view_space_intersect = Vector4f(ray_clip * m_objViewSpacePos.z, 1.0f);
//
//                    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//                    glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
//                    //ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
//                    
//                    // Step 4 - View to World space
//                    glm::mat4 view_matrix = glm::lookAt(camera_component->Position, camera_component->Position + camera_component->Front, camera_component->Up);
//                    printf("VIEW MATRIX: %s\n", glm::to_string(view_matrix).c_str());
//                    printf("VIEW MATRIX INVERSE: %s\n", glm::to_string(glm::inverse(view_matrix)).c_str());
//                    
//                    glm::vec3 ray_wor = glm::inverse(view_matrix) * ray_eye;
//                    // don't forget to normalise the vector at some point
//                    ray_wor = glm::normalize(ray_wor);
//                    
//                    printf("RAY WOR: %s\n", glm::to_string(ray_wor).c_str());
//                    printf("MODEL: %s\n\n", glm::to_string(cube_model->model).c_str());
//                                        
//                    cube_entity.set([ray_wor](Component::ShaderTransformations& st) {
//                        st.model = glm::translate(st.model, ray_wor);
//                    });
//                }
//            }
//        });
    
//    ecs.system<Component::Window>("mouseClickCallback")
//        .kind(flecs::OnLoad)
//        .each([&ecs](flecs::entity window, Component::Window& w){
//            flecs::entity camera_entity = ecs.lookup("camera");
//            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
//            
//            flecs::entity gui_entity = ecs.lookup("gui");
//            const Component::GUI* gui_component = gui_entity.get<Component::GUI>();
//                        
//            if (glfwGetMouseButton(w.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//            {
//                //SCREEN POS TO WORLD RAY
//                glm::mat4 projection_matrix = glm::perspective(glm::radians(camera_component->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//                glm::mat4 view_matrix = glm::lookAt(camera_component->Position, camera_component->Position + camera_component->Front, camera_component->Up);
//                glm::vec3 ray_origin;
//                glm::vec3 ray_direction;
//                
//                // The ray Start and End positions, in Normalized Device Coordinates
//                double xpos, ypos;
//                glfwGetCursorPos(w.window, &xpos, &ypos);
//                glm::vec4 lRayStart_NDC(
//                    ((float)xpos/(float)SCR_WIDTH  - 0.5f) * 2.0f,
//                    ((float)ypos/(float)SCR_HEIGHT - 0.5f) * 2.0f,
//                    -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
//                    1.0f
//                );
//                glm::vec4 lRayEnd_NDC(
//                    ((float)xpos/(float)SCR_WIDTH  - 0.5f) * 2.0f,
//                    ((float)ypos/(float)SCR_HEIGHT - 0.5f) * 2.0f,
//                    0.0,
//                    1.0f
//                );
//
//                // The Projection matrix goes from Camera Space to NDC.
//                // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
//                glm::mat4 projection_matrix_inverse = glm::inverse(projection_matrix);
//                
//                // The View Matrix goes from World Space to Camera Space.
//                // So inverse(ViewMatrix) goes from Camera Space to World Space.
//                glm::mat4 view_matrix_inverse = glm::inverse(view_matrix);
//                
//                glm::vec4 lRayStart_camera = projection_matrix_inverse * lRayStart_NDC;    lRayStart_camera/=lRayStart_camera.w;
//                glm::vec4 lRayStart_world  = view_matrix_inverse       * lRayStart_camera; lRayStart_world /=lRayStart_world .w;
//                glm::vec4 lRayEnd_camera   = projection_matrix_inverse * lRayEnd_NDC;      lRayEnd_camera  /=lRayEnd_camera  .w;
//                glm::vec4 lRayEnd_world    = view_matrix_inverse       * lRayEnd_camera;   lRayEnd_world   /=lRayEnd_world   .w;
//
//                // Faster way (just one inverse)
//                //glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
//                //glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
//                //glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;
//
//                glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
//                lRayDir_world = glm::normalize(lRayDir_world);
//                ray_origin = glm::vec3(lRayStart_world);
//                ray_direction = glm::normalize(lRayDir_world);
//
//                //TEST RAY OBB INTERSECTION
//                // Intersection method from Real-Time Rendering and Essential Mathematics for Games
//                float intersection_distance;
//                glm::vec3 aabb_min(-1.0f, -1.0f, -1.0f);
//                glm::vec3 aabb_max( 1.0f,  1.0f,  1.0f);
//                
//                for(const char* object : gui_component->entity_names){
//                    flecs::entity cube_entity = ecs.lookup(object);
//                    const Component::ShaderTransformations* cube_model = cube_entity.get<Component::ShaderTransformations>();
//                    if(test_rayobb_intersection(ray_origin, ray_direction, aabb_min, aabb_max, cube_model->model, intersection_distance)){
//                        printf("clicked on an object!!\n");
//                    }
//                }
//            }
//        });
}

#endif /* window_hpp */
