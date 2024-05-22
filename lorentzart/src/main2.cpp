
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


struct pos{
   float x;
   float y; 
   float z;
};

pos getNextPos(float x, float y, float z, float dt);
void setupSDL(SDL_Window** window, SDL_GLContext* context);
void updatePositions(float** positions);

float rho = 28;
float sigma = 10;
float beta = 8.0/3.0;
int positions_stepcount = 0;

int main(int argc, char** argv){

   SDL_Window *Window = nullptr;
   SDL_GLContext context = nullptr;
   

   setupSDL(&Window, &context);
   

   //GLenum err = glGetError();
   //if (err != GL_NO_ERROR){
   //   std::cout << "OPENGL ERROR DURING INIT";
   //}else{
   //   std::cout <<"Everything is fine...";
   //}

   int32_t Running = 1;
   int32_t FullScreen = 0;


   float vertices [] = {
      -1.0f,-1.0f, 
      1.0f,-1.0f, 
      1.0f,1.0f, 
      -1.0f,1.0f
   };

   int life = 1000;
   float** positions = new float*[life];
   uint32_t* pos_indices = new uint32_t[(life-1)*2];
   
   for (int i=0; i<life; i++){
      positions[i] = new float[3];
      positions[i][0] =0.0f;
      positions[i][1] =0.0f;
      positions[i][2] =0.0f;
   }
   positions[0][0] = 0.5f;
   positions[0][1] = 0.5f;
   positions[0][2] = 0.5f;

   uint32_t ind_counter = 0;
   for (int i=0; i<(life-1)*2; i += 2){
      pos_indices[i] = ind_counter++;
      pos_indices[i+1] = ind_counter;
   }

   updatePositions(positions);
   jjb::VAO my_vao = jjb::VAO("MainVAO");
   my_vao.bind();

   jjb::VBO my_vbo1 = jjb::VBO("positionData");
   my_vbo1.bind();
   my_vbo1.addData(positions,sizeof(float)*3*(positions_stepcount+1), GL_STATIC_DRAW);
   my_vbo1.setAttributePointer(0,3,GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*) 0);
   
   
   jjb::EBO my_ebo1 = jjb::EBO("vertexIndData");
   my_ebo1.bind();
   my_ebo1.addData(pos_indices, 2*sizeof(uint32_t)*positions_stepcount, GL_STATIC_DRAW);

   my_vao.addVBO(&my_vbo1);
   
   my_vao.addEBO(&my_ebo1);
      
   
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
   int dt_loc = glGetUniformLocation(shaderprog.getProgram(), "dt");

   //glEnableVertexAttribArray(0);
   glViewport(0,0,WINWIDTH, WINHEIGHT);

   //glBindVertexArray(VAO[1]);
   float timeValue_start = (float) SDL_GetTicks();
   float timeValue = (float) SDL_GetTicks();
   float timeSwitch_start = (float) SDL_GetTicks();
   unsigned int vao_num = 1;

   int winwidth = WINWIDTH;
   int winheight = WINHEIGHT;
   float dt;
   
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
       

      //my_vao.bind();
      //my_vbo1.bind();
      //my_ebo1.bind();
      //shaderprog.use();

      dt = (SDL_GetTicks()-timeValue_start)/1000.f;

      //glUniform1f(iTime_loc, (float) SDL_GetTicks()/1000.f);
      //glUniform1f(dt_loc, dt);
      //glUniform2f(iResolution_loc, winwidth, winheight);
      //glUniform2f(iMouse_loc, 0.f, 0.f);
      
      
      float greenValue = std::sin(timeValue /2.0f)+0.5f;
      //glViewport(0,0,WINWIDTH, WINHEIGHT);
      glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
      glClear(GL_COLOR_BUFFER_BIT);
      //glDrawArrays(GL_TRIANGLES, 0,6);
      
      //glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);
      glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
      SDL_GL_SwapWindow(Window);
      
      

      GLenum error = glGetError();
      if (error != GL_NO_ERROR) {
         std::cout<< "OPENGLERROR" << error << std::endl;
      } else {
         //std::cout << error << std::endl;
      }

      //updatePositions(positions);
      //glBindBuffer(GL_ARRAY_BUFFER, 0);
      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      //my_vbo1.bind();
      //my_vbo1.addData(positions, sizeof(float)*3*(positions_stepcount+1), GL_STATIC_DRAW);
      //my_vbo1.setAttributePointer(0,3, GL_FLOAT, 3*sizeof(float), (void*) 0);
      //my_ebo1.bind();
      //my_ebo1.addData(pos_indices, sizeof(uint32_t)*(positions_stepcount)*2, GL_STATIC_DRAW);

      std::cout <<"Stepcount" << positions_stepcount << std::endl;
      for (int i=0; i<positions_stepcount+1; i++){
         std::cout <<"Positions: " << positions[i][0] << ", " << positions[i][1] << ", " << positions[i][2] << ", " << std::endl;
      }
      for (int i=0; i<positions_stepcount*2; i++){
         std::cout<<"Indices: "<<pos_indices[i] << std::endl;
      }
      //std::cout <<"Positions: " << positions[1][0] << ", " << positions[1][1] << ", " << positions[1][2] << ", " << std::endl;
      //std::cout<< "Iteration: " << positions_stepcount << std::endl;


   }//while running
    //

   SDL_GL_DeleteContext(context);
   SDL_DestroyWindow(Window);
   SDL_Quit();

   for (int i=0; i<life; i++){
      delete[] positions[i];
   }

   delete[] pos_indices;
   
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

pos getNextPos(float x, float y, float z, float dt){
   float vx = sigma*(y-x);
   float vy = x*(rho-z)-y;
   float vz = x*y-beta*z;

   pos nextpos;
   nextpos.x = (x+vx*dt/30);
   nextpos.y = (y+vy*dt/30);
   nextpos.z = (z+vz*dt/30);
   return nextpos;
}

void updatePositions(float** positions){
   pos next = getNextPos(positions[positions_stepcount][0],
         positions[positions_stepcount][1],
         positions[positions_stepcount][2], 1.1f);
   
   positions_stepcount++;
   positions[positions_stepcount][0] = next.x;
   positions[positions_stepcount][1]= next.y;
   positions[positions_stepcount][2]= next.z;
}
