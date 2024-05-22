#ifndef SHADERS_H
#define SHADERS_H
#include<glad/glad.h>
#include<string>

namespace jjs {
   
   class Shader {
      public:
         Shader(const char*, GLenum);
         ~Shader();
         bool checkStatus();
         GLuint getShader(){return this->shaderID;};
      private:
         unsigned int shaderID;
         GLint success;
         GLenum shader_type;
         std::string shader_name;
   };

   class ShaderProgram{
      public: 
         ShaderProgram(Shader, Shader);
         ~ShaderProgram();
         void use();
         bool checkStatus();
         GLuint getProgram(){return this->program;};
      private:
         Shader* vertexShader;
         Shader* fragmentShader;
         GLuint program;
         GLint success;
            
   };

};//namespace

#endif
