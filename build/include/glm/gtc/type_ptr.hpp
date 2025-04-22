
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
