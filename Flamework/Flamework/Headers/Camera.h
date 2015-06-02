//
//  Camera.h
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//

#ifndef FRAMEWORK_CAMERA_H
#define FRAMEWORK_CAMERA_H

#include "addendum.hpp"
#include <memory>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/matrix_access.hpp>


class Camera
{
public:
    
    Camera();
    Camera(vmml::vec3f pos);
    ~Camera();
    
    void lookAt(const vmml::vec3f &point, const vmml::vec3f &up = vmml::vec3f::UP);
    void moveCamera(const vmml::vec3f &translation);
    void rotateCamera(const vmml::vec3f &axis, float rotation);
    void rotateCamera(const vmml::mat3f &rotation);
    void setProjection(const float fov = 2.3f, const float aspect = 1.0f);
    
    vmml::mat4f getViewMatrix() const;
    vmml::mat4f getProjectionMatrix() const;
    vmml::vec3f getPosition() const;
    

    
    
private:
    
    vmml::vec3f _position;
    vmml::mat4f _viewMatrix, _projectionMatrix;
    
    float _speed;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif
