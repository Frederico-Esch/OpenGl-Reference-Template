#include <callbacks.h>

void error_callback(int error, const char *description) {
  printf_s("ERROR %d", error);
  puts(": ");
  printf_s("%s\n", description);
  exit(1);
}

void key_callback(GLFWwindow *window, int key, int scan_code, int action,
                         int mods) {
  State *main_state = (State *)glfwGetWindowUserPointer(window);
  (void)scan_code;
  if (action == GLFW_RELEASE)
    return;

  switch (key) {
  case GLFW_KEY_Q:
    if (mods == 0)
      glfwSetWindowShouldClose(window, 1);
    break;
  case GLFW_KEY_ESCAPE:
    if (mods == 0)
      glfwSetWindowShouldClose(window, 1);
    break;

  case GLFW_KEY_R:
    if (mods == 0) {
      main_state->revise_circle = 1;
    } else {
      main_state->reload_shaders = 1;
    }
    break;

  case GLFW_KEY_RIGHT:
    switch (main_state->to_render) {
    case BACKGROUND:
      main_state->to_render = DIAMOND;
      break;
    case DIAMOND:
      main_state->to_render = CIRCLE;
      break;
    case CIRCLE:
      main_state->to_render = ALL;
      break;
    case ALL:
      main_state->to_render = BACKGROUND;
      break;
    }
    break;
  case GLFW_KEY_LEFT:
    switch (main_state->to_render) {
    case BACKGROUND:
      main_state->to_render = ALL;
      break;
    case DIAMOND:
      main_state->to_render = BACKGROUND;
      break;
    case CIRCLE:
      main_state->to_render = DIAMOND;
      break;
    case ALL:
      main_state->to_render = CIRCLE;
      break;
    }
    break;

  case GLFW_KEY_UP:
    if (main_state->uniforms != NULL) {
      // float time = 0;
      // glGetUniformfv(*main_state.buffers->program, main_state.uniforms->time,
      // &time); glUniform1f(main_state.uniforms->time, time+.1f);
    }
    break;
  case GLFW_KEY_DOWN:
    if (main_state->uniforms != NULL) {
      // float time = 0;
      // glGetUniformfv(*main_state.buffers->program, main_state.uniforms->time,
      // &time); glUniform1f(main_state.uniforms->time, time-.1f);
    }
    break;
  }
}

void window_resize_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  (void)width;
  (void)height;

  State *main_state = (State *)glfwGetWindowUserPointer(window);
  main_state->window->Width = width;
  main_state->window->Height = height;

  glViewport(0, 0, width, height);
}

