//
//  Wall.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#include "Wall.h"


Wall::Wall(float x, float y, float width, float height, bool horizontal, bool endRoundOnCollision) :
Cuboid(x, y, width, height),
_endRoundOnCollision(endRoundOnCollision)
{
    if(horizontal)
        _modelName = "horizontalwall";
    else
        _modelName = "verticalwall";
}

bool Wall::endRoundOnCollision()
{
    return _endRoundOnCollision;
}