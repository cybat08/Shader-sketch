
// Minimal GLFW3 stub
#ifndef GLFW3_H
#define GLFW3_H

#include <GL/gl.h>

#define GLFW_KEY_ESCAPE                256
#define GLFW_KEY_W                     87
#define GLFW_KEY_A                     65
#define GLFW_KEY_S                     83
#define GLFW_KEY_D                     68
#define GLFW_KEY_Q                     81
#define GLFW_KEY_E                     69
#define GLFW_KEY_1                     49
#define GLFW_KEY_2                     50
#define GLFW_KEY_3                     51
#define GLFW_KEY_N                     78
#define GLFW_KEY_O                     79
#define GLFW_MOUSE_BUTTON_LEFT         0
#define GLFW_MOUSE_BUTTON_RIGHT        1
#define GLFW_PRESS                     1
#define GLFW_RELEASE                   0
#define GLFW_MOD_CONTROL               0x0002
#define GLFW_TRUE                      1
#define GLFW_FALSE                     0

#define GLFW_CONTEXT_VERSION_MAJOR     0x00022002
#define GLFW_CONTEXT_VERSION_MINOR     0x00022003
#define GLFW_OPENGL_PROFILE            0x00022008
#define GLFW_OPENGL_CORE_PROFILE       0x00032001

struct GLFWwindow;

int glfwInit();
void glfwTerminate();
void glfwWindowHint(int hint, int value);
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWwindow* monitor, GLFWwindow* share);
void glfwDestroyWindow(GLFWwindow* window);
void glfwMakeContextCurrent(GLFWwindow* window);
void glfwSwapInterval(int interval);
void glfwSwapBuffers(GLFWwindow* window);
void glfwPollEvents();
int glfwGetKey(GLFWwindow* window, int key);
int glfwGetMouseButton(GLFWwindow* window, int button);
void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);
void glfwSetWindowShouldClose(GLFWwindow* window, int value);
int glfwWindowShouldClose(GLFWwindow* window);
void glfwSetFramebufferSizeCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int));
void glfwSetMouseButtonCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int, int));
void glfwSetCursorPosCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, double, double));
void glfwSetScrollCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, double, double));
void glfwSetKeyCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int, int, int));
GLFWwindow* glfwGetCurrentContext();
double glfwGetTime();

// implementation
int glfwInit() { return 1; }
void glfwTerminate() {}
void glfwWindowHint(int hint, int value) {}
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWwindow* monitor, GLFWwindow* share) { return (GLFWwindow*)1; }
void glfwDestroyWindow(GLFWwindow* window) {}
void glfwMakeContextCurrent(GLFWwindow* window) {}
void glfwSwapInterval(int interval) {}
void glfwSwapBuffers(GLFWwindow* window) {}
void glfwPollEvents() {}
int glfwGetKey(GLFWwindow* window, int key) { return GLFW_RELEASE; }
int glfwGetMouseButton(GLFWwindow* window, int button) { return GLFW_RELEASE; }
void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos) { *xpos = 0; *ypos = 0; }
void glfwSetWindowShouldClose(GLFWwindow* window, int value) {}
int glfwWindowShouldClose(GLFWwindow* window) { return 0; }
void glfwSetFramebufferSizeCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int)) {}
void glfwSetMouseButtonCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int, int)) {}
void glfwSetCursorPosCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, double, double)) {}
void glfwSetScrollCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, double, double)) {}
void glfwSetKeyCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int, int, int)) {}
GLFWwindow* glfwGetCurrentContext() { return (GLFWwindow*)1; }
double glfwGetTime() { return 0.0; }

#endif // GLFW3_H
