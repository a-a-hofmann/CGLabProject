//
//  Paddle.cpp
//  Flamework
//
//  Created by Alexander Hofmann on 29/04/15.
//
//

#include "Paddle.h"


Paddle::Paddle() : Cuboid(0.0, -4.5, 4.5, 0.3)
{
    _modelName = "paddle";
    _dvx = 0.5;
}


Paddle::Paddle(float x, float y, float width, float height, float dvx) : Cuboid(x, y, width, height), _dvx(dvx)
{
    _modelName = "paddle";
}

Paddle::~Paddle()
{
    
}

bool Paddle::detectCollision(BallPtr ball)
{
    if(Cuboid::detectCollision(ball))
    {
        ball->_vx += _vx/2;
        _vx = 0;
        
        return true;
    }
    _vx = 0;
    
    return false;
}