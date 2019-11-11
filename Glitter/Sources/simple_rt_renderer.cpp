#include "simple_rt_renderer.hpp"

namespace {

  constexpr unsigned int SCR_WIDTH = 800;
  constexpr unsigned int SCR_HEIGHT = 600;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

  Camera camera_(glm::vec3(0.0f, 0.0f, 3.0f));

  void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }

  void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera_.ProcessMouseMovement(xoffset, yoffset);
  }

  void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera_.ProcessMouseScroll(yoffset);
  }

}

SimpleRtRenderer::SimpleRtRenderer() {}

GLFWwindow* SimpleRtRenderer::OpenWindow(const std::string& window_name) {
  window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::exit(-1);
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, &framebuffer_size_callback);
  glfwSetCursorPosCallback(window_, &mouse_callback);
  glfwSetScrollCallback(window_, &scroll_callback);
    
  // tell GLFW to capture our mouse
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  // -------------------------
  shader_.reset(new Shader("1.model_loading.vs", "1.model_loading.fs"));
  return window_;
}

void SimpleRtRenderer::AddModel(const std::string& file_path) {
  models_.push_back(std::unique_ptr<Model>(new Model(FileSystem::getPath(file_path))));
}

void SimpleRtRenderer::Render() {
  glfwMakeContextCurrent(window_);

  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - lastFrameTime;
  lastFrameTime = currentFrame;

  processInput(deltaTime);

  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_->use();

  // view/projection transformations
  glm::mat4 projection = glm::perspective(glm::radians(camera_.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = camera_.GetViewMatrix();
  shader_->setMat4("projection", projection);
  shader_->setMat4("view", view);

  // render the loaded model
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
  // model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f)); // scale sponza model
  model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // scale nanosuit model
  shader_->setMat4("model", model);
  for(auto& model : models_) {
    model->Draw(shader_.get());
  }

  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void SimpleRtRenderer::processInput(float deltaTime) {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);

  if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS)
    camera_.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS)
    camera_.ProcessKeyboard(DOWN, deltaTime);
}