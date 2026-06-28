#version 330 core

out vec4 FragColor;
in vec4 gl_FragCoord;
in vec4 vertexColor;
uniform float u_time;
//uniform vec2 u_resolution;

void main() {
   float x = gl_FragCoord.x / 1000.0;
   float y = gl_FragCoord.y / 1000.0;
   if ((x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5) < 0.1 + 0.1 * sin(u_time * 5)) { FragColor = vec4(x, y, 1.0, 1.0); }
   else { FragColor = vec4(0.0, 0.0, 0.0, 1.0); }
}