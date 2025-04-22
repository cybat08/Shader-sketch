
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
