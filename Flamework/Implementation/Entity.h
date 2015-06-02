//
//  Entity.h
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#ifndef __Flamework__Entity__
#define __Flamework__Entity__

#include <iostream>
#include "addendum.hpp"

// An object of the game
class Entity {
protected:
    const char* _modelName = "";    // name of model for drawing
    
public:
    float _x, _y;    // position
    
    // Construct entity
    // x, y: position
    Entity(float x, float y) : _x(x), _y(y) {}
    
    // Get model name for drawing of model (must be loaded beforehand using loadModel)
    char* getModelName() { return (char*)_modelName; }
    
    virtual vmml::vec2f getPosition2f() const;
    virtual vmml::vec3f getPosition3f() const;
};

#endif /* defined(__Flamework__Entity__) */
