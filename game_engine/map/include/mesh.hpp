//
//  mesh.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include <vector>
#include <string.h>
#include <set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/include/shader.hpp"

class Mesh{
public:
    struct Vertex {
        // position
        glm::vec3 position;
        // normal
        glm::vec3 normal;
        // texCoords
        glm::vec2 tex_coords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    std::vector<Texture> textures;
    unsigned int vao, vbo, ebo;
    unsigned long vao_size;
    
    Mesh();
    //~Mesh();
    void setupMesh();
    void draw();
    
private:
};

#endif /* mesh_hpp */
