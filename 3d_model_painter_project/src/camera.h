#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, 
           float pitch = 0.0f);
    
    // Get view matrix
    glm::mat4 getViewMatrix() const;
    
    // Get projection matrix
    glm::mat4 getProjectionMatrix() const;
    
    // Process keyboard input
    void processKeyboard(CameraMovement direction, float deltaTime);
    
    // Process mouse movement
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    // Process mouse scroll
    void processMouseScroll(float yoffset);
    
    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    
    // Set aspect ratio
    void setAspectRatio(float ratio) { aspectRatio = ratio; }
    
private:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    // Euler angles
    float yaw;
    float pitch;
    
    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    float aspectRatio;
    
    // Update camera vectors
    void updateCameraVectors();
};
