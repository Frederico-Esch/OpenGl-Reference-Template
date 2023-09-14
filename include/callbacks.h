#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdlib.h>
#include <utils.h>

void error_callback(int error, const char *description);

void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);

void window_resize_callback(GLFWwindow *window, int width, int height);
#endif
