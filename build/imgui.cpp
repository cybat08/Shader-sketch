
#include "imgui/imgui.h"
#include <cstdio>

ImGuiContext* ImGui::CreateContext() { return nullptr; }
void ImGui::DestroyContext(ImGuiContext*) {}
ImGuiIO& ImGui::GetIO() { static ImGuiIO io; return io; }
void ImGui::StyleColorsDark() {}
void ImGui::NewFrame() {}
bool ImGui::BeginMainMenuBar() { return true; }
void ImGui::EndMainMenuBar() {}
bool ImGui::BeginMenu(const char*) { return true; }
void ImGui::EndMenu() {}
bool ImGui::MenuItem(const char*, const char*, bool, bool) { return false; }
void ImGui::Separator() {}
void ImGui::SameLine() {}
bool ImGui::Button(const char*, const ImVec2&) { return false; }
bool ImGui::RadioButton(const char*, bool) { return false; }
bool ImGui::IsItemHovered() { return false; }
void ImGui::SetTooltip(const char*, ...) {}
void ImGui::SetNextWindowPos(const ImVec2&) {}
void ImGui::SetNextWindowSize(const ImVec2&) {}
bool ImGui::Begin(const char*, bool*, int) { return true; }
void ImGui::End() {}
void ImGui::OpenPopup(const char*) {}
bool ImGui::BeginPopupModal(const char*, bool*, int) { return true; }
void ImGui::CloseCurrentPopup() {}
void ImGui::EndPopup() {}
bool ImGui::InputText(const char*, char*, size_t, int, void*, void*) { return false; }
bool ImGui::SliderFloat(const char*, float*, float, float, const char*, int) { return false; }
bool ImGui::ColorEdit4(const char*, float*, int) { return false; }
bool ImGui::Selectable(const char*, bool, int, const ImVec2&) { return false; }
bool ImGui::IsMouseDoubleClicked(int) { return false; }
void ImGui::TextUnformatted(const char*, const char*) {}
void ImGui::Text(const char*, ...) {}
void ImGui::PushID(int) {}
void ImGui::PopID() {}
void ImGui::Render() {}
bool ImGui::BeginPopup(const char*) { return true; }
