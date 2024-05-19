//
//  texture.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/5/24.
//

#ifndef texture_hpp
#define texture_hpp

#include <stdio.h>
#include "dependencies/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
public:
    unsigned int textureID;

    Texture() {

    }

//    ~Texture() {
//        glDeleteTextures(1, &textureID);
//    }

    bool load(const char* path) {
        int dimensions = 256; // Assuming square textures for simplicity
        int channels = 0;
        unsigned char* data = stbi_load(path, &dimensions, &dimensions, &channels, 0);
        if (data == nullptr) {
            std::cout << "Failed to load texture" << std::endl;
            return false;
        }

        // Generate and bind the texture
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters for pixelated look
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Determine the format
        GLenum format = GL_RGBA;
        if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;
        else if (channels == 1) format = GL_RED;

        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, dimensions, dimensions, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free image data
        stbi_image_free(data);

        return true;
    }
    
private:
};

#endif /* texture_hpp */
