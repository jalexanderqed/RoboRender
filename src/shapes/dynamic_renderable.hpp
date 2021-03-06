#ifndef SHAPES_DYNAMIC_RENDERABLE_HPP
#define SHAPES_DYNAMIC_RENDERABLE_HPP

#include "learnopengl/glitter.hpp"

#include "GLFW/glfw3.h"
#include "learnopengl/camera.h"
#include "shapes/renderable.hpp"

class DynamicRenderable : public Renderable {
 public:
  virtual void Tick(double delta_sec) = 0;
  virtual void Draw(ShaderSet shaders, glm::mat4 model_mat) = 0;
};

class CameraEventHandler {
 public:
  virtual void KeyboardEvents(GLFWwindow* window) = 0;
  virtual void TickUpdateCamera(Camera* camera, double delta_time) = 0;
};

#endif
