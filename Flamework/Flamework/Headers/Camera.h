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
#include <iostream>	
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/matrix_access.hpp>

//#include "vector.hpp"
//#include "matrix.hpp"

class Camera
{
public:
    
    Camera();
    ~Camera();
    
    void lookAt(const vmml::vec3f &point, const vmml::vec3f &up = vmml::vec3f::UP);
    void moveCamera(const vmml::vec3f &translation);
    void rotateCamera(const vmml::vec3f &axis, float rotation);
    void rotateCamera(const vmml::mat3f &rotation);
    
    vmml::mat4f getViewMatrix() const;
    vmml::mat4f getProjectionMatrix() const;
    
    
private:
    
    vmml::vec3f _position, _view;
    vmml::mat4f _viewMatrix, _projection;
    float _speed;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif
