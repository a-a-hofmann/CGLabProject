//
//  Brick.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#include "Brick.h"

Brick::Brick(float x, float y, float width, float height) : Cuboid(x, y, width, height)
{
    _modelName = "brick";
}

bool Brick::destroyOnCollision()
{
    return true;
}