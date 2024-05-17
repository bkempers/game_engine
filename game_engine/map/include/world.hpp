//
//  world.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#ifndef world_hpp
#define world_hpp

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "../../render/include/view.hpp"
#include "terrain.hpp"
#include "chunk.hpp"
#include "octree.hpp"

class View;
class World{
public:
    //Constructor
    World(int _SEED){
        SEED = _SEED;
    }

    //Block Data in Octree
    std::vector<Chunk> chunks; //Loaded Chunks
    std::unordered_map<int, int> chunk_order;
    int SEED = 0; //1588047050
    int chunkSize = 16;
    int sealevel = 16;
    std::chrono::milliseconds tickLength = std::chrono::milliseconds(1000);
    glm::vec3 dim = glm::vec3(64, 16, 64);

    //Min and Max Chunk Positions
    glm::vec3 min = glm::vec3(0);
    glm::vec3 max = dim;

    //Movement Weights
    int moveWeight(Voxel_Type _type);
    Voxel_Type getBlock(glm::vec3 _pos);
    void setBlock(glm::vec3 _pos, Voxel_Type _type, bool fullremesh);
    glm::vec3 getTop(glm::vec2 _pos);

    //World Generation
    void generate();
    void blank();
};

#endif /* world_hpp */
