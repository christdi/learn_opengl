#ifndef MYOPENGL_SHADER_EXCEPTION_H
#define MYOPENGL_SHADER_EXCEPTION_H

#include <exception>
#include <string>

namespace myopengl {

class shader_exception : public std::exception {

  public:
  shader_exception() = delete;
  shader_exception(const std::string& message) noexcept;

  const char* what() const noexcept override;

  private:
  std::string _message;
};

}

#endif