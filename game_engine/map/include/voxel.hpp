//
//  voxel.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef voxel_hpp
#define voxel_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../helpers/color.h"

// Voxel enums
enum Voxel_Type {
    VOXEL_AIR = 0,
    VOXEL_VOID,
    VOXEL_GRASS,
    VOXEL_DIRT,
    VOXEL_GRAVEL,
    VOXEL_WATER,
    VOXEL_CLAY,
    VOXEL_STONE,
    VOXEL_SNOW,
    VOXEL_WOOD,
    VOXEL_PLANKS,
    VOXEL_SAND,
    VOXEL_SANDSTONE,
    VOXEL_LEAVES,
    VOXEL_PUMPKIN,
    VOXEL_CACTUS,
    VOXEL_CACTUSFLOWER
};

class Voxel{
public:
    Voxel_Type type;
    
    Voxel();
    Voxel(Voxel_Type type);
    ~Voxel();
    static bool isVisible(Voxel_Type type);
    static bool isCubic(Voxel_Type type);
    static bool isSpawnable(Voxel_Type type);
    static glm::vec4 getColor(Voxel_Type _type, double t);

private:

};

#endif /* voxel_hpp */
