#ifndef POINT_SHADOWS_RT_RENDERER_HPP
#define POINT_SHADOWS_RT_RENDERER_HPP

#include <memory>
#include <string>

#include "glitter.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <rt_renderer.hpp>

class PointShadowsRtRenderer : public RtRenderer {
public:
  PointShadowsRtRenderer();
  GLFWwindow* OpenWindow(const std::string& window_name = "RT Render") override;
  void AddModel(const std::string& file_path) override;
  void Render() override;

private:
  void processInput(float deltaTime);
  
  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Shader> depth_shader_;
  std::unique_ptr<Shader> light_box_shader_;
  std::vector<std::unique_ptr<Model>> models_;

  bool pause_ = false;
  bool pause_key_pressed_ = false;

  float lastFrameTime = 0.0f;
  unsigned int depthMapFBO;
  unsigned int depthCubemap;
  glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif
