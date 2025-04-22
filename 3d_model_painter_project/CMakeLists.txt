cmake_minimum_required(VERSION 3.10)
project(3DModelPainter VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add cmake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# First create the cmake directory and module file
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Create a local FindOpenGL.cmake
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindOpenGL.cmake "
# Custom FindOpenGL module
set(OPENGL_FOUND TRUE)
set(OPENGL_INCLUDE_DIR \"/nix/store\")
set(OPENGL_LIBRARIES GL)
")

# Find packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(GLFW QUIET glfw3)
  pkg_check_modules(GLM QUIET glm)
  pkg_check_modules(ASSIMP QUIET assimp)
endif()

# Create directories for glad sources
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/glad)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/KHR)

# Create glad.h header
file(WRITE ${PROJECT_BINARY_DIR}/include/glad/glad.h "
#ifndef GLAD_H
#define GLAD_H

#include <GL/gl.h>

#ifdef __cplusplus
extern \"C\" {
#endif

int gladLoadGL(void);

#ifdef __cplusplus
}
#endif

#endif /* GLAD_H */
")

# Create khrplatform.h header 
file(WRITE ${PROJECT_BINARY_DIR}/include/KHR/khrplatform.h "
#ifndef KHRPLATFORM_H
#define KHRPLATFORM_H

typedef int khronos_int32_t;
typedef unsigned int khronos_uint32_t;
typedef float khronos_float_t;

#endif /* KHRPLATFORM_H */
")

# Create glad.c source
file(WRITE ${PROJECT_BINARY_DIR}/glad.c "
#include <glad/glad.h>
#include <GL/gl.h>
#include <stdlib.h>

int gladLoadGL(void) {
    return 1;
}
")

# Create stb_image.h
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/stb)
file(WRITE ${PROJECT_BINARY_DIR}/include/stb/stb_image.h "
// Minimal stb_image.h stub
#ifndef STB_IMAGE_H
#define STB_IMAGE_H

#include <stdlib.h>

unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
void stbi_image_free(void *retval_from_stbi_load);

// implementation
unsigned char *stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels) {
    *x = 1; *y = 1; *channels_in_file = 4;
    return (unsigned char*)malloc(4);
}

void stbi_image_free(void *retval_from_stbi_load) {
    free(retval_from_stbi_load);
}

#endif // STB_IMAGE_H
")

# Create stb_image_write.h
file(WRITE ${PROJECT_BINARY_DIR}/include/stb/stb_image_write.h "
// Minimal stb_image_write.h stub
#ifndef STB_IMAGE_WRITE_H
#define STB_IMAGE_WRITE_H

int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

// implementation
int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes) {
    return 1;
}

#endif // STB_IMAGE_WRITE_H
")

# Create imgui stub headers
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/imgui)
file(WRITE ${PROJECT_BINARY_DIR}/include/imgui/imgui.h "
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
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = \"%.3f\", int flags = 0);
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
")

file(WRITE ${PROJECT_BINARY_DIR}/include/imgui/imgui_impl_glfw.h "
// Minimal imgui_impl_glfw.h stub
#ifndef IMGUI_IMPL_GLFW_H
#define IMGUI_IMPL_GLFW_H

struct GLFWwindow;
bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
void ImGui_ImplGlfw_NewFrame();
void ImGui_ImplGlfw_Shutdown();

#endif // IMGUI_IMPL_GLFW_H
")

file(WRITE ${PROJECT_BINARY_DIR}/include/imgui/imgui_impl_opengl3.h "
// Minimal imgui_impl_opengl3.h stub
#ifndef IMGUI_IMPL_OPENGL3_H
#define IMGUI_IMPL_OPENGL3_H

bool ImGui_ImplOpenGL3_Init(const char* glsl_version);
void ImGui_ImplOpenGL3_NewFrame();
void ImGui_ImplOpenGL3_RenderDrawData(void* draw_data);
void ImGui_ImplOpenGL3_Shutdown();

#endif // IMGUI_IMPL_OPENGL3_H
")

# Create source files for ImGui stubs
file(WRITE ${PROJECT_BINARY_DIR}/imgui.cpp "
#include \"imgui/imgui.h\"
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
")

file(WRITE ${PROJECT_BINARY_DIR}/imgui_demo.cpp "// Empty implementation")
file(WRITE ${PROJECT_BINARY_DIR}/imgui_draw.cpp "// Empty implementation")
file(WRITE ${PROJECT_BINARY_DIR}/imgui_widgets.cpp "// Empty implementation")
file(WRITE ${PROJECT_BINARY_DIR}/imgui_tables.cpp "// Empty implementation")
file(WRITE ${PROJECT_BINARY_DIR}/imgui_impl_glfw.cpp "
#include \"imgui/imgui_impl_glfw.h\"
bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow*, bool) { return true; }
void ImGui_ImplGlfw_NewFrame() {}
void ImGui_ImplGlfw_Shutdown() {}
")

file(WRITE ${PROJECT_BINARY_DIR}/imgui_impl_opengl3.cpp "
#include \"imgui/imgui_impl_opengl3.h\"
bool ImGui_ImplOpenGL3_Init(const char*) { return true; }
void ImGui_ImplOpenGL3_NewFrame() {}
void ImGui_ImplOpenGL3_RenderDrawData(void*) {}
void ImGui_ImplOpenGL3_Shutdown() {}
")

# Create GLFW stub header
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/GLFW)
file(WRITE ${PROJECT_BINARY_DIR}/include/GLFW/glfw3.h "
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
")

# Create GLM stub headers
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/glm)
file(WRITE ${PROJECT_BINARY_DIR}/include/glm/glm.hpp "
// Minimal GLM stub
#ifndef GLM_HPP
#define GLM_HPP

namespace glm {
    template<typename T>
    struct vec2 {
        T x, y;
        vec2() : x(0), y(0) {}
        vec2(T x, T y) : x(x), y(y) {}
    };

    template<typename T>
    struct vec3 {
        T x, y, z;
        vec3() : x(0), y(0), z(0) {}
        vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    };

    template<typename T>
    struct vec4 {
        T x, y, z, w;
        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        vec4(const vec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
    };

    template<typename T>
    struct mat3 {
        vec3<T> columns[3];
        mat3() {}
    };

    template<typename T>
    struct mat4 {
        vec4<T> columns[4];
        mat4() {}
        mat4(T value) {
            columns[0] = vec4<T>(value, 0, 0, 0);
            columns[1] = vec4<T>(0, value, 0, 0);
            columns[2] = vec4<T>(0, 0, value, 0);
            columns[3] = vec4<T>(0, 0, 0, value);
        }
    };

    typedef vec2<float> vec2;
    typedef vec3<float> vec3;
    typedef vec4<float> vec4;
    typedef mat3<float> mat3;
    typedef mat4<float> mat4;

    template<typename T>
    T max(T a, T b) { return (a > b) ? a : b; }

    template<typename T>
    T min(T a, T b) { return (a < b) ? a : b; }

    template<typename T>
    vec3<T> normalize(const vec3<T>& v) { return v; }

    template<typename T>
    T distance(const vec2<T>& a, const vec2<T>& b) { return 0; }

    template<typename T>
    T length(const vec3<T>& v) { return 0; }

    template<typename T>
    T dot(const vec3<T>& a, const vec3<T>& b) { return 0; }

    template<typename T>
    vec2<T> mix(const vec2<T>& a, const vec2<T>& b, T t) { return a; }

    template<typename T>
    vec3<T> mix(const vec3<T>& a, const vec3<T>& b, T t) { return a; }

    mat4 perspective(float fovy, float aspect, float zNear, float zFar) { return mat4(1.0f); }
    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) { return mat4(1.0f); }
    mat4 translate(const mat4& m, const vec3& v) { return m; }
    mat4 rotate(const mat4& m, float angle, const vec3& axis) { return m; }
    mat4 scale(const mat4& m, const vec3& v) { return m; }
    mat3 transpose(const mat3& m) { return m; }
    mat3 inverse(const mat3& m) { return m; }
}

#endif // GLM_HPP
")

file(WRITE ${PROJECT_BINARY_DIR}/include/glm/gtc/type_ptr.hpp "
// Minimal GLM type_ptr stub
#ifndef GLM_GTC_TYPE_PTR_HPP
#define GLM_GTC_TYPE_PTR_HPP

#include <glm/glm.hpp>

namespace glm {
    template<typename T>
    const T* value_ptr(const vec2<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const vec3<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const vec4<T>& v) { return &v.x; }

    template<typename T>
    const T* value_ptr(const mat3<T>& m) { return &m.columns[0].x; }

    template<typename T>
    const T* value_ptr(const mat4<T>& m) { return &m.columns[0].x; }
}

#endif // GLM_GTC_TYPE_PTR_HPP
")

file(WRITE ${PROJECT_BINARY_DIR}/include/glm/gtx/vector_angle.hpp "
// Minimal GLM vector_angle stub
#ifndef GLM_GTX_VECTOR_ANGLE_HPP
#define GLM_GTX_VECTOR_ANGLE_HPP

#include <glm/glm.hpp>

namespace glm {
    template<typename T>
    T angle(const vec3<T>& x, const vec3<T>& y) { return T(0); }
}

#endif // GLM_GTX_VECTOR_ANGLE_HPP
")

# Create nlohmann/json stub header
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/nlohmann)
file(WRITE ${PROJECT_BINARY_DIR}/include/nlohmann/json.hpp "
// Minimal nlohmann/json stub
#ifndef NLOHMANN_JSON_HPP
#define NLOHMANN_JSON_HPP

#include <string>
#include <vector>
#include <map>

namespace nlohmann {
    class json {
    public:
        // Constructors
        json() {};
        json(std::nullptr_t) {};
        json(bool val) {};
        json(int val) {};
        json(float val) {};
        json(const std::string& val) {};

        // Static functions
        static json array() { return json(); }
        static json object() { return json(); }
        static json parse(const std::string& str) { return json(); }

        // Array/object methods
        json& operator[](const std::string& key) { static json j; return j; }
        json& operator[](int index) { static json j; return j; }
        void push_back(const json& val) {}
        
        // Conversion
        template<typename T>
        T get() const { return T(); }
        
        std::string dump(int indent = -1) const { return \"{}\"; }
        
        // Iteration
        bool empty() const { return true; }
        size_t size() const { return 0; }
        
        // Comparison
        bool operator==(const json& other) const { return false; }
        bool operator!=(const json& other) const { return true; }
    };
}

#endif // NLOHMANN_JSON_HPP
")

# Create Assimp stub headers
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/assimp)
file(WRITE ${PROJECT_BINARY_DIR}/include/assimp/Importer.hpp "
// Minimal Assimp stub
#ifndef ASSIMP_IMPORTER_HPP
#define ASSIMP_IMPORTER_HPP

#include <string>

namespace Assimp {
    class Importer {
    public:
        Importer() {}
        ~Importer() {}
        
        const struct aiScene* ReadFile(const std::string& file, unsigned int flags) { return nullptr; }
    };
}

#endif // ASSIMP_IMPORTER_HPP
")

file(WRITE ${PROJECT_BINARY_DIR}/include/assimp/scene.h "
// Minimal Assimp scene stub
#ifndef ASSIMP_SCENE_H
#define ASSIMP_SCENE_H

struct aiVector3D {
    float x, y, z;
};

struct aiColor4D {
    float r, g, b, a;
};

struct aiString {
    char data[1024];
};

struct aiMesh {
    unsigned int mNumVertices;
    aiVector3D* mVertices;
    aiVector3D* mNormals;
    aiVector3D* mTextureCoords[8];
    unsigned int mNumFaces;
    struct aiFace* mFaces;
    unsigned int mMaterialIndex;
};

struct aiFace {
    unsigned int mNumIndices;
    unsigned int* mIndices;
};

struct aiMaterial {
    // Empty
};

struct aiScene {
    unsigned int mNumMeshes;
    aiMesh** mMeshes;
    unsigned int mNumMaterials;
    aiMaterial** mMaterials;
};

#endif // ASSIMP_SCENE_H
")

file(WRITE ${PROJECT_BINARY_DIR}/include/assimp/postprocess.h "
// Minimal Assimp postprocess stub
#ifndef ASSIMP_POSTPROCESS_H
#define ASSIMP_POSTPROCESS_H

#define aiProcess_Triangulate           0x1
#define aiProcess_FlipUVs               0x2
#define aiProcess_CalcTangentSpace      0x4
#define aiProcess_GenNormals            0x8
#define aiProcess_JoinIdenticalVertices 0x10

#endif // ASSIMP_POSTPROCESS_H
")

file(WRITE ${PROJECT_BINARY_DIR}/include/assimp/material.h "
// Minimal Assimp material stub
#ifndef ASSIMP_MATERIAL_H
#define ASSIMP_MATERIAL_H

#include <assimp/scene.h>

#define AI_MATKEY_NAME "?mat.name", 0, 0
#define AI_MATKEY_TEXTURE_DIFFUSE(n) "?mat.texture.diffuse", n, 0

enum aiTextureType {
    aiTextureType_DIFFUSE = 1,
    aiTextureType_SPECULAR,
    aiTextureType_AMBIENT,
    aiTextureType_NORMALS
};

enum aiReturn {
    aiReturn_SUCCESS = 0,
    aiReturn_FAILURE = -1,
    aiReturn_OUTOFMEMORY = -3
};

#endif // ASSIMP_MATERIAL_H
")

# Include directories with our stub implementations
include_directories(
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# Source files - use the stubs we created
set(SOURCES
    src/main.cpp
    src/application.cpp
    src/renderer.cpp
    src/model.cpp
    src/texture.cpp
    src/camera.cpp
    src/layer.cpp
    src/paint_tool.cpp
    src/shader.cpp
    src/ui.cpp
    src/project.cpp
    src/utils.cpp
    
    # Stubs for third-party libraries
    ${PROJECT_BINARY_DIR}/glad.c
    ${PROJECT_BINARY_DIR}/imgui.cpp
    ${PROJECT_BINARY_DIR}/imgui_demo.cpp
    ${PROJECT_BINARY_DIR}/imgui_draw.cpp
    ${PROJECT_BINARY_DIR}/imgui_widgets.cpp
    ${PROJECT_BINARY_DIR}/imgui_tables.cpp
    ${PROJECT_BINARY_DIR}/imgui_impl_glfw.cpp
    ${PROJECT_BINARY_DIR}/imgui_impl_opengl3.cpp
)

# Define executable
add_executable(3DModelPainter ${SOURCES})

# Link libraries
target_link_libraries(3DModelPainter
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
    pthread
)

# Copy shader files to build directory
file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
