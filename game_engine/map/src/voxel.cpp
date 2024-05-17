//
//  voxel.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#include "voxel.hpp"

Voxel::Voxel(){
    
};

Voxel::Voxel(Voxel_Type voxel_type){
    type = voxel_type;
};

Voxel::~Voxel(){
    
};

//Setting Getters
bool Voxel::isVisible(Voxel_Type type){
  return (type != VOXEL_AIR && type != VOXEL_VOID);
}

bool Voxel::isSpawnable(Voxel_Type type){
  return (type == VOXEL_GRASS || type == VOXEL_SAND);
}

//Is the Blocks Model a Cube?
bool Voxel::isCubic(Voxel_Type type){
  return type != VOXEL_CACTUSFLOWER;
}

glm::vec4 Voxel::getColor(Voxel_Type _type, double t){
    Color color;
    //Switch the value and return a vector
    switch(_type){
        case VOXEL_GRASS:
            return color.bezier(t, color.grasscolors);
            break;
        case VOXEL_DIRT:
            return glm::vec4(0.74f, 0.5f, 0.36f, 1.0f);
            break;
        case VOXEL_WATER:
            return glm::vec4(0.3f, 0.57f, 0.67f, 0.8f); //Semi Transparent!
            break;
        case VOXEL_SAND:
            return color.bezier(t, color.sandcolors);
            break;
        case VOXEL_CLAY:
            return color.bezier(t, color.claycolors);
            break;
        case VOXEL_STONE:
            return color.bezier(t, color.stonecolors);
            break;
        case VOXEL_LEAVES:
            return color.bezier(t, color.leafcolors);
            break;
        case VOXEL_WOOD:
            return glm::vec4(0.6f, 0.375f, 0.14f, 1.0f);
            break;
        case VOXEL_GRAVEL:
            return color.bezier(t, color.gravelcolors);
            break;
        case VOXEL_SANDSTONE:
            return glm::vec4(0.8f, 0.75f, 0.64f, 1.0f);
            break;
        case VOXEL_PUMPKIN:
            return glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
            break;
        case VOXEL_CACTUS:
            return glm::vec4(0.0f, 0.44f, 0.3f, 1.0f);
            break;
        case VOXEL_PLANKS:
            return glm::vec4(0.75f, 0.6f, 0.28f, 1.0f);
            break;
//        case VOXEL_GLASS:
//            return glm::vec4(0.8f, 0.9f, 0.95f, 0.2f);
//            break;
        default:
            return glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
            break;
    }
}

