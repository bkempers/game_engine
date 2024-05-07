//
//  mesh.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/24/24.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdio.h>
#include "../component/component.hpp"
#include "../flecs.h"

void Mesh_Systems(flecs::world& ecs)
{
    ecs.system<Component::Vertex, Component::Texture, Component::Mesh, Component::ComponentShader>("setupMesh")
        .kind(flecs::OnStart)
        .each([&ecs](flecs::entity model, Component::Vertex &vertex, Component::Texture &tex, Component::Mesh &mesh, Component::ComponentShader& shader) {
            // create buffers/arrays
            glGenVertexArrays(1, &mesh.VAO);
            glGenBuffers(1, &mesh.VBO);
            glGenBuffers(1, &mesh.EBO);

            glBindVertexArray(mesh.VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Component::Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, normal));
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, tex_coords));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, bitangent));
            // ids
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, m_BoneIDs));

            // weights
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Component::Vertex), (void*)offsetof(Component::Vertex, m_Weights));
            glBindVertexArray(0);

        });
    
    ecs.system<Component::Vertex, Component::Texture, Component::Mesh, Component::ComponentShader>("renderMesh")
        .kind(flecs::OnStore)
        .each([&ecs](flecs::entity model,Component::Vertex &vertex, Component::Texture &tex, Component::Mesh &mesh, Component::ComponentShader &shader){
            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;
            for(unsigned int i = 0; i < mesh.textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name = mesh.textures[i].type;
                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to string
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                 else if(name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to string

                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader.shader.ID, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
            }
            
            // draw mesh
            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);
        });
}

#endif /* mesh_hpp */
