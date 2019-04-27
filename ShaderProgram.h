#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <unordered_map>
#include "common.h"

#include "LiteMath.h"

class ShaderProgram
{
public:

  ShaderProgram() : shaderProgram(-1) {};

  ShaderProgram(const std::unordered_map<GLenum, std::string> &inputShaders);

  virtual ~ShaderProgram() {};

  void Release(); //actual destructor

  virtual void StartUseShader() const;

  virtual void StopUseShader() const;

  GLuint GetProgram() const { return shaderProgram; }


  bool reLink();

  void SetUniform(const std::string &location, float value) const;

  void SetUniform(const std::string &location, double value) const;

  void SetUniform(const std::string &location, int value) const;

  void SetUniform(const std::string &location, unsigned int value) const;

  void SetUniform(const std::string &location, LiteMath::float4x4) const;

private:
  static GLuint LoadShaderObject(GLenum type, const std::string &filename);

  GLuint shaderProgram;
  std::unordered_map<GLenum, GLuint> shaderObjects;
};


#endif