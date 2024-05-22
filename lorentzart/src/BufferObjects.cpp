#include<BufferObjects.hpp>
#include<glad/glad.h>
#include<iostream>
#include<string.h>

namespace jjb{
   
   VBO::VBO(const char* name) {
      glGenBuffers(1, &(this->vbo_id));
      const char* p = name;
      int size=1;
      while (*p != '\0')
      {
         size++;
         p++;
      }
      this->nameLength = size;
      this->name = new char[size];
      for (int i=0; i<size; i++){
         this->name[i] = name[i];
      }
   }

   GLuint VBO::get(){
      return this->vbo_id;
   }

   void VBO::bind(){
      glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
      VBO::bound_buffer = this->vbo_id;
   }

   VBO::~VBO(){
      glDeleteBuffers(1, &this->vbo_id);
   }


   void VBO::addData(const void* data, GLsizeiptr size, GLenum usage){
      if (this->bound_buffer != this->vbo_id){
         std::cout<< "You need to bind this buffer before adding data to it\n";
         return;
      }
      glBufferData(GL_ARRAY_BUFFER, size, data, usage);
   }

   void VBO::setAttributePointer(GLuint index,GLint size, GLenum type,
      GLboolean normalized, GLsizei stride, const void* offset){
      if (this->bound_buffer != this->vbo_id){
         std::cout<< "You need to bind this buffer setting the attribute pointers\n";
         return;
      }
      glVertexAttribPointer(index, size, type, normalized, stride, offset);
      this->enableAttributeArray(index);
   }
   void VBO::setAttributePointer(GLuint index,GLint size, GLenum type,
      GLsizei stride, const void* offset){
      if (this->bound_buffer != this->vbo_id){
         std::cout<< "You need to bind this buffer setting the attribute pointers\n";
         return;
      }
      glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
      this->enableAttributeArray(index);
      
   }

   void VBO::enableAttributeArray(GLuint index){
      glEnableVertexAttribArray(index);
   }
   
   EBO::EBO(const char* name){
      glGenBuffers(1, &(this->ebo_id));
      const char* p = name;
      int size = 1;
      while (*p != '\0'){
         p++;
         size++;
      }
      this->name = new char[size];
      for (int i=0; i<size; i++){
         this->name[i] = name[i];
      }
      this->nameLength = size;
      
   }

   EBO::~EBO(){
      glDeleteBuffers(1, &this->ebo_id);
   }
   
   GLuint EBO::get(){
      return this->ebo_id;
   }

   void EBO::bind(){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_id);
      EBO::bound_buffer = this->ebo_id;
   }
   
   void EBO::addData(const void* data, GLsizeiptr size, GLenum usage){
      if (this->bound_buffer != this->ebo_id){
         std::cout<< "You need to bind this elementbuffer before adding data to it\n";
         return;
      }
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
   }

   VBO* VAO::getVBO_byName(const char* vbo_name){
      for (int i=0; i<this->numBuffers; i++){
         if (strcmp(vbo_name, this->buffernames[i])){
            return this->buffers[i];
         }
      }
      std::cout<<"Buffer "<<vbo_name<<" was not found"<<std::endl;
      return nullptr;
   }
   EBO* VAO::getEBO_byName(const char* ebo_name){
     for (int i=0; i<this->numElementBuffers; i++){
        if (strcmp(ebo_name, this->element_buffernames[i])){
           return this->element_buffers[i];
        }
     }
      std::cout<<"Buffer "<<ebo_name<<" was not found"<<std::endl;
      return nullptr;
   }

   VAO::VAO(const char* name) {
      this->name = name;
      glGenVertexArrays(1, &(this->vao_id));
   }
   VAO::~VAO(){
      glDeleteBuffers(1, &this->vao_id);
   }

   GLuint VAO::get(){
      return this->vao_id;
   }

   void VAO::bind(){
      glBindVertexArray(this->vao_id);
   }

   void VAO::addVBO(VBO* buffer_object){
      int b_size = buffer_object->getNameLen();
      VBO** buffers_tmp = this->buffers;
      VBO** buffers_new = new VBO*[this->numBuffers+1];
      for (int i=0; i<this->numBuffers; i++){
         buffers_new[i] = buffers_tmp[i];
      }
      buffers_new[this->numBuffers] = buffer_object;
      this->numBuffers += 1;
      delete this->buffers;
      this->buffers = buffers_new;
      
      if (b_size > this->namesMaxLength){
         this->namesMaxLength = b_size;
      }

      char** new_buffernames = new char*[this->numBuffers];
      for (int i=0; i<this->numBuffers; i++){
         new_buffernames[i] = this->buffers[i]->getName();
      }
      delete this->buffernames;
      this->buffernames = new_buffernames;
   }

   void VAO::printNames(){
      for (int i=0; i<this->numBuffers; i++){
         std::cout<<this->buffernames[i]<<std::endl;
      }
   }

   
   void VAO::addEBO(EBO* element_buffer){
      std::cout<<"EBO-debug 0\n";
      int b_size = element_buffer->getNameLen();
      std::cout<<"EBO-debug 1\n";
      EBO** buffers_tmp = this->element_buffers;
      std::cout<<"EBO-debug 2\n";
      EBO** buffers_new = new EBO*[this->numElementBuffers+1];
      std::cout<<"EBO-debug 3\n";
      for (int i=0; i<this->numElementBuffers; i++){
         buffers_new[i] = buffers_tmp[i];
      }
      buffers_new[this->numElementBuffers] = element_buffer;
      std::cout<<"EBO-debug 5\n";
      this->numElementBuffers += 1;
      std::cout<<"EBO-debug 6\n";
      delete this->element_buffers;
      std::cout<<"EBO-debug 7\n";
      this->element_buffers = buffers_new;
      
      if (b_size > this->elementNamesMaxLength){
         this->elementNamesMaxLength = b_size;
      }

      std::cout<<"EBO-debug 8\n";
      char** new_buffernames = new char*[this->numElementBuffers];
      for (int i=0; i<this->numElementBuffers; i++){
      std::cout<<"EBO-debug 8."<<i<<std::endl;
         new_buffernames[i] = this->element_buffers[i]->getName();
      }
      std::cout<<"EBO-debug 9\n";
      delete this->element_buffernames;
      this->element_buffernames = new_buffernames;
   }
   
   GLuint VBO::bound_buffer = -1;
   GLuint EBO::bound_buffer = -1;
};

