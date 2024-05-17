//
//  main.h
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

//OpenGL Dependencies
#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//File / General IO
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

//Standard Includes
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "shader/shader.hpp"
#include "texture/stb_image.h"

#include "gui/imgui/imgui.h"
#include "ecs/flecs.h"
#include "ecs/entity/entity.hpp"
#include "ecs/component/component.hpp"
