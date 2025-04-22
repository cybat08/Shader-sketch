
// Minimal ImGui stub
#ifndef IMGUI_H
#define IMGUI_H

#include <string>

#define IMGUI_CHECKVERSION() true

struct ImGuiContext;
struct ImGuiIO;
struct ImVec2 { float x, y; ImVec2(float _x=0, float _y=0) { x=_x; y=_y; } };
struct ImVec4 { float x, y, z, w; ImVec4(float _x=0, float _y=0, float _z=0, float _w=0) { x=_x; y=_y; z=_z; w=_w; } };

namespace ImGui {
    ImGuiContext* CreateContext();
    void DestroyContext(ImGuiContext* ctx = nullptr);
    ImGuiIO& GetIO();
    void StyleColorsDark();
    void NewFrame();
    bool BeginMainMenuBar();
    void EndMainMenuBar();
    bool BeginMenu(const char* label);
    void EndMenu();
    bool MenuItem(const char* label, const char* shortcut = nullptr, bool selected = false, bool enabled = true);
    void Separator();
    void SameLine();
    bool Button(const char* label, const ImVec2& size = ImVec2(0,0));
    bool RadioButton(const char* label, bool active);
    bool IsItemHovered();
    void SetTooltip(const char* fmt, ...);
    void SetNextWindowPos(const ImVec2& pos);
    void SetNextWindowSize(const ImVec2& size);
    bool Begin(const char* name, bool* p_open = nullptr, int flags = 0);
    void End();
    void OpenPopup(const char* str_id);
    bool BeginPopupModal(const char* name, bool* p_open = nullptr, int flags = 0);
    void CloseCurrentPopup();
    void EndPopup();
    bool InputText(const char* label, char* buf, size_t buf_size, int flags = 0, void* callback = nullptr, void* user_data = nullptr);
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", int flags = 0);
    bool ColorEdit4(const char* label, float* col, int flags = 0);
    bool Selectable(const char* label, bool selected = false, int flags = 0, const ImVec2& size = ImVec2(0,0));
    bool IsMouseDoubleClicked(int button);
    void TextUnformatted(const char* text, const char* text_end = nullptr);
    void Text(const char* fmt, ...);
    void PushID(int int_id);
    void PopID();
    void Render();
    bool BeginPopup(const char* str_id);
}

// ImGui IO
struct ImGuiIO {
    float        DisplaySize[2];
    bool         WantCaptureMouse;
    bool         WantCaptureKeyboard;
    int          ConfigFlags;
};

// ImGui Flags
enum ImGuiConfigFlags_ {
    ImGuiConfigFlags_NavEnableKeyboard = 1 << 0,
    ImGuiConfigFlags_DockingEnable     = 1 << 6
};

enum ImGuiWindowFlags_ {
    ImGuiWindowFlags_None                   = 0,
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,
    ImGuiWindowFlags_NoResize               = 1 << 1,
    ImGuiWindowFlags_NoMove                 = 1 << 2,
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 5
};

enum ImGuiInputTextFlags_ {
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 2
};

#endif // IMGUI_H
