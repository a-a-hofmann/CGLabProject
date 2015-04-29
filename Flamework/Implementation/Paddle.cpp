//
//  Paddle.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#include "Paddle.h"


Paddle::Paddle(float x, float y, float width, float height, float dvx) : Cuboid(x, y, width, height), _dvx(dvx)
{
    _modelName = "paddle";
}

bool Paddle::detectCollision(Ball& ball)
{
    if(Cuboid::detectCollision(ball))
    {
        ball._vx += _vx/2;
        _vx = 0;
        return true;
    }
    _vx = 0;
    
    return false;
}