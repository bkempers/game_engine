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
#include "../../texture/texture.hpp"

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
        Texture texture = Texture();
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
        //World entity ID
        if(id == ecs.id<Component::World>())
        {
            const World* component = entity.get<World>();
            if(ImGui::TreeNode("Chunk Map")){
                int chunk_count = 0;
                for(const Chunk& chunk : component->map){
                    if(ImGui::TreeNode((void *)(intptr_t)chunk_count, "Chunk %d", chunk_count)){
                        ImGui::Text("Position: %s", glm::to_string(chunk.position).c_str());
                        
                        ImGui::TreePop();
                    }
                    chunk_count++;
                }
                ImGui::TreePop();
            }
        }
        //Camera entty ID
        else if(id == ecs.id<Component::Camera>())
        {
            const Camera* component = entity.get<Camera>();
            ImGui::TextWrapped("Position: %s", glm::to_string(component->position).c_str());
            ImGui::TextWrapped("Front: %s", glm::to_string(component->front).c_str());
            ImGui::TextWrapped("Up: %s", glm::to_string(component->up).c_str());
            ImGui::TextWrapped("Right: %s", glm::to_string(component->right).c_str());
            ImGui::TextWrapped("World Up: %s", glm::to_string(component->world_up).c_str());
            ImGui::Text("Yaw: %f", component->yaw);
            ImGui::Text("Pitch: %f", component->pitch);
            ImGui::Text("Zoom: %f", component->zoom);
            if(ImGui::TreeNode("Settings")){
                ImGui::Text("Mouse Enable: %s", component->settings.mouse_enable ? "true" : "false");
                ImGui::Text("Scroll Enable: %s", component->mouse.scroll_enable ? "true" : "false");
                ImGui::Text("Movement Speed: %f", component->settings.movement_speed);
                ImGui::Text("Mouse Sensitivity: %f", component->settings.mouse_sensitivity);
                ImGui::TreePop();
            }
            
            
//            ImGui::TextWrapped("Model: %s", glm::to_string(component->model).c_str());
//            ImGui::TextWrapped("Object Color: %s", glm::to_string(component->object_color).c_str());
//            ImGui::TextWrapped("Light Color: %s", glm::to_string(component->light_color).c_str());
//            ImGui::TextWrapped("Translate: %s", glm::to_string(component->translate).c_str());
//            ImGui::TextWrapped("Scale: %s", glm::to_string(component->scale).c_str());
//            ImGui::Text("X-Axis Rotation: %f", component->rotation_x);
//            ImGui::Text("Y-Axis Rotation: %f", component->rotation_y);
//            ImGui::Text("Z-Axis Rotation: %f", component->rotation_z);
        }
    }
    
private:
};

#endif /* component_hpp */
