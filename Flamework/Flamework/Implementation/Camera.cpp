//
//  Camera.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#include "Camera.h"
#include <cassert>

Camera::Camera()
{
    _position = vmml::vec3f::ZERO;
    _viewMatrix = vmml::mat4f::IDENTITY;
    _projectionMatrix = vmml::mat4f::IDENTITY;
}

Camera::~Camera()
{
    
}

void Camera::lookAt(const vmml::vec3f &point, const vmml::vec3f &up)
{
    
    vmml::vec3f zaxis = vmml::normalize(_position - point);
    vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
    vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);
    
    _viewMatrix.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, _position)));
    _viewMatrix.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, _position)));
    _viewMatrix.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, _position)));
    _viewMatrix.set_row(3, vmml::vec4f(0, 0, 0, 1.0));
}

void Camera::moveCamera(const vmml::vec3f &translation)
{
    _position = translation;
}

void Camera::rotateCamera(const vmml::vec3f &axis, float rotation)
{
    _position = vmml::create_rotation(rotation, axis);
}

void Camera::rotateCamera(const vmml::mat3f &rotation)
{
    _position = rotation * _position;
}

void Camera::setProjection(const float fov, const float aspect)
{
    glm::mat4 proj = glm::perspective(fov, aspect, 0.1f, 100.0f);
    
    _projectionMatrix.set_column(0, (vmml::vec4f(proj[0][0], proj[0][1], proj[0][2], proj[0][3])));
    _projectionMatrix.set_column(1, (vmml::vec4f(proj[1][0], proj[1][1], proj[1][2], proj[1][3])));
    _projectionMatrix.set_column(2, (vmml::vec4f(proj[2][0], proj[2][1], proj[2][2], proj[2][3])));
}

vmml::mat4f Camera::getViewMatrix() const
{
    return _viewMatrix;
}

vmml::mat4f Camera::getProjectionMatrix() const
{
    return _projectionMatrix;
}

vmml::vec3f Camera::getPosition() const
{
    return _position;
}