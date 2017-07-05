#include <iostream>
#include "window.hxx"
#include "fps_counter.hxx"
#include <GL/gl.h>
#include <cmath>

int main(int argc, char *argv[]) {
  std::cout << "OpenGL test\n";

  Window window;
  FpsCounter fpsCounter(4, 255);
  float timeLast, time, frameTime;

  while (window.swapBuffers()) {
    // Approximate timings
    timeLast = time;
    time = window.getTime();
    frameTime = time - timeLast;

    // Run the FPS counter
    fpsCounter.addFrameTime(frameTime);
    fpsCounter.printer(time);

    glClear(GL_COLOR_BUFFER_BIT);
  }

  return 0;
}
