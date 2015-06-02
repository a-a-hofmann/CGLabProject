//
//  Entity.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 27/04/15.
//
//

#include "Entity.h"


vmml::vec2f Entity::getPosition2f() const
{
    return vmml::vec2f(_x, _y);
}

vmml::vec3f Entity::getPosition3f() const
{
    return vmml::vec3f(_x, _y, 0.0);
}