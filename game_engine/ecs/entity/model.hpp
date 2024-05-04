//
//  model.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/23/24.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <stdio.h>
#include "mesh.hpp"
#include "../component/component.hpp"
#include "../flecs.h"


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Component::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, Component::Model &model)
{
    std::vector<Component::Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < model.textures_loaded.size(); j++)
        {
            if(std::strcmp(model.textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(model.textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Component::Texture texture;
            texture.id = TextureFromFile(str.C_Str(), model.directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            model.textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

flecs::entity processMesh(aiMesh *mesh, const aiScene *scene, Component::Model &model, flecs::world &world)
{
    // data to fill
    std::vector<Component::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Component::Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Component::Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
            // tangent
            if(mesh->mTangents != nullptr){
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;
            }
            // bitangent
            if(mesh->mBitangents != nullptr){
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent = vector;
            }
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Component::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", model);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Component::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", model);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Component::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", model);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Component::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", model);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    // return a mesh object created from the extracted mesh data
    flecs::entity mesh_entity = world.entity()
        .set([model, vertices, indices, textures](Component::Mesh& mesh, Component::Texture& texture, Component::Vertex& vertex, Component::ShaderComponent& shader){
            mesh.vertices = vertices;
            mesh.indices = indices;
            mesh.textures = textures;
            shader.shading = model.shader;
        });
    //Mesh_Systems(world);
    
    return mesh_entity;
    //return Mesh(vertices, indices, textures);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void processNode(aiNode *node, const aiScene *scene, Component::Model &model, flecs::world &world)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(processMesh(mesh, scene, model, world));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, model, world);
    }
}

void Model_Systems(flecs::world &ecs) {
    ecs.system<Component::Model, Component::ShaderTransformations>("setupModel")
        .kind(flecs::OnStart)
        .each([&ecs](flecs::entity model, Component::Model& mc, Component::ShaderTransformations& st) {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(mc.path, aiProcess_Triangulate | aiProcess_FlipUVs);
            
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return;
            }

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene, mc, ecs);
        });
    
    ecs.system<Component::Model, Component::ShaderTransformations>("renderModelObject")
        .kind(flecs::OnStore)
        .each([&ecs](flecs::entity model, Component::Model& mc, Component::ShaderTransformations& st){
            flecs::entity camera_entity = ecs.lookup("camera");
            const Component::Camera* camera_component = camera_entity.get<Component::Camera>();
            
            // be sure to activate shader when setting uniforms/drawing objects
            mc.shader.use();
            
            glm::mat4 projection_temp = glm::perspective(glm::radians(camera_component->Zoom), (float)1200 / (float)750, 0.1f, 100.0f);
            glm::mat4 view_temp = glm::lookAt(camera_component->Position, camera_component->Position + camera_component->Front, camera_component->Up);
            
            mc.shader.setMat4("projection", projection_temp);
            mc.shader.setMat4("view", view_temp);
            
            //world transformations
            mc.shader.setMat4("model", st.model);
        });
}

#endif /* model_hpp */
