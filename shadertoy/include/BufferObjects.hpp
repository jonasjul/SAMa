#ifndef BUFFEROBJECTS_H
#define BUFFEROBJECTS_H
#include<glad/glad.h>

namespace jjb {
   class VBO {
      public:
         VBO(const char*);
         ~VBO();
         GLuint get();
         void bind();
         void addData(const void* data, GLsizeiptr size, GLenum usage);
         int getNameLen() const {return this->nameLength;};
         char* getName() {return this->name;};
         void setAttributePointer(GLuint index,GLint size, GLenum type,
               GLboolean normalized, GLsizei stride, const void* offset);
         void setAttributePointer(GLuint index,GLint size, GLenum type,
               GLsizei stride, const void* offset);
         void enableAttributeArray(GLuint index);
      private:
         static GLuint bound_buffer;
         GLuint vbo_id;
         char* name;
         int nameLength;
   };
   
   class EBO {
      public:
         EBO(const char*);
         ~EBO();
         GLuint get();
         void bind();
         void addData(const void* data, GLsizeiptr size, GLenum usage);
         int getNameLen() const {return this->nameLength;};
         char* getName() {return this->name;};
      private:
         static GLuint bound_buffer;
         GLuint ebo_id;
         char* name;
         int nameLength;
   };


   class VAO {
      public:
         VAO(const char*);
         ~VAO();
         GLuint get();
         void bind();
         void addVBO(VBO*);
         void addEBO(EBO*);
         void printNames();
         VBO* getVBO_byName(const char* vbo_name);
         EBO* getEBO_byName(const char* ebo_name);
      private:
         GLuint vao_id;
         const char* name;
         VBO** buffers = nullptr;
         EBO** element_buffers = nullptr;
         char** buffernames = nullptr;
         char** element_buffernames = nullptr;
         int namesMaxLength = 0;
         int elementNamesMaxLength = 0;
         int numBuffers = 0;
         int numElementBuffers =0;
   };

   

};
#endif
