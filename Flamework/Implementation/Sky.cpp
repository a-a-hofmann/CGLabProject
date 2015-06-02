//
//  Sky.cpp
//  Flamework
//
//  Created by Alex Hofmann on 02/06/15.
//
//

#include "Sky.h"

Sky::Sky() : _modelMatrix(vmml::mat4f::IDENTITY), _name("")
{}

Sky::Sky(const std::string &name) : _name(name), _modelMatrix(vmml::mat4f::IDENTITY)
{}

Sky::~Sky()
{}


void Sky::transformModelMatrix(const vmml::mat4f &t)
{
    _modelMatrix =  _modelMatrix * t;
}

std::string Sky::getName() const
{
    return _name;
}

vmml::mat4f Sky::getModelMatrix() const
{
    return _modelMatrix;
}