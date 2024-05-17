//
//  view.cpp
//  game_engine
//
//  Created by Ben Kempers on 5/12/24.
//

#include "view.hpp"
#include "../../map/include/world.hpp"

/*
================================================================================
                              Setup / Cleanup
================================================================================
*/

bool View::Init(){
  //Configure Global OpenGL State
  glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
  glLineWidth(2.5f);

  setupShaders();

  //This shoudl now work.
  image.setup(SCREEN_WIDTH, SCREEN_HEIGHT);
  reflection.setup(SCREEN_WIDTH, SCREEN_HEIGHT);
  shadow.setup2(SHADOW_WIDTH, SHADOW_HEIGHT);
  temp1.setup(SCREEN_WIDTH, SCREEN_HEIGHT);
  temp2.setup(SCREEN_WIDTH, SCREEN_HEIGHT);

  picker.setup();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void View::setupShaders(){
  //Setup Cube Shader
  cubeShader.setup("default.vs", "default.fs");
  cubeShader.addAttribute(0, "in_Position");
  cubeShader.addAttribute(1, "in_Color");
  cubeShader.addAttribute(2, "in_Normal");

  //Reflection Shader
  reflectShader.setup("reflect.vs", "reflect.fs");
  reflectShader.addAttribute(0, "in_Position");
  reflectShader.addAttribute(1, "in_Color");
  reflectShader.addAttribute(2, "in_Normal");

  //Setup Depthshader
  depthShader.setup("depth.vs", "depth.fs");
  depthShader.addAttribute(0, "in_Position");

  //Setup Billboardshader
  blurShader.setup("dof.vs", "dof.fs");
  blurShader.addAttribute(0, "in_Quad");
  blurShader.addAttribute(1, "in_Tex");

  billboardShader.setup("billboard.vs", "billboard.fs");
  billboardShader.addAttribute(0, "in_Quad");
  billboardShader.addAttribute(1, "in_Tex");
}

void View::cleanup(){
  //Cleanup Models
  for(unsigned int i = 0; i < models.size(); i++){
    //Cleanup the Models
    models[i].cleanup();
  }

  //Cleanup Shaders
  cubeShader.cleanup();
  reflectShader.cleanup();
  depthShader.cleanup();
  billboardShader.cleanup();
  blurShader.cleanup();

  image.cleanup();
  reflection.cleanup();
  shadow.cleanup();
  temp1.cleanup();
  temp2.cleanup();
}

/*
================================================================================
                              Model Generation
================================================================================
*/

void View::loadChunkModels(World &world){
  //Loop over all chunks
  for(unsigned int i = 0; i < world.chunks.size(); i++){

    //If we are at capacity, add a new item (i.e. only the newly added chunks are fully remodeled)
    if(i == models.size()){
      Model model;
      model.cpos = world.chunks[i].position;
      model.fromChunkGreedy(world.chunks[i]);
      model.setup();
      models.push_back(model);
    }

    //Old chunks need to be translated too. Translate according to player position.
    glm::vec3 axis = (glm::vec3)(world.chunks[i].position)*(float)world.chunkSize-viewPos;

    //Translate the guy
    models[i].reset();
    models[i].translate(axis);
  }
  //Make sure updateLOD is false
  updateLOD = false;
}

void View::updateChunkModels(World &world){
  //Loop over all chunks, see if they are updated.
  for(unsigned int i = 0; i < world.chunks.size(); i++){
    if(world.chunks[i].remesh){
      models[i].fromChunkGreedy(world.chunks[i]);
      models[i].update();
      world.chunks[i].remesh = false;
    }
  }
}

/*
================================================================================
                                Rendering
================================================================================
*/

void View::render(World &world){

  /* Set Current Parameters based on the time! */

  float t = (double)540/(60.0*24.0);
  skyCol = color.bezier(ease.cubic(t), color.skycolors);

  //Move the Light Across the Sky
  lightPos = glm::vec3(-10.0f, ease.quadratic(t)*20.0f+10.0f, -10.0f+t*20.0f);
  glm::mat4 depthCamera = glm::lookAt(lightPos, glm::vec3(0,0,0), glm::vec3(0,1,0));

  //Set the Fog-Color
  fogColor = glm::vec4(ease.quartic(t), ease.quartic(t), ease.quartic(t), 1.0);
  lightStrength = 1.4*ease.quartic(t)+0.1;

  /* SHADOW MAPPING */
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, shadow.fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Use the Shader
  depthShader.useProgram();

  //Loop over the Models to Render to Shadowmap
  for(unsigned int i = 0; i < models.size(); i++){
    //Set the Projection Stuff
    depthShader.setMat4("dmvp", depthProjection * depthCamera * models[i].model);
    //Render the Model
    models[i].render();
  }

  glBindVertexArray(0);

  /* REFLECTION */
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  /*
  glBindFramebuffer(GL_FRAMEBUFFER, reflection.fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  reflectShader.useProgram();    //Use the model's shader
  reflectShader.setVec3("lightCol", lightCol);
  reflectShader.setVec3("lightPos", lightPos);
  reflectShader.setFloat("lightStrength", lightStrength);

  //Reflect the Camera around a specific position (and make sure it is flipped  VVVVVV  upside down)
  glm::vec3 reflectcamerapos = (cameraPos)*glm::vec3(1, -1, 1) + glm::vec3(0, 2*world.sealevel, 0);
  glm::vec3 reflectlookpos = lookPos*glm::vec3(1, -1, 1) + glm::vec3(0, 2*world.sealevel, 0);
  glm::mat4 reflectcamera = glm::rotate(glm::lookAt(reflectcamerapos, reflectlookpos, glm::vec3(0,-1,0)), glm::radians(rotation), glm::vec3(0,1,0));

  //Set the other matrices
  reflectShader.setInt("clip", world.sealevel+viewPos.y);
  reflectShader.setMat4("projectionCamera", projection*reflectcamera);

  //Loop over the Stuff
  for(unsigned int i = 0; i < models.size(); i++){
    //View Projection Matrix
    reflectShader.setMat4("model", glm::translate(models[i].model, glm::vec3(0, 2*viewPos.y, 0)));
    //Render the Model
    models[i].render();               //Render Scene
  }
  */

  /* REGULAR IMAGE */
  glBindFramebuffer(GL_FRAMEBUFFER, image.fbo);
  glClearColor(skyCol.x, skyCol.y, skyCol.z, 1.0f); //Blue
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Use the Shader
  cubeShader.useProgram();    //Use the model's shader
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, shadow.depthTexture);
  glActiveTexture(GL_TEXTURE0+1);
  glBindTexture(GL_TEXTURE_2D, reflection.texture);
  cubeShader.setInt("reflection", 1);
  cubeShader.setInt("clip", world.sealevel-viewPos.y);

  //Appearance Stuff
  cubeShader.setInt("shadowMap", 0);
  cubeShader.setVec3("lightCol", lightCol);
  cubeShader.setVec3("lightPos", lightPos);
  cubeShader.setVec3("lookDir", lookPos-cameraPos);
  cubeShader.setFloat("lightStrength", lightStrength);
  cubeShader.setBool("_grain", grain);

  //Stuff for Adding Transparency Windows
  cubeShader.setBool("transparent", transparent && picked2);  //We need two selections and transparency activated.
  cubeShader.setVec3("volPosA", select-viewPos);
  cubeShader.setVec3("volPosB", select2-viewPos);

  //Set the other matrices
  cubeShader.setMat4("projectionCamera", projection * camera);
  cubeShader.setMat4("dbmvp", biasMatrix * depthProjection * depthCamera * glm::mat4(1.0f));

  //Loop over the Stuff
  for(unsigned int i = 0; i < models.size(); i++){
    //View Projection Matrix
    cubeShader.setMat4("model", models[i].model);
    //Render the Model
    models[i].render();               //Render Scene
  }

  /* Picker Cube! */
  if(picked){
    picker.shaderColorPick.useProgram();
    if(picked2){
      //Get the Volume Selection Better
      glm::vec3 a = select2-viewPos-(select2-select)/glm::vec3(2.0);
      glm::mat4 trans = glm::translate(glm::mat4(1.0), a);
      picker.model = glm::scale(trans, glm::abs(select2-select)+glm::vec3(1.0));
      picker.shaderColorPick.setVec3("un_Color", hoverColorBlock);
      picker.shaderColorPick.setMat4("mvp", projection*camera*picker.model);
    }
    else{
      picker.model = glm::translate(glm::mat4(1.0f), select-viewPos);
      picker.shaderColorPick.setVec3("un_Color", clickColorBlock);
      picker.shaderColorPick.setMat4("mvp", projection*camera*picker.model);
    }
    glBindVertexArray(picker.vao);
    glDrawArrays(GL_LINE_STRIP, 0, 16);
  }

  //Render Temp to Screen with a horizontal blur shader
  glBindFramebuffer(GL_FRAMEBUFFER, temp1.fbo);
  blurShader.useProgram();
  blurShader.setFloat("mousex", focus.x);
  blurShader.setFloat("mousey", focus.y);
  blurShader.setFloat("width", SCREEN_WIDTH);
  blurShader.setFloat("height", SCREEN_HEIGHT);
  blurShader.setBool("vert", false);
  blurShader.setBool("_fog", fog);
  blurShader.setInt("_blur", blur);
  blurShader.setVec3("fogColor", fogColor);
  glActiveTexture(GL_TEXTURE0+0);
  glBindTexture(GL_TEXTURE_2D, image.texture);
  blurShader.setInt("imageTexture", 0);
  glActiveTexture(GL_TEXTURE0+1);
  glBindTexture(GL_TEXTURE_2D, image.depthTexture);
  blurShader.setInt("depthTexture", 1);
  glBindVertexArray(image.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //Render screen to monitor with vertical blur shader
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0+0);
  glBindTexture(GL_TEXTURE_2D, temp1.texture);
  blurShader.setInt("imageTexture", 0);
  glActiveTexture(GL_TEXTURE0+1);
  glBindTexture(GL_TEXTURE_2D, image.depthTexture);
  blurShader.setInt("depthTexture", 1);
  blurShader.setBool("vert", true);
  glBindVertexArray(temp1.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

glm::vec3 View::intersect(World world, glm::vec2 mouse){
  //Rotation Matrix
  glm::mat4 _rotate = glm::rotate(glm::mat4(1.0), glm::radians(-rotation), glm::vec3(0, 1, 0));
  glm::vec3 _cameraposabs = _rotate*glm::vec4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0);
  glm::vec3 _camerapos = viewPos + _cameraposabs;

  //Get our position offset
  float scalex = 2.0f*(mouse.x/SCREEN_WIDTH)-1.0f;
  float scaley = 2.0f*(mouse.y/SCREEN_HEIGHT)-1.0f;

  glm::vec3 _dir =  glm::normalize(lookPos-_cameraposabs);
  glm::vec3 _xdir = glm::normalize(glm::vec3(-_dir.z , 0, _dir.x));
  glm::vec3 _ydir = glm::normalize(glm::cross(_dir, _xdir));
  glm::vec3 _startpos = _camerapos + _xdir*scalex*(SCREEN_WIDTH*zoom) + _ydir*scaley*(SCREEN_HEIGHT*zoom);

  glm::vec3 a = glm::round(_startpos - _dir * glm::vec3(100));
  glm::vec3 b = glm::round(_startpos + _dir * glm::vec3(100));

  //Get the direction
  glm::vec3 dir = glm::abs(b - a);
  glm::vec3 s = glm::vec3(0);

  s.x = (a.x > b.x)?-1:1;
  s.y = (a.y > b.y)?-1:1;
  s.z = (a.z > b.z)?-1:1;

  Voxel_Type _block;

  //Find the driving axis and do the guy
  if(dir.x >= dir.y && dir.x >= dir.z){
    //Get these values
    float p1 = 2*dir.y-dir.x;
    float p2 = 2*dir.z-dir.x;

    //While we aren't at the goal position
    while (a.x != b.x){
      a.x += s.x;

      if (p1 >= 0){
        a.y += s.y;
        p1 -= 2 * dir.x;
      }
      if (p2 >= 0){
        a.z += s.z;
        p2 -= 2 * dir.x;
      }
      p1 += 2 * dir.y;
      p2 += 2 * dir.z;

      _block = world.getBlock(a);
      if(Voxel::isVisible(_block)) return a;
    }
  }
  else if(dir.y >= dir.x && dir.y >= dir.z){
    //Get these values
    float p1 = 2*dir.x-dir.y;
    float p2 = 2*dir.z-dir.y;

    //While we aren't at the goal position
    while (a.y != b.y){
      a.y += s.y;

      if (p1 >= 0){
        a.x += s.x;
        p1 -= 2 * dir.y;
      }
      if (p2 >= 0){
        a.z += s.z;
        p2 -= 2 * dir.y;
      }
      p1 += 2 * dir.x;
      p2 += 2 * dir.z;

      _block = world.getBlock(a);
      if(Voxel::isVisible(_block)) return a;
    }
  }
  else{
    //Get these values
    float p1 = 2*dir.x-dir.z;
    float p2 = 2*dir.y-dir.z;

    //While we aren't at the goal position
    while (a.z != b.z){
      a.z += s.z;

      if (p1 >= 0){
        a.x += s.x;
        p1 -= 2 * dir.z;
      }
      if (p2 >= 0){
        a.y += s.y;
        p2 -= 2 * dir.z;
      }
      p1 += 2 * dir.x;
      p2 += 2 * dir.y;

      _block = world.getBlock(a);
      if(Voxel::isVisible(_block)) return a;
    }
  }
  return glm::vec3(0);
}

/*
================================================================================
                                    Helpers
================================================================================
*/

template<typename D>
void View::calcFrameTime(){
  //Current Time!
  auto _new = std::chrono::high_resolution_clock::now();
  frameTime = std::chrono::duration_cast<D>(_new - _old).count();
  _old = std::chrono::high_resolution_clock::now();
}
