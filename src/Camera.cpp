
#include <Camera.h>

Camera::Camera () {
 this->m_name = "default";
}

std::string Camera::name () {
  return this->m_name;
}
