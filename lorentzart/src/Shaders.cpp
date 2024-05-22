#include <Shaders.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

namespace jjs {
   
   Shader::Shader(const char* filename, GLenum shader_type){
      std::cout << "debug\n";
      const char* shaderString;
      std::ifstream file;
      std::ostringstream buffer; 

      
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      this->shader_type = shader_type;

      try {
         file.open(filename);
      }
      catch (const std::system_error& e) {
         std::cout<< "Exception opening file:"<<filename<<std::endl
            <<e.code().message()<<std::endl;
      }

      buffer << file.rdbuf();
      std::string filstring = buffer.str();
      shaderString = filstring.c_str();
      
      this->shaderID = glCreateShader(shader_type);
      glShaderSource(shaderID, 1, &shaderString, NULL);
      glCompileShader(shaderID);
      glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &this->success);

         switch (shader_type) {
            case GL_VERTEX_SHADER:
               this->shader_name = std::string("VERTEX");;
               break;
            case GL_FRAGMENT_SHADER:
               this->shader_name = std::string("FRAGMENT");
               break;
            default:
               break;
         }
      
   }


   Shader::~Shader(){
      glDeleteShader(this->shaderID);
   }

   bool Shader::checkStatus(){
      GLint logSize;
      if (!this->success){
         glGetShaderiv(this->shaderID, GL_INFO_LOG_LENGTH, &logSize);
         char* log = new char[logSize];
         glGetShaderInfoLog(this->shaderID, logSize, NULL, log);
         std::cout<<"ERROR::SHADER::"<<this->shader_name<<"COMPILATION_FAILED:\n"
            << log << std::endl;
         delete[] log;
         return false;
      }
      return true;
   }


   ShaderProgram::ShaderProgram(Shader vertex, Shader fragment){
      this->vertexShader = &vertex;
      this->fragmentShader = &fragment;

      this->program = glCreateProgram();
      glAttachShader(program, vertex.getShader());
      glAttachShader(program, fragment.getShader());
      glLinkProgram(program); 
      glGetProgramiv(program, GL_LINK_STATUS, &this->success);
      

      
   }

   bool ShaderProgram::checkStatus(){
      if (!this->success){
         int loglength; 
         glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &loglength);
         char* log;
         glGetProgramInfoLog(this->program, loglength, NULL, log);
         std::cout<<"ERROR::PROGRAM::LINKING::COMPILATION_FAILED\n"
            << log <<std::endl;
         return false;
      }
      return true;
   };

   void ShaderProgram::use(){
      glUseProgram(this->program);
   }

   ShaderProgram::~ShaderProgram(){
      std::cout<<"Shaderprogramdestructor called";
   }
};
