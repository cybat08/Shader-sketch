
// Minimal imgui_impl_glfw.h stub
#ifndef IMGUI_IMPL_GLFW_H
#define IMGUI_IMPL_GLFW_H

struct GLFWwindow;
bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
void ImGui_ImplGlfw_NewFrame();
void ImGui_ImplGlfw_Shutdown();

#endif // IMGUI_IMPL_GLFW_H
