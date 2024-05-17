//
//  billboard.hpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#ifndef billboard_hpp
#define billboard_hpp

#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Billboard{
public:
  GLuint fbo;       //We need an FBO to render scene to screen
  GLuint texture;
  GLuint depthTexture;
  GLuint vao;
  GLuint vbo[2];

  //Load the Texture from some surface
  bool fromRaw(SDL_Surface* TextureImage);
  bool fromImage(std::string file);

  //Render the Billboard to a specific quad
  bool setup(int width, int height);
  bool setup2(int width, int height);
  void render();
  void renderDepth();
  void cleanup();
};

#endif /* billboard_hpp */
