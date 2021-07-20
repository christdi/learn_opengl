#include "myopengl/shader_exception.h"

namespace myopengl {

// Construct a new instance of a shader_exception
//
// Parameters
// message - the message to be associated with the exception
myopengl::shader_exception::shader_exception(const std::string& message) noexcept
    : _message(message) {
}

// Returns the message associated with this exception
const char* myopengl::shader_exception::what() const noexcept {
  return _message.c_str();
}

}
