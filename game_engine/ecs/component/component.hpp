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

#include "../../map/include/chunk.hpp"

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
    
    struct CameraSettings{
        bool mouse_enable;
        bool first_mouse;
        float movement_speed;
        float mouse_sensitivity;
        float last_x;
        float last_y;
    };
    
    struct Mouse{
        bool scroll_enable;
        float x_offset;
        float y_offset;
    };
    
    struct Camera{
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;
        float yaw;
        float pitch;
        float zoom;
        CameraSettings settings;
        Mouse mouse;
    };
    
    struct Keyboard{
        int key;
        int action;
        int scan_code;
        int mods;
        float delta_time;
    };
    
    struct GUI{
        bool gui;
        std::set<const char*> entity_names;
        const char* selected_entity = "NONE";
        float fps;
    };

    struct World{
        std::vector<Chunk> map;
        double lastChunkUpdate = glfwGetTime();
    };
    
    struct Renderer{
        unsigned int VBO;
        unsigned int VAO;
        Shader shader = Shader();
        Shader light_shader = Shader();
        glm::vec3 light_pos;
    };
    
    struct VertexCube{
        unsigned int VBO;
        unsigned int VAO;
    };
    
    struct ComponentShader{
        Shader shader = Shader();
        bool light_shader;
    };
    
    struct Transformation{
        glm::mat4 model;
        glm::vec3 color;
        glm::vec3 light_color;
        glm::vec3 light_pos;
        glm::vec3 translate;
        glm::vec3 scale;
        glm::vec3 rotation;
    };
    
    // MODEL FLECS STRUCT
//    struct Model {
//        std::vector<Texture> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
//        std::vector<flecs::entity> meshes; //vector of mesh entities
//        std::string directory;
//        std::string path;
//        bool gammaCorrection;
//        Shader shader = Shader();
//    };
    
    static void ReadComponent(flecs::world& ecs, flecs::entity& entity, flecs::id& id)
    {
//        if(id == ecs.id<Component::VertexCube>())
//        {
//            const VertexCube* component = entity.get<VertexCube>();
//            ImGui::Text("VBO: %d", component->VBO);
//            ImGui::Text("VAO: %d", component->VAO);
//        }
//        else if(id == ecs.id<Component::ShaderComponent>())
//        {
//            const ShaderComponent* component = entity.get<ShaderComponent>();
//            ImGui::Text("Shader: ");
//            
//            ImGui::Indent();
//            ImGui::Text("info??");
//            ImGui::Unindent();
//            
//            ImGui::Text("Light Source: %s", component->light_source ? "true" : "false");
//            ImGui::TextWrapped("Light Position: %s", glm::to_string(component->light_pos).c_str());
//        }
//        else if(id == ecs.id<Component::ShaderTransformations>())
//        {
//            const ShaderTransformations* component = entity.get<ShaderTransformations>();
//            ImGui::TextWrapped("Model: %s", glm::to_string(component->model).c_str());
//            ImGui::TextWrapped("Object Color: %s", glm::to_string(component->object_color).c_str());
//            ImGui::TextWrapped("Light Color: %s", glm::to_string(component->light_color).c_str());
//            ImGui::TextWrapped("Translate: %s", glm::to_string(component->translate).c_str());
//            ImGui::TextWrapped("Scale: %s", glm::to_string(component->scale).c_str());
//            ImGui::Text("X-Axis Rotation: %f", component->rotation_x);
//            ImGui::Text("Y-Axis Rotation: %f", component->rotation_y);
//            ImGui::Text("Z-Axis Rotation: %f", component->rotation_z);
//        }
    }
    
private:
};

#endif /* component_hpp */
