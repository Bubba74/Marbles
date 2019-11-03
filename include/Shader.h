
#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include <string>

class Shader {
  unsigned int m_ID;

  void setMatrix (std::string type, glm::mat4 transform);
  std::string readFile (std::string path);
public:
  Shader (std::string vert_shader, std::string frag_shader);
  void use();

  void setPerspective (glm::mat4 transform);
  void setView  (glm::mat4 transform);
  void setModel (glm::mat4 transform);
  void setLocal (glm::mat4 transform);

  void setInt   (std::string key, int value);
  void setBool  (std::string key, bool value);
  void setFloat (std::string key, float value);
};

#endif
