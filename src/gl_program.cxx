#include "gl_program.hxx"
#include "debug.hxx"
#include <iostream>

GlProgram GlProgram::loadFromFiles(std::string const &vsName,
    std::string const &fsName) {
  GlShader vs = GlShader::loadFromFile(vsName, GL_VERTEX_SHADER);
  GlShader fs = GlShader::loadFromFile(fsName, GL_FRAGMENT_SHADER);
  return GlProgram(vs, fs);
}

GlProgram::GlProgram() {
  msg("GlProgram::GlProgram()");
  id = glCreateProgram();
  chk(__FILE__, __LINE__);
}

GlProgram::GlProgram(GlShader const &vs, GlShader const &fs):
  GlProgram()
{
  msg("GlProgram::GlProgram(GlShader const &vs, GlShader const &fs)");
  attachShader(vs);
  attachShader(fs);
  link();
}

GlProgram::~GlProgram() {
  msg("GlProgram destroyed!");
  glDeleteProgram(id);
}

void GlProgram::attachShader(GlShader const &s) {
  glAttachShader(id, s.getId());
  chk(__FILE__, __LINE__);
}

GLint GlProgram::link() {
  msg("GlProgram::link()");
  glLinkProgram(id);
  chk(__FILE__, __LINE__);

  GLint succ;
  GLchar log[512];
  glGetProgramiv(id, GL_LINK_STATUS, &succ);
  if (!succ) {
    glGetProgramInfoLog(id, 512, nullptr, log);
    msg("Error linking program\n" + std::string(log));
  }

  //Look up and store all uniforms to a string keyed std::map
  int uniformCount;
  int uniformLength;
  glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);
  chk(__FILE__, __LINE__);
  msg("Uniform count: " + std::to_string(uniformCount));
  glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformLength);
  chk(__FILE__, __LINE__);
  GLchar* ufmName = new GLchar[uniformLength];
  GLint ufmSize;
  GLenum ufmType;
  for (int i=0; i<uniformCount; i++) {
    glGetActiveUniform(id, i, uniformLength, nullptr, &ufmSize, &ufmType, ufmName);
    chk(__FILE__, __LINE__);
    msg("GlProgram has uniform " + std::string(ufmName));
    uniforms[std::string(ufmName)] = glGetUniformLocation(id, ufmName);
  }
  delete[] ufmName;

  //Look up and store all attributes to a string keyed std::map
  int attribCount;
  int attribLength;
  glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attribCount);
  chk(__FILE__, __LINE__);
  glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribLength);
  chk(__FILE__, __LINE__);
  GLchar* atrName = new GLchar[attribLength];
  GLint atrSize;
  GLenum atrType;
  for (int i=0; i<attribCount; i++) {
    glGetActiveAttrib(id, i, attribLength, nullptr, &atrSize, &atrType,
        atrName);
    chk(__FILE__, __LINE__);
    attributes[std::string(atrName)] = glGetAttribLocation(id, atrName);
  }
  delete[] atrName;

  msg("GlProgram link finished with "+std::to_string(succ));
  return succ;
}

void GlProgram::use() const {
  glUseProgram(id);
  chk(__FILE__, __LINE__);
}

GLuint GlProgram::getId() {
  return id;
}

GLint GlProgram::getAttribLocation(std::string const &name) const {
  if (attributes.count(name) == 0) {
    return -1;
  }
  return attributes.at(name);
}

GLint GlProgram::getUniformLocation(std::string const &name) const {
  if (uniforms.count(name) == 0) {
    return -1;
  }
  return uniforms.at(name);
}
