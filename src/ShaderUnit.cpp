#include "ShaderUnit.h"


ShaderUnit::ShaderUnit(const std::string & fileName, GLenum t): type(t)
{
  std::string source = getSource(fileName);
  const char* data = source.c_str();
  shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &data, nullptr);
  glCompileShader(shaderId);

  int success;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
      GLchar infoLog[512];
      glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
      std::cerr << "Erreur de compilation du shader :\n" << infoLog << std::endl;
      throw std::runtime_error(infoLog);
  }
}


std::string ShaderUnit::getSource(const std::string & fileName) const
{
  std::ifstream file(fileName, std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("Can't open shader file: " + fileName + ".");

  std::stringstream stream;
  stream << file.rdbuf();
  return stream.str();
}

std::string ShaderUnit::getCompileMessageErrorAndClear() const
{
  int length;
  glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
  char* message = new char[length];

  glGetShaderInfoLog(shaderId, length, &length, message);
  glDeleteShader(shaderId);

  std::string finalMess = message;
  delete[] message;
  return finalMess;
}

