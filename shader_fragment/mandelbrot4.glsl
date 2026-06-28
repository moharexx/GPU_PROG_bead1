#version 330 core

#define ITERATION (1000.0)
#define PI (3.1415926535897932384626433832795)

out vec4 FragColor;
in vec4 gl_FragCoord;
in vec4 vertexColor;
uniform float u_time;
uniform vec2 u_resolution;
uniform float u_x;
uniform float u_y;
uniform float u_mouse;
uniform float u_zoom;

// burning ship fractal
void main() {
   float x = (gl_FragCoord.x - u_x - u_resolution.x / 2) / 300 * u_zoom;
   float y = (gl_FragCoord.y + u_y - u_resolution.y / 2) / 300 * u_zoom;

   float c1x = x;
   float c2x = x;
   float c1y = y;
   float c2y = y;

   float i = 0;
   while (i < ITERATION) {
      i++;
      if (c2x * c2x + c2y * c2y > 100) break;

      c2x = x + c1x * c1x - c1y * c1y;
      c2y = y - abs(2 * c1x * c1y); // achieving the burning ship
      c1x = c2x;
      c1y = c2y;
   }
   if (i >= ITERATION) FragColor = vec4(1, 1, 1, 1);
   else FragColor = vec4(abs(sin(i / 10)), abs(sin(i / 10 + PI / 3)), abs(sin(i / 10 + 2 * PI / 3)), 1);
}