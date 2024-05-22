
#include "../extern/glad/include/glad/glad.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <cstdlib>
#include <iostream>

#include "../include/game_object.h"

#include <Shaders.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include<BufferObjects.hpp>

#define WINWIDTH 800
#define WINHEIGHT 600


void setupSDL(SDL_Window** window, SDL_GLContext* context);



int main(int argc, char** argv){

   SDL_Window *Window = nullptr;
   SDL_GLContext context = nullptr;
   
   std::cout<<"debug -1\n";

   setupSDL(&Window, &context);
   std::cout<<"debug 0\n";
   

   //GLenum err = glGetError();
   //if (err != GL_NO_ERROR){
   //   std::cout << "OPENGL ERROR DURING INIT";
   //}else{
   //   std::cout <<"Everything is fine...";
   //}

   int32_t Running = 1;
   int32_t FullScreen = 0;

   GameObject test(1.0f, 5.f); 
   test.printvar();

   // TEST SOME STUFF
   //
   // 
   //
   std::cout<<"debug 0\n";

   float vertices [] = {
      -1.0f,-1.0f, 
      1.0f,-1.0f, 
      1.0f,1.0f, 
      -1.0f,1.0f
   };


   uint32_t indices[] = {0,1,2,0,2,3};
   
   jjb::VAO my_vao = jjb::VAO("MainVAO");
   my_vao.bind();
   std::cout<<"debug 1\n";

   jjb::VBO my_vbo1 = jjb::VBO("vecData");
   my_vbo1.bind();
   my_vbo1.addData(vertices, sizeof(vertices), GL_STATIC_DRAW);
   my_vbo1.setAttributePointer(0,2,GL_FLOAT,GL_FALSE, 2*sizeof(float), (void*) 0);
   
   
   jjb::EBO my_ebo1 = jjb::EBO("vertexIndData");
   my_ebo1.bind();
   my_ebo1.addData(indices, sizeof(indices), GL_STATIC_DRAW);

   my_vao.addVBO(&my_vbo1);
   
   std::cout<<"debug 6\n";
   my_vao.addEBO(&my_ebo1);
      
   
   std::cout<<"debug 3\n";
   my_vao.printNames();

   std::cout<<"End my first test\n";


   jjs::Shader vshader = jjs::Shader("../src/shaders/vertexShader.glsl", GL_VERTEX_SHADER);
   bool succ = vshader.checkStatus();

   if (succ){
      std::cout << "Vertexshader created successfully\n";
   }else{
      std::cout << "Not ....";
      return 0;
   }
   
   

   jjs::Shader fshader = jjs::Shader("../src/shaders/fragmentShaders.glsl", GL_FRAGMENT_SHADER);
   succ = fshader.checkStatus();
   if (succ){
      std::cout << "Fragmentshader created successfully"<<std::endl;
   }else{
      std::cout << "Not ....";
      return 0;
   }
   
   jjs::ShaderProgram shaderprog = jjs::ShaderProgram(vshader, fshader);

   if (!shaderprog.checkStatus()){
      std::cout << "TRØBBEL!\n";
   }else{
      std::cout << "ingen TRØBBEL!\n";
   }

   shaderprog.use();

   
   int iResolution_loc  = glGetUniformLocation(shaderprog.getProgram(), "iResolution");
   int iTime_loc  = glGetUniformLocation(shaderprog.getProgram(), "iTime");
   int iMouse_loc  = glGetUniformLocation(shaderprog.getProgram(), "iMouse");

   //glEnableVertexAttribArray(0);
   glViewport(0,0,WINWIDTH, WINHEIGHT);

   //glBindVertexArray(VAO[1]);
   float timeValue_start = (float) SDL_GetTicks();
   float timeValue = (float) SDL_GetTicks();
   float timeSwitch_start = (float) SDL_GetTicks();
   unsigned int vao_num = 1;

   int winwidth = WINWIDTH;
   int winheight = WINHEIGHT;
   
   while (Running){
      
      SDL_Event event;
      while(SDL_PollEvent(&event)){
         
         
         switch (event.type) {
            case SDL_KEYDOWN:
               switch (event.key.keysym.sym){
                  case SDLK_ESCAPE:
                     Running=0;
                     break;
                  case SDLK_SPACE:
                     vao_num++;
                     vao_num = vao_num%2;
                     break;
                  default:
                     break;
               }
               break;
            case SDL_QUIT:
               Running = 0;
               break;
            case SDL_WINDOWEVENT:
               switch (event.window.event){
                  case SDL_WINDOWEVENT_RESIZED:
                     glViewport(0,0,event.window.data1, event.window.data2);
                     winwidth = event.window.data1;
                     winheight = event.window.data2;
                     break;
                  case SDL_WINDOWEVENT_SIZE_CHANGED:
                     glViewport(0,0,event.window.data1, event.window.data2);
                     winwidth = event.window.data1;
                     winheight = event.window.data2;
                     break;
                  default:
                     break;
               }
            default:
               break;
         }
               
      }//while pollevent
       //
       

      my_vao.bind();
      my_vbo1.bind();
      my_ebo1.bind();
      shaderprog.use();

      glUniform1f(iTime_loc, (float) SDL_GetTicks()/1000.f);
      glUniform2f(iResolution_loc, winwidth, winheight);
      glUniform2f(iMouse_loc, 0.f, 0.f);
      
      timeValue = ((float) SDL_GetTicks() - timeValue_start)/1000.0f;
      float greenValue = std::sin(timeValue /2.0f)+0.5f;
      //glViewport(0,0,WINWIDTH, WINHEIGHT);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //glDrawArrays(GL_TRIANGLES, 0,6);
      
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      SDL_GL_SwapWindow(Window);
      
      

      GLenum error = glGetError();
      if (error != GL_NO_ERROR) {
         std::cout<< "OPENGLERROR" << error << std::endl;
      } else {
         //std::cout << error << std::endl;
      }


   }//while running
    //

   SDL_GL_DeleteContext(context);
   SDL_DestroyWindow(Window);
   SDL_Quit();
   
   return 0;
}

void setupSDL(SDL_Window** window, SDL_GLContext* context){
   
   uint32_t WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
   uint32_t initflags = SDL_INIT_EVERYTHING;


   if (SDL_Init(initflags) != 0){
      std::cout<<"Error initializing SDL:\n"<<SDL_GetError();
   }

   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //Use core profile

   *window = SDL_CreateWindow("OpenGL TEst", 0,3, WINWIDTH, WINHEIGHT, WindowFlags);
   if (*window == NULL){
      std::cout<<"Error creating window: "<<std::endl<<SDL_GetError();
   }
   else{
      std::cout << "window not NULL"<< std::endl;
   }

   *context = SDL_GL_CreateContext(*window);

   if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
   }

   SDL_GL_MakeCurrent(*window, context);
}

