#version 100

precision mediump float;

varying vec4 v_pos;

uniform float u_time;
uniform vec2 u_resolution;

uniform sampler2D u_f1;
uniform sampler2D u_f2;

void main() {
  vec2 p = v_pos.xy*0.5+0.5;

  gl_FragColor = vec4(texture2D(u_f2, p).rgb*0.5 + texture2D(u_f1, p).rgb*0.5,
    1.);
}
