//
//  component.hpp
//  game_engine
//
//  Created by Ben Kempers on 4/10/24.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <vector>
#include <string.h>
#include <set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <GLFW/glfw3.h>
#include "../../shader/shader.hpp"

#define MAX_BONE_INFLUENCE 4

// settings
unsigned int SCR_WIDTH = 1450;
unsigned int SCR_HEIGHT = 775;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};

class Component
{
public:
    struct Window{
        GLFWwindow* window;
        int src_width;
        int src_height;
    };
    
    struct Frame{
        float delta_time;
        float last_frame;
    };
    
    struct Camera{
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        float Yaw;
        float Pitch;
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;
    };
    
//    struct MouseSettings{
//        bool MouseEnable;
//        bool FirstMouse;
//        bool ScrollEnable;
//    };
    
    struct CameraSettings{
        bool MouseEnable;
        bool FirstMouse;
        float LastX;
        float LastY;
        float XOffset;
        float YOffset;
        float XPosIn;
        float YPosIn;
    };
    
    struct MouseScroll{
        bool ScrollEnable;
        float XOffset;
        float YOffset;
    };
    
    struct Keyboard{
        int Key;
        int Action;
        int ScanCode;
        int Mods;
        float DeltaTime;
    };
    
    struct VertexCube{
        unsigned int VBO;
        unsigned int VAO;
        GLuint frame_buffer;
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
    };
    
    struct ShaderComponent{
        Shader shading = Shader();
        bool light_source;
        glm::vec3 light_pos;
    };
    
    struct ShaderTransformations{
        glm::mat4 model;
        glm::vec3 object_color;
        glm::vec3 light_color;
        glm::vec3 translate;
        glm::vec3 scale;
        float rotation_x;
        float rotation_y;
        float rotation_z;
    };
    
    struct GUI{
        bool gui;
        std::set<const char*> entity_names;
        const char* selected_entity = "None";
        float fps;
    };
    
    // MODEL FLECS STRUCT
    struct Vertex {
        // position
        glm::vec3 position;
        // normal
        glm::vec3 normal;
        // texCoords
        glm::vec2 tex_coords;
        // tangent
        glm::vec3 tangent;
        // bitangent
        glm::vec3 bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };
    
    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };
    
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
    };
    
    struct Model {
        std::vector<Texture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<flecs::entity> meshes; //vector of mesh entities
        std::string directory;
        std::string path;
        bool gammaCorrection;
        Shader shader = Shader();
    };
    
    static void ReadComponent(flecs::world& ecs, flecs::entity& entity, flecs::id& id)
    {
        if(id == ecs.id<Component::VertexCube>())
        {
            const VertexCube* component = entity.get<VertexCube>();
            ImGui::Text("VBO: %d", component->VBO);
            ImGui::Text("VAO: %d", component->VAO);
            ImGui::Text("Frame Buffer: %u", component->frame_buffer);
        }
        else if(id == ecs.id<Component::ShaderComponent>())
        {
            const ShaderComponent* component = entity.get<ShaderComponent>();
            ImGui::Text("Shader: ");
            
            ImGui::Indent();
            ImGui::Text("info??");
            ImGui::Unindent();
            
            ImGui::Text("Light Source: %s", component->light_source ? "true" : "false");
            ImGui::TextWrapped("Light Position: %s", glm::to_string(component->light_pos).c_str());
        }
        else if(id == ecs.id<Component::ShaderTransformations>())
        {
            const ShaderTransformations* component = entity.get<ShaderTransformations>();
            ImGui::TextWrapped("Model: %s", glm::to_string(component->model).c_str());
            ImGui::TextWrapped("Object Color: %s", glm::to_string(component->object_color).c_str());
            ImGui::TextWrapped("Light Color: %s", glm::to_string(component->light_color).c_str());
            ImGui::TextWrapped("Translate: %s", glm::to_string(component->translate).c_str());
            ImGui::TextWrapped("Scale: %s", glm::to_string(component->scale).c_str());
            ImGui::Text("X-Axis Rotation: %f", component->rotation_x);
            ImGui::Text("Y-Axis Rotation: %f", component->rotation_y);
            ImGui::Text("Z-Axis Rotation: %f", component->rotation_z);
        }
    }
    
private:
};

#endif /* component_hpp */
