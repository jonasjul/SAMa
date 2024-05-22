//#include"game_object.h"
#include"../include/game_object.h"
#include<iostream>

GameObject::GameObject(){
   myvar = 1;
}
GameObject::GameObject(double x, double y){
   position = glm::vec2(x,y);
}

void GameObject::printvar(){
   std::cout<<"The position is: "<<myvar<<std::endl;
}
