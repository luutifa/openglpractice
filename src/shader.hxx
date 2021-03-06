#pragma once

#include "window.hxx"
#include "gl_vertex_attrib.hxx"
#include "gl_program.hxx"
#include "demo.hxx"
#include "drawable.hxx"
#include <vector>
#include <map>

class Shader: public Drawable {
  public:
    Shader(Demo const &demo, GlShader const &fs);
    Shader(Demo const &demo, std::string const &fsSource);
    void draw() const;

    void setUniform(std::string const &name, int n, float const v[]) const;
    void setUniform(std::string const &name, int n, int const v[]) const;
    void setUniform(GLuint id, int n, float const v[]) const;
    void setUniform(GLuint id, int n, int const v[]) const;

    void addRocketTrack(std::string const &name);

    static Shader loadFromFile(Demo const &demo, std::string filename);

  private:
    Demo const &demo;
    GlProgram program;
    GlVertexAttrib vaPos;

    GLuint const idTime;
    GLuint const idFftBass;
    GLuint const idFftTreble;

    class RocketTrackUniform {
      public:
        RocketTrackUniform(sync_track const *track, GLint const id):
          id(id), nTracks(1)
        {tracks[0]=track;}
        sync_track const *tracks[4];
        int nTracks;
        GLint const id;
    };

    std::map<std::string, RocketTrackUniform> tracks;
};
