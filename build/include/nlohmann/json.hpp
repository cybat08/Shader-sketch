
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
        
        std::string dump(int indent = -1) const { return "{}"; }
        
        // Iteration
        bool empty() const { return true; }
        size_t size() const { return 0; }
        
        // Comparison
        bool operator==(const json& other) const { return false; }
        bool operator!=(const json& other) const { return true; }
    };
}

#endif // NLOHMANN_JSON_HPP
