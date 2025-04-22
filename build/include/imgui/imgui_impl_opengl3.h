
// Minimal imgui_impl_opengl3.h stub
#ifndef IMGUI_IMPL_OPENGL3_H
#define IMGUI_IMPL_OPENGL3_H

bool ImGui_ImplOpenGL3_Init(const char* glsl_version);
void ImGui_ImplOpenGL3_NewFrame();
void ImGui_ImplOpenGL3_RenderDrawData(void* draw_data);
void ImGui_ImplOpenGL3_Shutdown();

#endif // IMGUI_IMPL_OPENGL3_H
