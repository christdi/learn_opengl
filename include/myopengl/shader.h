#ifndef MYOPENGL_SHADER_H
#define MYOPENGL_SHADER_H

namespace myopengl {

class shader {

  public:
  shader(const char* vertex_shader_path, const char* fragment_shader_path);
  ~shader();

  void load(const char* vertex_shader_path, const char* fragment_shader_path);

  void setFloat(const std::string& name, float value) const noexcept;

  void use();

  private:
  unsigned int _id;

  std::string read_file_content(const char* path);
  unsigned int compile(unsigned int type, const char* src);
  unsigned int link(unsigned int vertex_shader_id, unsigned int fragment_shader_id);
};

}

#endif