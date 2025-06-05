#ifndef SHADER_UNIT_H
#define SHADER_UNIT_H 1

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

class ShaderUnit
{
  unsigned shaderId;
  GLenum type;

  std::string getSource(const std::string & fileName) const;
  std::string getCompileMessageErrorAndClear() const;

  ShaderUnit(const std::string & fileName, GLenum t);
public:
  static ShaderUnit createVertexShader(const std::string & fileName)
  {
    return ShaderUnit(fileName, GL_VERTEX_SHADER);
  }

  static ShaderUnit createFragmentShader(const std::string & fileName)
  {
    return ShaderUnit(fileName, GL_FRAGMENT_SHADER);
  }

  static ShaderUnit createGeometryShader(const std::string & fileName)
  {
    return ShaderUnit(fileName, GL_GEOMETRY_SHADER);
  }

  

  static ShaderUnit createComputeShader(const std::string & fileName)
  {
    return ShaderUnit(fileName, GL_COMPUTE_SHADER);
  }

  void clear() const { glDeleteShader(shaderId); }

  unsigned getId() const { return shaderId; }
  GLenum getType() const { return type; }
};

#endif // SHADER_UNIT_H

