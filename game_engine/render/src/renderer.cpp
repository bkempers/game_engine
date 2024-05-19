//
//  renderer.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/18/24.
//

#include "renderer.hpp"

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

Renderer::Renderer()
{
    
}

bool Renderer::Setup()
{
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
    
    setupShaders();
    
    picker.setup();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Renderer::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Renderer::setupShaders()
{
    //Setup Cube Shader
    cubeShader.setupShader("/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/basic_lighting.vs", "/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/basic_lighting.fs");
    lightShader.setupShader("/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/light_cube.vs", "/Users/benkempers/Developer/Projects/game_engine/game_engine/render/shaders/light_cube.fs");

}

void Renderer::Render(flecs::world& world)
{
    flecs::entity world_entity = world.lookup("world");
    flecs::entity camera_entity = world.lookup("camera");
    const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
    
    //matrices
    glm::mat4 projection_matrix = glm::perspective(glm::radians(camera_component->zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 200.0f);
    glm::mat4 view_matrix = glm::lookAt(camera_component->position, camera_component->position + camera_component->front, glm::vec3(0.0f, 1.0f, 0.0f));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    //=======================
    // RENDER LIGHT CUBE SHADER
    //=======================
    lightShader.use();
    lightShader.setMat4("projection", projection_matrix);
    lightShader.setMat4("view", view_matrix);

    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    //glm::vec3 scale = glm::vec3(0.2f);
    model = glm::translate(model, lightPos);
    //model = glm::scale(model, scale); // a smaller cube
    lightShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //=======================
    // RENDER VOXEL CHUNKS
    //=======================
    cubeShader.use();
    //render.shader.setInt("texture1", 0);
    cubeShader.setMat4("projection", projection_matrix);
    cubeShader.setMat4("view", view_matrix);
    cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("lightPos", lightPos);
    cubeShader.setVec3("viewPos", camera_component->position);
    
    const Component::World* world_component = world_entity.get<Component::World>();
    for(Chunk chunk : world_component->map){
//                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.position.x, 0, chunk.position.y));
//                render.shader.setMat4("model", model);
        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        chunk.chunk_mesh.draw();
    }
}
