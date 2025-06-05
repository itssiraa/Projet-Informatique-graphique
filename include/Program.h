#ifndef PROGRAM_H
#define PROGRAM_H

#include "ShaderUnit.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program
{
private:
  GLuint glid;
  GLuint compile_shader(const std::string& path, GLenum shader_type);
  unsigned programId = 0;
  unsigned * amount;

  std::string getLinkMessageErrorAndClear() const;
  unsigned getUniformId(const char * name) const;

  void swap(const Program & program);
  void clear();

public:
  Program();
  Program(const Program & program);
  Program & operator=(const Program & program);
  Program(const ShaderUnit & vertex, const ShaderUnit & fragment);
  Program(const std::string & vertFileName, const std::string & fragFileName);
  ~Program();

  void create();
  void link() const;
  void attachShader(const ShaderUnit & shader) const;

  void use() const { glUseProgram(programId); }
  GLuint get_id();
  GLuint getID() const { return glid; }

  void setInt(const char * name, int i) const;
  void setFloat(const char * name, float f) const;
  void setVec2(const char * name, const glm::vec2 & vec) const;
  void setVec3(const char * name, const glm::vec3 & vec) const;
  void setVec4(const char * name, const glm::vec4 & vec) const;
  void setMat2(const char * name, const glm::mat2 & mat) const;
  void setMat3(const char * name, const glm::mat3 & mat) const;
  void setMat4(const char * name, const glm::mat4 & mat) const;

  unsigned getId() const { return programId; }
};

#endif

