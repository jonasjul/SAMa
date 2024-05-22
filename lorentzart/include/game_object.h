#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject{
   public:
      GameObject(double x, double y);
      GameObject();
      
      void printvar();
      
   private:
      int myvar;
      glm::vec2 position;
};

