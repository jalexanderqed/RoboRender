#ifndef SIMPLE_RT_RENDERER_HPP
#define SIMPLE_RT_RENDERER_HPP

#include <memory>
#include <string>

#include "glitter.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <rt_renderer.hpp>

class SimpleRtRenderer : public RtRenderer {
public:
  SimpleRtRenderer();
  GLFWwindow* OpenWindow(const std::string& window_name = "RT Render") override;
  void AddModel(const std::string& file_path) override;
  void Render() override;

private:
  void processInput(float deltaTime);
  
  GLFWwindow* window_;
  std::unique_ptr<Shader> shader_;
  std::vector<std::unique_ptr<Model>> models_;

  float lastFrameTime = 0.0f;
};

#endif